/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    FilteringExamples7.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
// this file defines the FilterExamples for the test driver
// and all it expects is that you have a function called RegisterTests
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif
#include <iostream>
#include "itkTestMain.h" 


void RegisterTests()
{
  REGISTER_TEST(DigitallyReconstructedRadiograph1Test);
  REGISTER_TEST(FFTImageFilterTest);
  REGISTER_TEST(RGBToGrayscaleTest);
  REGISTER_TEST(LaplacianRecursiveGaussianImageFilter1Test);
  REGISTER_TEST(LaplacianRecursiveGaussianImageFilter2Test);
}


#undef main
#define main DigitallyReconstructedRadiograph1Test
#include "DigitallyReconstructedRadiograph1.cxx"

#undef main
#define main FFTImageFilterTest
#include "FFTImageFilter.cxx"

#undef main
#define main RGBToGrayscaleTest
#include "RGBToGrayscale.cxx"

#undef main
#define main LaplacianRecursiveGaussianImageFilter1Test
#include "LaplacianRecursiveGaussianImageFilter1.cxx"

#undef main
#define main LaplacianRecursiveGaussianImageFilter2Test
#include "LaplacianRecursiveGaussianImageFilter2.cxx"
