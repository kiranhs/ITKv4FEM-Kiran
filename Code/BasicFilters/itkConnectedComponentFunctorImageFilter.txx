/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkConnectedComponentFunctorImageFilter.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkConnectedComponentFunctorImageFilter_txx
#define __itkConnectedComponentFunctorImageFilter_txx

#include "itkConnectedComponentFunctorImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkNumericTraits.h"
#include "itkProgressReporter.h"
#include "itkEquivalencyTable.h"
#include "itkConstShapedNeighborhoodIterator.h"
#include "itkConstantBoundaryCondition.h"

namespace itk
{

template< class TInputImage, class TOutputImage, class TFunctor, class TMaskImage >
void
ConnectedComponentFunctorImageFilter< TInputImage, TOutputImage, TFunctor, TMaskImage >
::GenerateData()
{
  // create an equivalency table
  EquivalencyTable::Pointer eqTable = EquivalencyTable::New();

  InputPixelType     value, neighborValue;
  OutputPixelType    label, originalLabel, neighborLabel;
  OutputPixelType    maxLabel = NumericTraits<OutputPixelType>::Zero;
  const OutputPixelType maxPossibleLabel=NumericTraits<OutputPixelType>::max();

  typename TOutputImage::Pointer output = this->GetOutput();
  typename TInputImage::ConstPointer input = this->GetInput();

  // Allocate the output and initialize to unlabeled
  this->AllocateOutputs();
  output->FillBuffer(maxPossibleLabel);
  
  // Set up the boundary condition to be zero padded (used on output image)
  ConstantBoundaryCondition<TOutputImage> BC;
  BC.SetConstant(NumericTraits<OutputPixelType>::Zero);

  // Neighborhood iterators.  Let's use a shaped neighborhood so we can
  // restrict the access to face connected neighbors. These iterators
  // will be applied to both the input and the output image
  typedef ConstShapedNeighborhoodIterator<TInputImage> InputNeighborhoodIteratorType;
  typedef ConstShapedNeighborhoodIterator<TOutputImage> OutputNeighborhoodIteratorType;

  SizeType kernelRadius;
  kernelRadius.Fill(1);

  InputNeighborhoodIteratorType init(kernelRadius, input, output->GetRequestedRegion());
  OutputNeighborhoodIteratorType onit(kernelRadius, output, output->GetRequestedRegion());
  onit.OverrideBoundaryCondition(&BC); // assign the boundary condition
  
  // only activate the indices that are "previous" to the current
  // pixel and face connected (exclude the center pixel from the
  // neighborhood)
  //
  unsigned int d;
  typename OutputNeighborhoodIteratorType::OffsetType offset;

  if (!this->m_FullyConnected)
    {
    // only activate the "previous" neighbors that are face connected
    // to the current pixel. do not include the center pixel
    offset.Fill(0);
    for (d=0; d < InputImageType::ImageDimension; ++d)
      {
      offset[d] = -1;
      init.ActivateOffset(offset);
      onit.ActivateOffset(offset);
      offset[d] = 0;
      }
    }
  else
    {
    // activate all "previous" neighbors that are face+edge+vertex
    // connected to the current pixel. do not include the center pixel
    unsigned int centerIndex = onit.GetCenterNeighborhoodIndex();
    for (d=0; d < centerIndex; d++)
      {
      offset = onit.GetOffset(d);
      init.ActivateOffset(offset);
      onit.ActivateOffset(offset);
      }
    }

  // along with a neighborhood iterator on the output, use a standard
  // iterator on the input and output
  ImageRegionConstIterator<InputImageType> it;
  ImageRegionIterator<OutputImageType> oit;
  it = ImageRegionConstIterator<InputImageType>(input, output->GetRequestedRegion());
  oit = ImageRegionIterator<OutputImageType>(output, output->GetRequestedRegion());

  // Setup a progress reporter.  We have 2 stages to the algorithm so
  // pretend we have 2 times the number of pixels
  ProgressReporter progress(this, 0, 2*output->GetRequestedRegion().GetNumberOfPixels());

  // if the mask is set mark pixels not under the mask as background
  typename TMaskImage::ConstPointer mask = this->GetMaskImage();
  if (mask)
    {
    ImageRegionConstIterator<MaskImageType> mit;
    mit = ImageRegionConstIterator<MaskImageType>(mask,output->GetRequestedRegion());

    mit.GoToBegin();
    oit.GoToBegin();
    while (!mit.IsAtEnd())
      {
      if ( mit.Get() == NumericTraits<MaskPixelType>::Zero)
        {
        // mark pixel as unlabeled
        oit.Set(NumericTraits<OutputPixelType>::Zero);
        }
      
      ++mit;
      ++oit;
      }
    }

  // iterate over the image, labeling the objects and defining
  // equivalence classes.  Use the neighborhood iterator to access the
  // "previous" neighbor pixels and an output iterator to access the
  // current pixel
  init.GoToBegin();
  onit.GoToBegin();
  it.GoToBegin();
  oit.GoToBegin();
  while ( !oit.IsAtEnd() )
    {
    // Get the current pixel label
    label = oit.Get();
    value = it.Get();
    originalLabel = label;

    // If the pixel is not background
    if (label != NumericTraits<OutputPixelType>::Zero)
      {
      // loop over the "previous" neighbors to find labels.  this loop
      // may establish one or more new equivalence classes
      typename InputNeighborhoodIteratorType::ConstIterator isIt;
      typename OutputNeighborhoodIteratorType::ConstIterator osIt;
      for (isIt=init.Begin(),osIt=onit.Begin(); !osIt.IsAtEnd(); ++isIt,++osIt)
        {
        // get the label of the pixel previous to this one along a
        // particular dimension (neighbors activated in neighborhood iterator)
        neighborLabel = osIt.Get();

        // if the previous pixel has a label, verify equivalence or
        // establish a new equivalence
        if (neighborLabel != NumericTraits<OutputPixelType>::Zero)
          {
          // see if current pixel is connected to its neighbor
          neighborValue = isIt.Get(); 
          if (m_Functor(value,neighborValue))
            {
            // if current pixel is unlabeled, then copy the label from neighbor
            if (label == maxPossibleLabel)
              {
              // copy the label from the previous pixel
              label = neighborLabel;
              }
            // else if current pixel has a label that is not already
            // equivalent to the label of the previous pixel, then setup
            // a new equivalence.  
            else if ((label != neighborLabel)
                     && (eqTable->RecursiveLookup(label)
                         != eqTable->RecursiveLookup(neighborLabel))) 
              {
              eqTable->Add(label, neighborLabel);
              }
            }
          }
        }

      // if none of the "previous" neighbors were set, then make a new label
      if (originalLabel == label)
        {
        // create a new entry label
        if (maxLabel == maxPossibleLabel)
          {
          itkWarningMacro(
            << "ConnectedComponentFunctorImageFilter::GenerateData: Number of labels " << (long) maxLabel
            << " exceeds number of available labels " << (long) maxPossibleLabel << " for the output type." );
          }
        else
          {
          ++maxLabel;
          }
        
        // assign the new label
        label = maxLabel;
        }

      // Finally, set the output pixel to whatever label we have
      if (label != originalLabel)
        {
        oit.Set( label );
        }
      }

    // move the iterators
    ++init;
    ++onit;
    ++it;
    ++oit;
    progress.CompletedPixel();
    }

  // Flatten the equavalency table
  eqTable->Flatten();

  // remap the labels
  oit.GoToBegin();
  while ( !oit.IsAtEnd() )
    {
    label = oit.Get();
    // if pixel has a label, write out the final equivalence
    if (label != NumericTraits<OutputPixelType>::Zero)
      {
      oit.Set( eqTable->Lookup( label ) );
      }
    ++oit;
    progress.CompletedPixel();
    }
}

} // end namespace itk

#endif
