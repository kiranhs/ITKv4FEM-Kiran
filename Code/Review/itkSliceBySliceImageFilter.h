/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkSliceBySliceImageFilter.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkSliceBySliceImageFilter_h
#define __itkSliceBySliceImageFilter_h

#include "itkImageToImageFilter.h"


namespace itk {

/**
 * \class SliceBySliceImageFilter
 * \brief Apply a filter or a pipeline slice by slice on an image
 *
 * SliceBySliceImageFilter let the user apply a filter or a pipeline
 * of filters on the slices of an image. The filters must work on images
 * smaller of one dimension than the input and output images of the 
 * SliceBySliceImageFilter - if the SliceBySliceImageFilter work on 3D
 * images, the filters used internally must work on 2D images.
 *
 * The dimension along which the slices are extracted can be selected with SetDimension()
 * and defaults to the last one.
 *
 * SliceBySliceImageFilter takes the input and the output filters of a pipeline
 * as parameter. They can be set with SetInputFilter() and SetOutputFilter().
 * The pipeline will be run once per slice.
 *
 * If there is only one filter to apply to the slices of the input image,
 * the SetFilter() method can be used to set the filter passed as parameter
 * both as the input and as the output filter.
 *
 * SliceBySliceImageFilter can take several images as input. In that case, the same
 * number of slices will be passed to the input filter. If the output filter
 * produce several output slices, SliceBySliceImageFilter produce the same number
 * of output images. The input images are passed with the same input number
 * to the input filter - if SetInput( 3, img ) is used on the SliceBySliceImageFilter
 * the corresponding slice will be passed to the input filter with SetInput( 3, img ).
 * See http://www.itk.org/pipermail/insight-users/2008-May/026112.html for an
 * example of usage of that feature with MaskImageFilter.
 *
 * The requested region is always enlarged by the filter to cover entirely
 * the whole slice - however, only the slice in the requested region are
 * processed (the requested region is not enlarged to the whole image if
 * not needed).
 *
 * The ouput images of SliceBySliceImageFilter must be of the same size than the
 * input images. All the input images must be of the same pixel type. All the
 * output images must be of the same pixel type.
 *
 * \author Gaetan Lehmann
 *
 * This class was taken from the Insight Journal paper:
 * http://insight-journal.org/midas/handle.php?handle=1926/368
 *
 */

template<class TInputImage,
  class TOutputImage,
  class TInputFilter=ImageToImageFilter< 
    Image< ITK_TYPENAME TInputImage::PixelType,  ::itk::GetImageDimension<TInputImage >::ImageDimension - 1 >,
    Image< ITK_TYPENAME TOutputImage::PixelType, ::itk::GetImageDimension<TOutputImage>::ImageDimension - 1 > >,
  class TOutputFilter=ITK_TYPENAME TInputFilter::Superclass,
  class TInternalInputImage=ITK_TYPENAME TInputFilter::InputImageType,
  class TInternalOutputImage=ITK_TYPENAME TOutputFilter::OutputImageType >
class ITK_EXPORT SliceBySliceImageFilter : 
public ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef SliceBySliceImageFilter                         Self;
  typedef ImageToImageFilter<TInputImage,TOutputImage>    Superclass;
  typedef SmartPointer<Self>                              Pointer;
  typedef SmartPointer<const Self>                        ConstPointer;
  
  /** Superclass typedefs. */
  typedef typename Superclass::InputImagePointer InputImagePointer;

  /** Standard New method. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(SliceBySliceImageFilter, ImageToImageFilter);
 
  /** Image related typedefs. */
  typedef TInputImage                           InputImageType;
  typedef typename TInputImage::RegionType      RegionType;
  typedef typename TInputImage::SizeType        SizeType;
  typedef typename TInputImage::IndexType       IndexType;
  typedef typename TInputImage::PixelType       PixelType;
  typedef typename TInputImage::OffsetType      OffsetType;
  
  typedef TOutputImage                          OutputImageType;
  typedef typename TOutputImage::PixelType      OutputPixelType;

  typedef TInputFilter                          InputFilterType;
  typedef TOutputFilter                         OutputFilterType;
  
  typedef TInternalInputImage                         InternalInputImageType;
  typedef typename InternalInputImageType::RegionType InternalRegionType;
  typedef typename InternalInputImageType::SizeType   InternalSizeType;
  typedef typename InternalInputImageType::IndexType  InternalIndexType;
  typedef typename InternalInputImageType::OffsetType InternalOffsetType;
  typedef typename InternalInputImageType::PixelType  InternalInputPixelType;

  typedef TInternalOutputImage                        InternalOutputImageType;
  typedef typename InternalOutputImageType::PixelType InternalOutputPixelType;

  /** Image related typedefs. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TInputImage::ImageDimension);

  itkStaticConstMacro(InternalImageDimension, unsigned int,
                      InternalInputImageType::ImageDimension);

  itkSetMacro(Dimension, unsigned int);
  itkGetConstMacro(Dimension, unsigned int);

  void SetFilter(InputFilterType * filter);
  InputFilterType * GetFilter()
    {
    return this->m_InputFilter;
    }

  const InputFilterType * GetFilter() const
    {
    return this->m_InputFilter;
    }

  void SetInputFilter( InputFilterType * filter );
  itkGetObjectMacro( InputFilter, InputFilterType );

  void SetOutputFilter( OutputFilterType * filter );
  itkGetObjectMacro( OutputFilter, OutputFilterType );

  /** The index of the slice currently processed by the filter. This is intended to be
   * used with the IterationEvent sent before the processing of each object. It contains
   * a relevant value only during the filter update.
   */
  itkGetConstMacro(SliceIndex, long);

protected:
  SliceBySliceImageFilter();
  ~SliceBySliceImageFilter() {};

  void GenerateData();

  void PrintSelf(std::ostream& os, Indent indent) const;

  void GenerateInputRequestedRegion();
  void EnlargeOutputRequestedRegion(DataObject *itkNotUsed(output));

private:
  SliceBySliceImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  unsigned int                            m_Dimension;
  typename InputFilterType::Pointer       m_InputFilter;
  typename OutputFilterType::Pointer      m_OutputFilter;
  long                                    m_SliceIndex;
};

}

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkSliceBySliceImageFilter.txx"
#endif

#endif
