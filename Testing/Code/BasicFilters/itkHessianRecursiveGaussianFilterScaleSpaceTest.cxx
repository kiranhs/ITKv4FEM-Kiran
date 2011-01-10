/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkHessianRecursiveGaussianFilterScaleSpaceTest.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <itkImage.h>
#include <itkHessianRecursiveGaussianImageFilter.h>
#include <itkImageRegionIteratorWithIndex.h>

// This test creates an image varying as a 1D Gaussian in the X direction
// for different values of sigma, and checks the scale-space response of 
// the xx component of the Hessian at the center of the Gaussian.
// If NormalizeAcrossScale works correctly, the filter should yield the 
// same Hxx across different scales.

int itkHessianRecursiveGaussianFilterScaleSpaceTest(int, char* [] ) 
{
  const unsigned int Dimension = 3;
  typedef double PixelType;
  typedef itk::Image<PixelType,Dimension> ImageType;
  typedef itk::Index<Dimension> IndexType;
  typedef itk::Size<Dimension> SizeType;
  typedef itk::ImageRegion<Dimension> RegionType;
  typedef ImageType::PointType PointType;
  typedef ImageType::SpacingType SpacingType;

  ImageType::Pointer inputImage = ImageType::New();
  
  SizeType size;
  size.Fill(21);
  size[0] = 401;

  IndexType start;
  start.Fill(0);

  RegionType region;
  region.SetIndex(start);
  region.SetSize(size);

  PointType origin;
  origin.Fill(-1.25);
  origin[0] = -20.0;

  SpacingType spacing;
  spacing.Fill(0.1);

  inputImage->SetOrigin(origin);
  inputImage->SetSpacing(spacing);

  inputImage->SetLargestPossibleRegion(region);
  inputImage->SetBufferedRegion(region);
  inputImage->SetRequestedRegion(region);
  inputImage->Allocate();

  typedef itk::ImageRegionIteratorWithIndex<ImageType> IteratorType;

  const unsigned int numberOfScales = 4;
  double scales[numberOfScales];
  scales[0] = 1.0;
  scales[1] = 2.0;
  scales[2] = 3.0;
  scales[3] = 5.0;

  for (unsigned int i=0; i<numberOfScales; i++)
    {
    IteratorType it(inputImage, inputImage->GetRequestedRegion());
  
    PointType point;
    double objectSize = scales[i];

    // Fill the image with a 1D Gaussian along X with sigma equal to the current scale
    // The Gaussian is not normalized, since it should have the same peak value across
    // scales, only sigma should change
    while(!it.IsAtEnd()) 
      {
      inputImage->TransformIndexToPhysicalPoint(it.GetIndex(),point);
      double value = vcl_exp(-point[0]*point[0] / (2.0*objectSize*objectSize));
      it.Set(value);
      ++it;
      }

    // Compute the hessian using NormalizeAcrossScale true
    typedef itk::HessianRecursiveGaussianImageFilter<ImageType> FilterType;
              
    typedef FilterType::OutputImageType HessianImageType;

    FilterType::Pointer filter = FilterType::New();
    filter->SetInput(inputImage); 
    filter->SetSigma(objectSize); 
    filter->SetNormalizeAcrossScale(true);
    filter->Update();

    HessianImageType::Pointer outputImage = filter->GetOutput();

    // Get the value at the center of the image, the location of the peak of the Gaussian
    PointType center;
    center.Fill(0.0);
    
    IndexType centerIndex;

    typedef HessianImageType::PixelType HessianType;

    outputImage->TransformPhysicalPointToIndex(center,centerIndex);

    // Irrespective of the scale, the Hxx component should be the same 
    double centerHxx = outputImage->GetPixel(centerIndex)[0];

    if (centerHxx > -35.46 || centerHxx < -35.47)
      {
      return EXIT_FAILURE;
      }
    }

  return EXIT_SUCCESS;
}

