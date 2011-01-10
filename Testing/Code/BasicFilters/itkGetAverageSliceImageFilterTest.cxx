/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkGetAverageSliceImageFilterTest.cxx
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

#include "itkGetAverageSliceImageFilter.h"
#include "itkRGBPixel.h"
#include "itkImageSeriesReader.h"
#include "itkImageSeriesWriter.h"
#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"
#include "itksys/SystemTools.hxx"

int itkGetAverageSliceImageFilterTest(int argc, char *argv[] )
{
  typedef short PixelType;
  static const int ImageDimension = 3;

  typedef itk::Image<PixelType,ImageDimension> InputImageType;
  typedef itk::Image<PixelType,ImageDimension> OutputImageType;
  typedef itk::Image<unsigned char,ImageDimension> WriteImageType;
  typedef itk::ImageSeriesReader< InputImageType > ReaderType ;
  typedef itk::GetAverageSliceImageFilter<InputImageType,OutputImageType> GetAveragerType;
  typedef itk::ImageSeriesWriter<OutputImageType,WriteImageType> WriterType; 
  typedef itk::GDCMSeriesFileNames                SeriesFileNames;
  typedef itk::GDCMImageIO                        ImageIOType;

  if (argc < 3)
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << "  inputDICOMDirectory outputFile" << std::endl;
    return EXIT_FAILURE;
    }

  // Get the input filenames
  SeriesFileNames::Pointer names = SeriesFileNames::New();

  // Get the DICOM filenames from the directory
  names->SetInputDirectory( argv[1] );

  // Create the reader
  ImageIOType::Pointer gdcmIO = ImageIOType::New();
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetImageIO( gdcmIO );
  try
    {
    reader->SetFileNames( names->GetInputFileNames() );
    reader->Update();
    }
  catch (itk::ExceptionObject &excp)
    {
    std::cerr << "Error reading the series" << std::endl;
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }

  // GetAverage the input images
  GetAveragerType::Pointer average = GetAveragerType::New();
  average->SetInput( reader->GetOutput() );
  average->SetAveragedOutDimension( 2 );

  try
    {
    average->Update();
    }
  catch ( itk::ExceptionObject &excp)
    {
    std::cerr << "Error running the average filter" << std::endl;
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }
  average->GetOutput()->Print(std::cout);

  average->Print( std::cout );

  try
    {
    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName ( argv[2] );
    
    writer->SetInput(average->GetOutput());
    writer->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << "Error writing the series" << std::endl;
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;

    }
  std::cout << "Test passed." << std::endl;
  return EXIT_SUCCESS;

}
