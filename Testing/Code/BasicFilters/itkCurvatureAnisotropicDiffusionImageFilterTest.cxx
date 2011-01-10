/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkCurvatureAnisotropicDiffusionImageFilterTest.cxx
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
#include "itkCurvatureAnisotropicDiffusionImageFilter.h"
#include "itkNullImageToImageFilterDriver.txx"
#include "itkFilterWatcher.h"

/**
 * This program tests the FilterImageAnisotropicDiffusion object by driving it
 * with a null input and output.  Returns 0 on success and 1 on failure.
 */
int itkCurvatureAnisotropicDiffusionImageFilterTest(int itkNotUsed(argc), char *itkNotUsed(argv) [] )
{
  try
    {
      typedef itk::Image<float, 2> ImageType;
      
      // Set up filter
      itk::CurvatureAnisotropicDiffusionImageFilter<ImageType, ImageType>::Pointer
        filter = itk::CurvatureAnisotropicDiffusionImageFilter<ImageType,
        ImageType>::New();
      FilterWatcher watcher(filter);

      filter->SetNumberOfIterations(1);
      filter->SetConductanceParameter(3.0f);
      filter->SetTimeStep(0.125f);

      // Run Test
      itk::Size<2> sz;
      sz[0] = 250;
      sz[1] = 250;
      itk::NullImageToImageFilterDriver< ImageType, ImageType > test1;
      test1.SetImageSize(sz);
      test1.SetFilter(filter.GetPointer());
      test1.Execute();
    }
  catch(itk::ExceptionObject &err)
    {
      (&err)->Print(std::cerr);
      return EXIT_FAILURE;
    } 
  return EXIT_SUCCESS;   
}
