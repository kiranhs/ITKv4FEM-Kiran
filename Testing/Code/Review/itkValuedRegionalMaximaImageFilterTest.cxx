/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkValuedRegionalMaximaImageFilterTest.cxx
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
#include "itkValuedRegionalMaximaImageFilter.h"
#include "itkHConvexImageFilter.h"
#include "itkMaximumImageFilter.h"
#include "itkInvertIntensityImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCommand.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkAndImageFilter.h"
#include "itkCommand.h"
#include "itkSimpleFilterWatcher.h"

int itkValuedRegionalMaximaImageFilterTest(int argc, char * argv[])
{
  const int dim = 2;
  
  if( argc < 5 )
    {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " InputImage  OutputImageFile1 OutputImageFile2  " 
              << "OutputImageFile3" << std::endl;
    return EXIT_FAILURE;
    }
 
  typedef unsigned char                PixelType;
  typedef itk::Image< PixelType, dim > ImageType;

  typedef itk::ImageFileReader< ImageType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[2] );

  typedef itk::ValuedRegionalMaximaImageFilter< ImageType, ImageType >
                                                               FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput( reader->GetOutput() );
  filter->SetFullyConnected( atoi(argv[1]) );
  itk::SimpleFilterWatcher watcher(filter, "filter");

  typedef itk::ImageFileWriter< ImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( filter->GetOutput() );
  writer->SetFileName( argv[3] );
  writer->Update();


  // produce the same output with other filters
  typedef itk::HConvexImageFilter< ImageType, ImageType > ConvexType;
  ConvexType::Pointer convex = ConvexType::New();
  convex->SetInput( reader->GetOutput() );
  convex->SetFullyConnected( atoi(argv[1]) );
  convex->SetHeight( 1 );

  // convex gives maxima with value=1 and others with value=0
  // rescale the image so we have maxima=255 other=0
  typedef itk::RescaleIntensityImageFilter< ImageType, ImageType > RescaleType;
  RescaleType::Pointer rescale = RescaleType::New();
  rescale->SetInput( convex->GetOutput() );
  rescale->SetOutputMaximum( 255 );
  rescale->SetOutputMinimum( 0 );

  // in the input image, select the values of the pixel at the minima
  typedef itk::AndImageFilter< ImageType, ImageType, ImageType > AndType;
  AndType::Pointer a = AndType::New();
  a->SetInput(0, rescale->GetOutput() );
  a->SetInput(1, reader->GetOutput() );

  WriterType::Pointer writer2 = WriterType::New();
  writer2->SetInput( a->GetOutput() );
  writer2->SetFileName( argv[4] );
  writer2->Update();

  return EXIT_SUCCESS;
}
