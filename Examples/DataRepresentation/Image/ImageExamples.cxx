/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ImageExamples.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
// this file defines the ImageExamples for the test driver
#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif
#include <iostream>
#include "itkTestMain.h" 


void RegisterTests()
{
  REGISTER_TEST(Image1Test);
  REGISTER_TEST(Image2Test);
  REGISTER_TEST(Image3Test);
  REGISTER_TEST(Image4Test);
  REGISTER_TEST(Image5Test);
}

#undef main
#define main Image1Test
#include "Image1.cxx"

#undef main
#define main Image2Test
#include "Image2.cxx"

#undef main
#define main Image3Test
#include "Image3.cxx"

#undef main
#define main Image4Test
#include "Image4.cxx"

#undef main
#define main Image5Test
#include "Image5.cxx"
