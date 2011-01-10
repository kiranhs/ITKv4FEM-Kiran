/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkGEImageIOTest.cxx
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
#include <string>
#include "itkGEAdwImageIO.h"
#include "itkGE4ImageIO.h"
#include "itkGE5ImageIO.h"
#include "itkSiemensVisionImageIO.h"
#include "itkGEAdwImageIOFactory.h"
#include "itkGE4ImageIOFactory.h"
#include "itkGE5ImageIOFactory.h"
#include "itkSiemensVisionImageIOFactory.h"
#include "itkImageIOFactory.h"
#include "itkExceptionObject.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImage.h"
#include <itksys/SystemTools.hxx>

typedef itk::Image<signed short, 3> ImageType ;
typedef ImageType::Pointer ImagePointer ;
typedef itk::ImageFileReader< ImageType > ImageReaderType ;
typedef itk::ImageFileWriter< ImageType > ImageWriterType ;

int itkGEImageIOFactoryTest(int ac, char * av[])
{
  static bool firstTime = true;
  if(firstTime) 
    {
    itk::ObjectFactoryBase::RegisterFactory(itk::GEAdwImageIOFactory::New() );
    itk::ObjectFactoryBase::RegisterFactory(itk::GE4ImageIOFactory::New() );
    itk::ObjectFactoryBase::RegisterFactory(itk::GE5ImageIOFactory::New() );
    itk::ObjectFactoryBase::RegisterFactory(itk::SiemensVisionImageIOFactory::New() );
    firstTime = false;
    }
  if(ac < 2)
    {
    return EXIT_FAILURE;
    }
  char *filename = *++av;

  ImagePointer input ;
  ImageReaderType::Pointer imageReader = ImageReaderType::New() ;

  try
    {
    imageReader->SetFileName(filename);
    imageReader->Update() ;
    input = imageReader->GetOutput() ;
    }
  catch (itk::ExceptionObject &e)
    {
    std::cout << "Caught unexpected exception. Test Failed!" << std::endl;
    std::cout << e << std::endl;
    return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;
}

int itkGEImageIOTest(int ac, char * av[])
{
  //
  // first argument is passing in the writable directory to do all testing
  if(ac > 1) {
    char *testdir = *++av;
    --ac;
    itksys::SystemTools::ChangeDirectory(testdir);
  }

  if((ac != 5) && (ac != 4))
    {
    return EXIT_FAILURE;
    }
  std::string failmode(av[1]);
  std::string filetype(av[2]);
  std::string filename(av[3]);
  bool Failmode = failmode == std::string("true");
  itk::ImageIOBase::Pointer io;
  if(filetype == "GE4")
    {
      io = itk::GE4ImageIO::New();
    }
  else if(filetype == "GE5")
    {
      io = itk::GE5ImageIO::New();
    }
  else if(filetype == "GEAdw")
    {
      io = itk::GEAdwImageIO::New();
    }
  else if(filetype == "Siemens")
    {
      io = itk::SiemensVisionImageIO::New();
    }
  else
    {
      return EXIT_FAILURE;
    }

  ImagePointer input ;
  ImageReaderType::Pointer imageReader = ImageReaderType::New() ;

  try
    {
      imageReader->SetImageIO(io);
      imageReader->SetFileName(filename.c_str()) ;
      imageReader->Update() ;
      input = imageReader->GetOutput() ;
    }
  catch (itk::ExceptionObject &e)
    {
    if (Failmode)
      {
      std::cout << "Caught unexpected exception. Test Failed!" << std::endl;
      }
    else
      {
      std::cout << "Caught expected exception. Test Passed!" << std::endl;
      return EXIT_SUCCESS;
      }
    std::cout << e << std::endl;
    return Failmode ? 1 : 0;
    }

  if (failmode == std::string("true"))
    {
    ImageWriterType::Pointer writer = ImageWriterType::New();
    writer->SetInput( input );
    writer->SetFileName( av[4] );
    writer->Update();
    }

  return Failmode ? 0 : 1;
}
