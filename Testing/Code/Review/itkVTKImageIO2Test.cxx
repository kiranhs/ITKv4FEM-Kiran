/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkVTKImageIO2Test.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkVTKImageIO2.h"

#include "itkImageFileWriter.h"
#include "itkImageFileReader.h"
#include "itkRGBPixel.h"
#include "itkRGBAPixel.h"
#include "itkImageRegionIterator.h"
#include "itkTestingMacros.h"

static unsigned int m_CallNumber;

const unsigned int TEST_VECTOR_PIXEL_DIM = 3;

template <typename TPixelType, unsigned int VImageDimension>
class VTKImageIO2Tester
{
public:
  
  virtual int Test(int argc, char* argv[] );
  
  virtual ~VTKImageIO2Tester(){};
  
  static std::string 
  SetupFileName( const std::string &filePrefix,
                 const std::string &fileExtension,
                 std::string &outputPath )
    {
    std::ostringstream m_NameWithIndex;
    m_NameWithIndex << filePrefix << "_" << m_CallNumber << "." << fileExtension;
    
    std::ostringstream m_OutputFileName;
  
#if defined(WIN32) // windows
    // if it ends in \\ just append the name
    if (outputPath[outputPath.size()-1] == '\\')
      {
      m_OutputFileName << outputPath << m_NameWithIndex.str();
      }
    else
      {
      m_OutputFileName << outputPath << "\\" << m_NameWithIndex.str();
      }
  
#else /// POSIX UNIX
    // if it ends in / just append the name
    if (outputPath[outputPath.size()-1] == '/')
      {
      m_OutputFileName << outputPath << m_NameWithIndex.str();
      }
    // otherwise, add / and the name
    else
      {
      m_OutputFileName << outputPath << "/" << m_NameWithIndex.str();
      }
#endif
  
    return m_OutputFileName.str();
    }

  static bool
  Write( const std::string &filePrefix,
         std::string &outputPath,
         bool ascii )
    {
    try 
      {
      ++m_CallNumber;

      typedef TPixelType                              PixelType;
      typedef itk::Image<PixelType,VImageDimension>   ImageType;
    
    
      // force use of VTKImageIO2
      typedef itk::VTKImageIO2 IOType;
      IOType::Pointer vtkIO = IOType::New();
      vtkIO->SetFileTypeToASCII();
      if (ascii)
        {
        vtkIO->SetFileTypeToASCII();
        }
      else
        {
        vtkIO->SetFileTypeToBinary();
        }
  
      typedef itk::ImageFileWriter<ImageType> ImageFileWriterType;
      typename ImageFileWriterType::Pointer writer = ImageFileWriterType::New();
      writer->SetImageIO( vtkIO );
    
      // allocate an 10x10x10 image
      typename ImageType::Pointer image = ImageType::New();
      typename ImageType::SizeType imageSize;
      imageSize.Fill(10);
      image->SetRegions( imageSize );
      image->Allocate();

      unsigned int cnt = 0;
      itk::ImageRegionIterator< ImageType > i( image, image->GetLargestPossibleRegion() );
      i.GoToBegin();
      while (! i.IsAtEnd() )
        {
        // fill the image switching between these pixels
        switch (cnt%4) 
          {
          case 0:
            i.Set( itk::NumericTraits<PixelType>::ZeroValue() );
            break;
          case 1:
            i.Set( itk::NumericTraits<PixelType>::OneValue() );
            break;
          case 2:
            i.Set( itk::NumericTraits<PixelType>::OneValue() );
            break;
          case 3:
            i.Set( itk::NumericTraits<PixelType>::ZeroValue() );
          }
        ++cnt;
        ++i;
        }
    
      writer->SetInput( image );
    
      std::string m_OutputFileName = 
        VTKImageIO2Tester<char,3>::SetupFileName(filePrefix, "vtk", outputPath);

      writer->SetFileName( m_OutputFileName );
      writer->Update();
    
      // test the CanWriteFile function after the fact (should always
      // be true at this point)
      if (!vtkIO->CanWriteFile(m_OutputFileName.c_str()))
        {
        return false;
        }
    
      return true;
      
      } 
    catch (itk::ExceptionObject &e) 
      {
      std::cout << e << std::endl;
      return false;
      }
    }

  static bool
  Read( const std::string &filePrefix,
        std::string &outputPath,
        bool ascii )
    {
    try
      {
      typedef TPixelType                              PixelType;
      typedef itk::Image<PixelType,VImageDimension>   ImageType;
    
      typedef itk::ImageFileReader<ImageType> ImageFileReaderType;
      typename ImageFileReaderType::Pointer reader = ImageFileReaderType::New();
    
      // force use of VTKImageIO2
      typedef itk::VTKImageIO2 IOType;
      IOType::Pointer vtkIO = IOType::New();
      reader->SetImageIO(vtkIO);
    
      // set ascii or binary
      if (ascii)
        {
        vtkIO->SetFileTypeToASCII();
        }
      else
        {
        vtkIO->SetFileTypeToBinary();
        }
    
      std::string m_OutputFileName = VTKImageIO2Tester::SetupFileName( filePrefix, "vtk", outputPath );
      reader->SetFileName( m_OutputFileName );
    
      // read the image
      typename ImageType::Pointer image = reader->GetOutput();
      reader->Update();
    
      // test the CanReadFile function after the fact (should always
      // be true at this point)
      if (!vtkIO->CanReadFile(m_OutputFileName.c_str()))
        {
        return false;
        }
    
      // check the size
      typename ImageType::RegionType region =
        image->GetLargestPossibleRegion();
      typename ImageType::SizeType size = region.GetSize();
      bool sizeGood = true;
      for (unsigned int i = 0; i < ImageType::GetImageDimension(); i++)
        {
        if (size[i] != 10)
          {
          sizeGood = false;
          break;
          }
        }
      if (!sizeGood)
        {
        std::cout << "Error: Size didn't read properly" << std::endl;
        return false;
        }
    
      // check each pixel
      bool pixelsGood = true;
      unsigned int cnt = 0;
      itk::ImageRegionIterator< ImageType > iter( image, region );
      iter.GoToBegin();
    
      while (! iter.IsAtEnd() && pixelsGood)
        {
        // check image switching between these pixels
        switch (cnt%4) 
          {
          case 0:
            if (iter.Get() != itk::NumericTraits<PixelType>::ZeroValue())
              {
              pixelsGood = false;
              }
            break;
          case 1:
            if (iter.Get() != itk::NumericTraits<PixelType>::OneValue())
              {
              pixelsGood = false;
              }
            break;
          case 2:
            if (iter.Get() != itk::NumericTraits<PixelType>::OneValue())
              {
              pixelsGood = false;
              }
            break;
          case 3:
            if (iter.Get() != itk::NumericTraits<PixelType>::ZeroValue())
              {
              pixelsGood = false;
              }
            break;
          }
        ++cnt;
        ++iter;
        }
    
      if (!pixelsGood)
        {
        std::cout << "Error: Pixels didn't read properly" << std::endl;
        return false;
        }

      // reading successfull, so return true
      return true;
      }
    catch(itk::ExceptionObject &e)
      {
      std::cout << e << std::endl;
      return false;
      }
    }

  static bool
  CanReadFileTest( const std::string &filePrefix,
                   const std::string &fileExtension,
                   std::string &outputPath )
    {
    typedef itk::VTKImageIO2 IOType;
    IOType::Pointer vtkIO = IOType::New();
  
    std::string fileName =
      VTKImageIO2Tester::SetupFileName(filePrefix, fileExtension, outputPath);
  
    return vtkIO->CanReadFile( fileName.c_str() );
    }

  static bool
  CanWriteFileTest( const std::string &filePrefix,
                    const std::string &fileExtension,
                    std::string &outputPath )
    {
    typedef itk::VTKImageIO2 IOType;
    IOType::Pointer vtkIO = IOType::New();
  
    std::string fileName =
      VTKImageIO2Tester::SetupFileName(filePrefix, fileExtension, outputPath);
  
    return vtkIO->CanWriteFile( fileName.c_str() );
    }
};


int itkVTKImageIO2Test(int argc, char* argv[])
{
  
  if( argc < 2 )
    {
    std::cerr << "Usage: " << argv[0] << " outputPath" << std::endl;
    return EXIT_FAILURE;
    }
  
  std::string outputPath = argv[1];
  const std::string filePrefix = argv[0];
  
  //
  // test all usable pixel types
  //
  
  // unsigned char (ascii)
  if (!(VTKImageIO2Tester<unsigned char, 3>::Write( filePrefix, outputPath, true )))
    {
    std::cout << "[FAILED] writing (unsigned char - ascii)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] writing (unsigned char - ascii)" << std::endl;
  if (!(VTKImageIO2Tester<unsigned char, 3>::Read( filePrefix, outputPath, true )))
    {
    std::cout << "[FAILED] reading (unsigned char - ascii)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] reading (unsigned char - ascii)" << std::endl;
  
  // unsigned char (binary)
  if (!(VTKImageIO2Tester<unsigned char, 3>::Write( filePrefix, outputPath, false )))
    {
    std::cout << "[FAILED] writing (unsigned char - binary)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] writing (unsigned char - binary)" << std::endl;
  if (!(VTKImageIO2Tester<unsigned char, 3>::Read( filePrefix, outputPath, false )))
    {
    std::cout << "[FAILED] reading (unsigned char - binary)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] reading (unsigned char - binary)" << std::endl;
  
  // char (ascii)
  if (!(VTKImageIO2Tester<char, 3>::Write( filePrefix, outputPath, true )))
    {
    std::cout << "[FAILED] writing (char - ascii)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] writing (char - ascii)" << std::endl;
  if (!(VTKImageIO2Tester<char, 3>::Read( filePrefix, outputPath, true )))
    {
    std::cout << "[FAILED] reading (char - ascii)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] reading (char - ascii)" << std::endl;
  
  // char (binary)
  if (!(VTKImageIO2Tester<char, 3>::Write( filePrefix, outputPath, false )))
    {
    std::cout << "[FAILED] writing (char - binary)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] writing (char - binary)" << std::endl;
  if (!(VTKImageIO2Tester<char, 3>::Read( filePrefix, outputPath, false )))
    {
    std::cout << "[FAILED] reading (char - binary)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] reading (char - binary)" << std::endl;
  
  // unsigned short (ascii)
  if (!(VTKImageIO2Tester<unsigned short, 3>::Write( filePrefix, outputPath, true )))
    {
    std::cout << "[FAILED] writing (unsigned short - ascii)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] writing (unsigned short - ascii)" << std::endl;
  if (!(VTKImageIO2Tester<unsigned short, 3>::Read( filePrefix, outputPath, true )))
    {
    std::cout << "[FAILED] reading (unsigned short - ascii)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] reading (unsigned short - ascii)" << std::endl;
  
  // unsigned short (binary)
  if (!(VTKImageIO2Tester<unsigned short, 3>::Write( filePrefix, outputPath, false )))
    {
    std::cout << "[FAILED] writing (unsigned short - binary)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] writing (unsigned short - binary)" << std::endl;
  if (!(VTKImageIO2Tester<unsigned short, 3>::Read( filePrefix, outputPath, false )))
    {
    std::cout << "[FAILED] reading (unsigned short - binary)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] reading (unsigned short - binary)" << std::endl;
  
  // short (ascii)
  if (!(VTKImageIO2Tester<short, 3>::Write( filePrefix, outputPath, true )))
    {
    std::cout << "[FAILED] writing (short - ascii)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] writing (short - ascii)" << std::endl;
  if (!(VTKImageIO2Tester<short, 3>::Read( filePrefix, outputPath, true )))
    {
    std::cout << "[FAILED] reading (short - ascii)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] reading (short - ascii)" << std::endl;
  
  // short (binary)
  if (!(VTKImageIO2Tester<short, 3>::Write( filePrefix, outputPath, false )))
    {
    std::cout << "[FAILED] writing (short - binary)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] writing (short - binary)" << std::endl;
  if (!(VTKImageIO2Tester<short, 3>::Read( filePrefix, outputPath, false )))
    {
    std::cout << "[FAILED] readting (short - binary)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] reading (short - binary)" << std::endl;
  
  // unsigned int (ascii)
  if (!(VTKImageIO2Tester<unsigned int, 3>::Write( filePrefix, outputPath, true )))
    {
    std::cout << "[FAILED] writing (unsigned int - ascii)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] writing (unsigned int - ascii)" << std::endl;
  if (!(VTKImageIO2Tester<unsigned int, 3>::Read( filePrefix, outputPath, true )))
    {
    std::cout << "[FAILED] reading (unsigned int - ascii)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] reading (unsigned int - ascii)" << std::endl;
  
  // unsigned int (binary)
  if (!(VTKImageIO2Tester<unsigned int, 3>::Write( filePrefix, outputPath, false )))
    {
    std::cout << "[FAILED] writing (unsigned int - binary)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] writing (unsigned int - binary)" << std::endl;
  if (!(VTKImageIO2Tester<unsigned int, 3>::Read( filePrefix, outputPath, false )))
    {
    std::cout << "[FAILED] reading (unsigned int - binary)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] reading (unsigned int - binary)" << std::endl;
  
  // int (ascii)
  if (!(VTKImageIO2Tester<int, 3>::Write( filePrefix, outputPath, true )))
    {
    std::cout << "[FAILED] writing (int - ascii)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] writing (int - ascii)" << std::endl;
  if (!(VTKImageIO2Tester<int, 3>::Read( filePrefix, outputPath, true )))
    {
    std::cout << "[FAILED] reading (int - ascii)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] reading (int - ascii)" << std::endl;
  
  // int (binary)
  if (!(VTKImageIO2Tester<int, 3>::Write( filePrefix, outputPath, false )))
    {
    std::cout << "[FAILED] writing (int - binary)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] writing (int - binary)" << std::endl;
  if (!(VTKImageIO2Tester<int, 3>::Read( filePrefix, outputPath, false )))
    {
    std::cout << "[FAILED] reading (int - binary)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] reading (int - binary)" << std::endl;
  
  // unsigned long (ascii)
  if (!(VTKImageIO2Tester<unsigned long, 3>::Write( filePrefix, outputPath, true )))
    {
    std::cout << "[FAILED] writing (unsigned long - ascii)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] writing (unsigned long - ascii)" << std::endl;
  if (!(VTKImageIO2Tester<unsigned long, 3>::Read( filePrefix, outputPath, true )))
    {
    std::cout << "[FAILED] reading (unsigned long - ascii)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] reading (unsigned long - ascii)" << std::endl;
  
  // unsigned long (binary)
  if (!(VTKImageIO2Tester<unsigned long, 3>::Write( filePrefix, outputPath, false )))
    {
    std::cout << "[FAILED] writing (unsigned long - binary)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] writing (unsigned long - binary)" << std::endl;
  if (!(VTKImageIO2Tester<unsigned long, 3>::Read( filePrefix, outputPath, false )))
    {
    std::cout << "[FAILED] reading (unsigned long - binary)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] reading (unsigned long - binary)" << std::endl;
  
  // long (ascii)
  if (!(VTKImageIO2Tester<long, 3>::Write( filePrefix, outputPath, true )))
    {
    std::cout << "[FAILED] writing (long - ascii)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] writing (long - ascii)" << std::endl;
  if (!(VTKImageIO2Tester<long, 3>::Read( filePrefix, outputPath, true )))
    {
    std::cout << "[FAILED] reading (long - ascii)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] reading (long - ascii)" << std::endl;
  
  // long (binary)
  if (!(VTKImageIO2Tester<long, 3>::Write( filePrefix, outputPath, false )))
    {
    std::cout << "[FAILED] writing (long - binary)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] writing (long - binary)" << std::endl;
  if (!(VTKImageIO2Tester<long, 3>::Read( filePrefix, outputPath, false )))
    {
    std::cout << "[FAILED] reading (long - binary)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] reading (long - binary)" << std::endl;
  
  // float - ascii
  if (!(VTKImageIO2Tester<float, 3>::Write( filePrefix, outputPath, true )))
    {
    std::cout << "[FAILED] writing (float - ascii)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] writing (float - ascii)" << std::endl;
  if (!(VTKImageIO2Tester<float, 3>::Read( filePrefix, outputPath, true )))
    {
    std::cout << "[FAILED] reading (float - ascii)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] reading (float - ascii)" << std::endl;
  
  // float - binary
  if (!(VTKImageIO2Tester<float, 3>::Write( filePrefix, outputPath, false )))
    {
    std::cout << "[FAILED] writing (float - binary)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] writing (float - binary)" << std::endl;
  if (!(VTKImageIO2Tester<float, 3>::Read( filePrefix, outputPath, false )))
    {
    std::cout << "[FAILED] reading (float - binary)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] reading (float - binary)" << std::endl;
  
  // double - ascii
  if (!(VTKImageIO2Tester<double, 3>::Write( filePrefix, outputPath, true )))
    {
    std::cout << "[FAILED] writing (double - ascii)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] writing (double - ascii)" << std::endl;
  if (!(VTKImageIO2Tester<double, 3>::Read( filePrefix, outputPath, true )))
    {
    std::cout << "[FAILED] reading (double - ascii)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] reading (double - ascii)" << std::endl;
  
  // double - binary
  if (!(VTKImageIO2Tester<double, 3>::Write( filePrefix, outputPath, false )))
    {
    std::cout << "[FAILED] writing (double - binary)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] writing (double - binary)" << std::endl;
  if (!(VTKImageIO2Tester<double, 3>::Read( filePrefix, outputPath, false )))
    {
    std::cout << "[FAILED] reading (double - binary)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] reading (double - binary)" << std::endl;
  
  // RGBPixel<unsigned char> - ascii
  if (!(VTKImageIO2Tester< itk::RGBPixel<unsigned char>, 3>::Write( filePrefix, outputPath, true )))
    {
    std::cout << "[FAILED] writing (RGBPixel<unsigned char> - ascii)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] writing (RGBPixel<unsigned char> - ascii)" << std::endl;
  if (!(VTKImageIO2Tester< itk::RGBPixel<unsigned char>, 3>::Read( filePrefix, outputPath, true )))
    {
    std::cout << "[FAILED] reading (RGBPixel<unsigned char> - ascii)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] reading (RGBPixel<unsigned char> - ascii)" << std::endl;
  
  // RGBPixel<unsigned char> - binary
  if (!(VTKImageIO2Tester< itk::RGBPixel<unsigned char>, 3>::Write( filePrefix, outputPath, false )))
    {
    std::cout << "[FAILED] writing (RGBPixel<unsigned char> - binary)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] writing (RGBPixel<unsigned char> - binary)" << std::endl;
  if (!(VTKImageIO2Tester< itk::RGBPixel<unsigned char>, 3>::Read( filePrefix, outputPath, false )))
    {
    std::cout << "[FAILED] reading (RGBPixel<unsigned char> - binary)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] reading (RGBPixel<unsigned char> - binary)" << std::endl;
  
  // RGBAPixel<unsigned char> - ascii
  if (!(VTKImageIO2Tester< itk::RGBAPixel<unsigned char>, 3>::Write( filePrefix, outputPath, true )))
    {
    std::cout << "[FAILED] writing (RGBAPixel<unsigned char> - ascii)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] writing (RGBAPixel<unsigned char> - ascii)" << std::endl;
  if (!(VTKImageIO2Tester< itk::RGBAPixel<unsigned char>, 3>::Read( filePrefix, outputPath, true )))
    {
    std::cout << "[FAILED] reading (RGBAPixel<unsigned char> - ascii)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] reading (RGBAPixel<unsigned char> - ascii)" << std::endl;
  
  // RGBAPixel<unsigned char> - binary
  if (!(VTKImageIO2Tester< itk::RGBAPixel<unsigned char>, 3>::Write( filePrefix, outputPath, false )))
    {
    std::cout << "[FAILED] writing (RGBAPixel<unsigned char> - binary)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] writing (RGBAPixel<unsigned char> - binary)" << std::endl;
  if (!(VTKImageIO2Tester< itk::RGBAPixel<unsigned char>, 3>::Read( filePrefix, outputPath, false )))
    {
    std::cout << "[FAILED] reading (RGBAPixel<unsigned char> - binary)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] reading (RGBAPixel<unsigned char> - binary)" << std::endl;

  // Vector<int> - ascii
  if (!(VTKImageIO2Tester< itk::Vector<int, TEST_VECTOR_PIXEL_DIM>, 3 >::Write( filePrefix, outputPath, true )))
    {
    std::cout << "[FAILED] writing (Vector<int> - ascii)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] writing (Vector<int> - ascii)" << std::endl;
  if (!(VTKImageIO2Tester< itk::Vector<int, TEST_VECTOR_PIXEL_DIM>, 3 >::Read( filePrefix, outputPath, true )))
    {
    std::cout << "[FAILED] reading (Vector<int> - ascii)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] reading (Vector<int> - ascii)" << std::endl;
  
  // Vector<int> - binary
  if (!(VTKImageIO2Tester< itk::Vector<int, TEST_VECTOR_PIXEL_DIM>, 3 >::Write( filePrefix, outputPath, false )))
    {
    std::cout << "[FAILED] writing (Vector<int> - binary)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] writing (Vector<int> - binary)" << std::endl;
  if (!(VTKImageIO2Tester< itk::Vector<int, TEST_VECTOR_PIXEL_DIM>, 3 >::Read( filePrefix, outputPath, false )))
    {
    std::cout << "[FAILED] reading (Vector<int> - binary)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] reading (Vector<int> - binary)" << std::endl;
  
  // Vector<double> - ascii
  if (!(VTKImageIO2Tester< itk::Vector<double, TEST_VECTOR_PIXEL_DIM>, 3 >::Write( filePrefix, outputPath, true )))
    {
    std::cout << "[FAILED] writing (Vector<double> - ascii)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] writing (Vector<double> - ascii)" << std::endl;
  if (!(VTKImageIO2Tester< itk::Vector<double, TEST_VECTOR_PIXEL_DIM>, 3 >::Read( filePrefix, outputPath, true )))
    {
    std::cout << "[FAILED] reading (Vector<double> - ascii)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] reading (Vector<double> - ascii)" << std::endl;
  
  // Vector<double> - binary
  if (!(VTKImageIO2Tester< itk::Vector<double, TEST_VECTOR_PIXEL_DIM>, 3 >::Write( filePrefix, outputPath, false )))
    {
    std::cout << "[FAILED] writing (Vector<double> - binary)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] writing (Vector<double> - binary)" << std::endl;
  if (!(VTKImageIO2Tester< itk::Vector<double, TEST_VECTOR_PIXEL_DIM>, 3 >::Read( filePrefix, outputPath, false )))
    {
    std::cout << "[FAILED] reading (Vector<double> - binary)" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] reading (Vector<double> - binary)" << std::endl;
  
  
  //
  // Test bad paths
  //
  
  // read bad file extension
  if ( VTKImageIO2Tester<char,3>::CanReadFileTest( filePrefix, "bad", outputPath ))
    {
    std::cout << "[FAILED] didn't properly reject bad file extension for reading" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] rejected bad file extension for reading" << std::endl;
  
  // read bad file name
  if ( VTKImageIO2Tester<char,3>::CanReadFileTest( "BadFile", "vtk", outputPath ))
    {
    std::cout << "[FAILED] didn't properly reject bad file name for reading" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] rejected bad file name for reading" << std::endl;
  
  // write bad file extension
  if ( VTKImageIO2Tester<char,3>::CanWriteFileTest( filePrefix, "bad", outputPath ))
    {
    std::cout << "[FAILED] didn't properly reject bad file extension for writing" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] rejected bad file extension for writing" << std::endl;
  
  
  //
  // use print methods
  //
  typedef itk::VTKImageIO2 IOType;
  IOType::Pointer vtkIO = IOType::New();
  EXERCISE_BASIC_OBJECT_METHODS( vtkIO, IOType );


  //
  // All tests successful
  //
  return EXIT_SUCCESS;
}
