/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkIsolatedConnectedImageFilterTest.cxx
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

#include <fstream>
#include "itkIsolatedConnectedImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImageRegionIterator.h"
#include "itkNumericTraits.h"
#include "itkFilterWatcher.h"

int itkIsolatedConnectedImageFilterTest(int ac, char* av[] )
{
  if(ac < 8)
    {
    std::cerr << "Usage: " << av[0] << " InputImage OutputImage FindUpper(true,false) seed1_x seed1_y seed2_x seed2_y [seed1_x2 seed1_y2 seed2_x2 seed2_y2]*\n";
    return -1;
    }

  typedef unsigned char PixelType;
  typedef itk::Image<PixelType, 2> myImage;
  itk::ImageFileReader<myImage>::Pointer input 
    = itk::ImageFileReader<myImage>::New();
  input->SetFileName(av[1]);
  
  // Create a filter
  typedef itk::IsolatedConnectedImageFilter<myImage,myImage> FilterType;

  FilterType::Pointer filter = FilterType::New();
  FilterWatcher watcher(filter);

  filter->SetInput(input->GetOutput());
  
  FilterType::IndexType seed1;
  
  seed1[0] = atoi(av[4]); seed1[1] = atoi(av[5]);
  filter->SetSeed1(seed1); // deprecated method
  
  seed1[0] = atoi(av[6]); seed1[1] = atoi(av[7]);
  filter->SetSeed2(seed1); // deprecated method

  // Add additional seeds
  for (int i=8; i<ac; i+=4)
    {
    seed1[0] = atoi(av[i]); seed1[1] = atoi(av[i+1]);
    filter->AddSeed1(seed1);
  
    seed1[0] = atoi(av[i+2]); seed1[1] = atoi(av[i+3]);
    filter->AddSeed2(seed1);
    }

  // The min and max values for a .png image
  filter->SetLower(0);
  filter->SetUpperValueLimit(255); //deprecated method
  filter->SetUpper(255);
  filter->SetReplaceValue(255);
    
  // Test SetMacro
  filter->SetIsolatedValueTolerance(1);
  
  // Test SetMacro
  std::string findUpper = av[3];
  if (findUpper == "true")
    { filter->FindUpperThresholdOn(); }
  else
    { filter->FindUpperThresholdOff(); }    

  // Test GetMacros
  PixelType lower = filter->GetLower();
  std::cout << "filter->GetLower(): "
            << static_cast<itk::NumericTraits<PixelType>::PrintType>(lower)
            << std::endl;
  PixelType isolatedValueTolerance = filter->GetIsolatedValueTolerance();
  std::cout << "filter->GetIsolatedValueTolerance(): " 
            << static_cast<itk::NumericTraits<PixelType>::PrintType>(isolatedValueTolerance)
            << std::endl;
  PixelType upperValueLimit = filter->GetUpperValueLimit();
  std::cout << "filter->GetUpperValueLimit(): "
            << static_cast<itk::NumericTraits<PixelType>::PrintType>(upperValueLimit)
            << std::endl;
  PixelType upper = filter->GetUpper();
  std::cout << "filter->GetUpper(): "
            << static_cast<itk::NumericTraits<PixelType>::PrintType>(upper)
            << std::endl;
  PixelType replaceValue = filter->GetReplaceValue();
  std::cout << "filter->GetReplaceValue(): "
            << static_cast<itk::NumericTraits<PixelType>::PrintType>(replaceValue)
            << std::endl;
  bool findUpperThreshold = filter->GetFindUpperThreshold();
  std::cout << "filter->GetFindUpperThreshold(): "
            << findUpperThreshold
            << std::endl;

  

  try
    {
    input->Update();
    filter->Update();
    }
  catch (itk::ExceptionObject& e)
    {
    std::cerr << "Exception detected: "  << e.GetDescription();
    return -1;
    }

  bool thresholdingFailed = filter->GetThresholdingFailed();

  if (thresholdingFailed)
    {
    std::cout << "Selection of isolating threshold failed" << std::endl;
    }
  else
    {
    std::cout << "Selection of isolating threshold succeeded" << std::endl;
    }
    
  // Generate test image
  itk::ImageFileWriter<myImage>::Pointer writer;
  writer = itk::ImageFileWriter<myImage>::New();
  writer->SetInput( filter->GetOutput() );
  writer->SetFileName( av[2] );
  writer->Update();


  // Now flip the mode to test whether it fails 
  if (findUpper == "true")
    { filter->FindUpperThresholdOff(); }
  else
    { filter->FindUpperThresholdOn(); }    
  
  try
    {
    filter->Update();
    }
  catch (itk::ExceptionObject& e)
    {
    std::cerr << "Exception detected: "  << e.GetDescription();
    return -1;
    }

  thresholdingFailed = filter->GetThresholdingFailed();

  if (thresholdingFailed)
    {
    std::cout << "When mode flipped: Selection of isolating threshold failed" << std::endl;
    }
  else
    {
    std::cout << "When mode flipped: Selection of isolating threshold succeeded" << std::endl;
    }


  return EXIT_SUCCESS;
}
