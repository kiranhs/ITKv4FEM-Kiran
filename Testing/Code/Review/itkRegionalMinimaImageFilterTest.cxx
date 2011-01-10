/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkRegionalMinimaImageFilterTest.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

// a test routine for regional extrema using flooding
#include "itkRegionalMinimaImageFilter.h"
#include "itkHConcaveImageFilter.h"
#include "itkMaximumImageFilter.h"
#include "itkInvertIntensityImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCommand.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkAndImageFilter.h"
#include "itkCommand.h"
#include "itkSimpleFilterWatcher.h"


int itkRegionalMinimaImageFilterTest(int argc, char * argv[])
{
  const int dim = 3;
   
  if( argc < 5 )
    {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " Connection InputImage  OutputImageFile  " 
              << "OutputImageFile2" << std::endl;
    return EXIT_FAILURE;
    }
  
  typedef unsigned char                PixelType;
  typedef itk::Image< PixelType, dim > ImageType;

  typedef itk::ImageFileReader< ImageType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[2] );

  typedef itk::RegionalMinimaImageFilter< ImageType, ImageType > FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput( reader->GetOutput() );

  // exercise Set/Get Fully connected methods for 
  // testing purpose
  filter->SetFullyConnected ( true ); 

  if( filter->GetFullyConnected() != true )
    {
    std::cerr << "Set/Get FullyConnected error" << std::endl; 
    return EXIT_FAILURE;
    }

  filter->FullyConnectedOn(); 
  if( filter->GetFullyConnected() != true )
    {
    std::cerr << "Set/Get FullyConnected error" << std::endl; 
    return EXIT_FAILURE;
    }

  filter->FullyConnectedOff(); 
  if( filter->GetFullyConnected() != false )
    {
    std::cerr << "Set/Get FullyConnected error" << std::endl; 
    return EXIT_FAILURE;
    }

  // exercise Set/Get FlatIsMinima flag for 
  // testing purpose
  filter->SetFlatIsMinima ( true ); 

  if( filter->GetFlatIsMinima() != true )
    {
    std::cerr << "Set/Get FlatIsMinima error" << std::endl; 
    return EXIT_FAILURE;
    }

  filter->FlatIsMinimaOn(); 
  if( filter->GetFlatIsMinima() != true )
    {
    std::cerr << "Set/Get FlatIsMinima error" << std::endl; 
    return EXIT_FAILURE;
    }

  filter->FlatIsMinimaOff(); 
  if( filter->GetFlatIsMinima() != false )
    {
    std::cerr << "Set/Get FlatIsMinima error" << std::endl; 
    return EXIT_FAILURE;
    }


  filter->SetForegroundValue( 255 ); 
  if( filter->GetForegroundValue() != 255 )
    {
    std::cerr << "Set/Get ForegroundValue error" << std::endl; 
    return EXIT_FAILURE;
    }

  filter->SetBackgroundValue( 0 ); 
  if( filter->GetBackgroundValue() != 0 )
    {
    std::cerr << "Set/Get BackgroundValue error" << std::endl; 
    return EXIT_FAILURE;
    }

  filter->SetForegroundValue( itk::NumericTraits< PixelType>::max() );  
  filter->SetBackgroundValue( itk::NumericTraits< PixelType>::NonpositiveMin());

  filter->SetFullyConnected( atoi(argv[1]) );
  filter->FlatIsMinimaOff();
  itk::SimpleFilterWatcher watcher(filter, "filter");

  typedef itk::ImageFileWriter< ImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( filter->GetOutput() );
  writer->SetFileName( argv[3] );
  writer->Update();


  // produce the same output with other filters
  typedef itk::HConcaveImageFilter< ImageType, ImageType > ConcaveType;
  ConcaveType::Pointer concave = ConcaveType::New();
  concave->SetInput( reader->GetOutput() );
  concave->SetFullyConnected( atoi(argv[1]) );
  concave->SetHeight( 1 );

  // concave gives maxima with value=1 and others with value=0
  // rescale the image so we have maxima=255 other=0
  typedef itk::RescaleIntensityImageFilter< ImageType, ImageType > RescaleType;
  RescaleType::Pointer rescale = RescaleType::New();
  rescale->SetInput( concave->GetOutput() );
  rescale->SetOutputMaximum( 255 );
  rescale->SetOutputMinimum( 0 );

  WriterType::Pointer writer2 = WriterType::New();
  writer2->SetInput( rescale->GetOutput() );
  writer2->SetFileName( argv[4] );
  writer2->Update();

  return EXIT_SUCCESS; 
}
