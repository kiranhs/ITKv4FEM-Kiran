/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkBinaryThresholdImageFunctionTest.cxx
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

#include <iostream>

#include "itkImage.h"
#include "itkImageRegionIterator.h"

#include "itkBinaryThresholdImageFunction.h"

int itkBinaryThresholdImageFunctionTest(int, char* [] )
{
  std::cout << "itkBinaryThresholdImageFunctionTest Start" << std::endl;

  typedef itk::Image<float,3> FloatImage;

  FloatImage::Pointer    image  = FloatImage::New();
  FloatImage::RegionType region;
  FloatImage::SizeType   size; size.Fill(64);
  FloatImage::IndexType  index; index.Fill(0);

  region.SetIndex (index);
  region.SetSize (size);

  image->SetRegions( region );
  image->Allocate();
  image->FillBuffer(0.0);


  for (unsigned int i = 0; i < FloatImage::ImageDimension; i++)
    {
    size[i] -= 20;
    index[i] += 10;
    }
  region.SetIndex (index);
  region.SetSize (size);

  itk::ImageRegionIterator<FloatImage> it(image,region);
  while (!it.IsAtEnd())
    {
    it.Set(100.0);
    ++it;
    }
  
  // Try the function inside and outside the region
  typedef itk::BinaryThresholdImageFunction<FloatImage> ImageFunction;
  ImageFunction::Pointer threshold = ImageFunction::New();

  ImageFunction::PointType  point;
  ImageFunction::ContinuousIndexType continuousIndex;

  threshold->SetInputImage(image);

  threshold->ThresholdBelow (100.0);
  index[0] = 11; index[1] = 11; index[2] = 11;

  int failed = 0;
  if (!threshold->EvaluateAtIndex(index)) failed++;

  threshold->ThresholdAbove (100.0);
  if (!threshold->EvaluateAtIndex(index)) failed++;

  threshold->ThresholdAbove (101.0);
  if (threshold->EvaluateAtIndex(index)) failed++;

  threshold->ThresholdBetween (100.0,100.0);
  if (!threshold->EvaluateAtIndex(index)) failed++;

  threshold->ThresholdBetween (-100.0,0.0);
  if (threshold->EvaluateAtIndex(index)) failed++;

  index[0] = 8; index[1] = 8; index[2] = 8;
  threshold->ThresholdBetween (100.0,200.0);
  if (threshold->EvaluateAtIndex(index)) failed++;

  std::cout << threshold;

  // Test Evaluate
  point[0] = 5; point[1] = 5; point[2] = 5;
  threshold->Evaluate(point);

  // Test EvaluateAtContinuousIndex
  continuousIndex[0] = 9; 
  continuousIndex[1] = 9; 
  continuousIndex[2] = 9;
  threshold->EvaluateAtContinuousIndex(continuousIndex);
  

  if (failed)
    {
    std::cout << "Failed!" << std::endl;
    return EXIT_FAILURE;
    }
  else
    {
    std::cout << "Passed!" << std::endl;
    }
  return EXIT_SUCCESS;
}
