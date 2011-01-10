/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkImageToListSampleFilterTest2.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
// The example tests the class itk::Statistics::ImageToListSampleFilter.
// The class is capable of generating an itk::ListSample from an image
// confined to a mask (if specified). This test exercises that.

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkImageToListSampleFilter.h"
#include "itkImageRegionIteratorWithIndex.h"

int itkImageToListSampleFilterTest2(int, char* [] )
{
  const unsigned int MeasurementVectorSize = 8;
  typedef unsigned long MeasurementComponentType;
  typedef itk::FixedArray< MeasurementComponentType, MeasurementVectorSize > PixelType;

  const unsigned int ImageDimension = 3;
  typedef itk::Image< PixelType, ImageDimension > ImageType;

  typedef itk::Image< unsigned char, ImageDimension > MaskImageType;

  ImageType::Pointer image = ImageType::New();
  ImageType::IndexType start;
  ImageType::SizeType  size;

  start.Fill( 0 );
  size.Fill( 10 );

  ImageType::RegionType region( start, size );
  image->SetRegions( region );
  image->Allocate();
  typedef itk::ImageRegionIteratorWithIndex< ImageType > IteratorType;
  IteratorType it( image, region );
  it.GoToBegin();
  while (!it.IsAtEnd())
    {
    PixelType value;
    for( unsigned int i=0; i< MeasurementVectorSize; i++ )
      {
      value[i] = i + it.GetIndex()[0];
      }
    it.Set( value );
    ++it;
    }

  MaskImageType::Pointer maskImage = MaskImageType::New();
  maskImage->SetRegions( region );
  maskImage->Allocate();
  maskImage->FillBuffer(0);
  MaskImageType::IndexType startMask;
  MaskImageType::SizeType sizeMask;

  startMask[0] = 2;
  startMask[1] = 3;
  startMask[2] = 5;

  sizeMask[0] = 7;
  sizeMask[1] = 3;
  sizeMask[2] = 4;

  MaskImageType::RegionType regionMask( startMask, sizeMask);
  typedef itk::ImageRegionIteratorWithIndex< MaskImageType > MaskIteratorType;
  MaskIteratorType mit( maskImage, regionMask );
  mit.GoToBegin();
  while( !mit.IsAtEnd() )
    {
    mit.Set((unsigned char)255);
    ++mit;
    }

  // Generate a list sample from "image" confined to the mask, "maskImage".
  typedef itk::Statistics::ImageToListSampleFilter<
    ImageType, MaskImageType > ImageToListSampleFilterType;
  ImageToListSampleFilterType::Pointer filter
                              = ImageToListSampleFilterType::New();

  filter->SetInput( image );
  filter->SetMaskImage( maskImage );
  filter->SetMaskValue( 255 );

  try
    {
    filter->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }

  typedef ImageToListSampleFilterType::ListSampleType ListSampleType;
  const ListSampleType * list = filter->GetOutput();

  // Check the sum of the pixels in the list sample. This should
  // be 420
  ListSampleType::ConstIterator lit = list->Begin();
  unsigned int sum = 0;

  while (lit != list->End())
    {
    sum += lit.GetMeasurementVector()[0];
    ++lit;
    }

  if (sum != 420)
    {
    std::cerr << "Computed sum of pixels in the list sample (masked) is : "
              << sum
              << " but should be 420.";
    return EXIT_FAILURE;
    }

  std::cerr << "[PASSED]" << std::endl;
  return EXIT_SUCCESS;
}
