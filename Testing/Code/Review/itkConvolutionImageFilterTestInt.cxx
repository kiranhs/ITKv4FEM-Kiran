/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkConvolutionImageFilterTestInt.cxx
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

#include "itkConvolutionImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkSimpleFilterWatcher.h"

int itkConvolutionImageFilterTestInt(int argc, char * argv[])
{
  
  if ( argc < 4 )
    {
    std::cout << "Usage: " << argv[0]
      << " inputImage kernelImage outputImage [normalizeImage]" << std::endl;
    return EXIT_FAILURE;
    }

  const int ImageDimension = 2;
  
  typedef unsigned char                          PixelType;
  typedef itk::Image<PixelType, ImageDimension>  ImageType;
  typedef itk::ImageFileReader<ImageType>        ReaderType;

  ReaderType::Pointer reader1 = ReaderType::New();
  reader1->SetFileName( argv[1] );
  reader1->Update();

  ReaderType::Pointer reader2 = ReaderType::New();
  reader2->SetFileName( argv[2] );
  reader2->Update();

  typedef itk::ConvolutionImageFilter<ImageType> ConvolutionFilterType;
  ConvolutionFilterType::Pointer convoluter
    = ConvolutionFilterType::New();
  convoluter->SetInput( reader1->GetOutput() );
  convoluter->SetImageKernelInput( reader2->GetOutput() );

  itk::SimpleFilterWatcher watcher(convoluter, "filter");

  if( argc >= 5 )
    {
    convoluter->SetNormalize( static_cast<bool>( atoi( argv[4] ) ) );
    }

  typedef itk::ImageFileWriter<ImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( argv[3] );
  writer->SetInput( convoluter->GetOutput() );

  try
    {
    writer->Update();
    }
  catch ( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
