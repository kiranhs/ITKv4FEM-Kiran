/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkDicomImageIODirection2DTest.cxx
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

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImage.h"

int itkDicomImageIODirection2DTest( int argc, char * argv[] )
{

  if( argc != 4 )
    {
    std::cerr << "Usage: " << argv[0] 
    << " OutputImage3DFormat1 OutputImage3DFormat2  OutputImage2DFormat1 "
    << std::endl;
    return EXIT_FAILURE;
    }
  
  typedef signed short PixelType;

  typedef itk::Image<PixelType, 2 >    Image2DType;
  typedef itk::Image<PixelType, 3 >    Image3DType;

  typedef itk::ImageFileWriter< Image3DType >   Writer3DType;
  typedef itk::ImageFileReader< Image2DType >   Reader2DType;

  Image3DType::Pointer image3D = Image3DType::New();

  Image3DType::RegionType region;
  Image3DType::SizeType size;
  size[0] = 100;
  size[1] = 100;
  size[2] = 1;

  Image3DType::IndexType index;
  index.Fill( 0 );

  region.SetIndex( index );
  region.SetSize( size );

  image3D->SetRegions( region );
  image3D->Allocate();
  
  Image3DType::DirectionType direction;

  direction(0,0) =  0.0;
  direction(0,1) = -1.0;
  direction(0,2) =  0.0;

  direction(1,0) =  0.0;
  direction(1,1) =  0.0;
  direction(1,2) = -1.0;

  direction(2,0) = 1.0;
  direction(2,1) = 0.0;
  direction(2,2) = 0.0;

  typedef itk::ImageRegionIterator< Image3DType > IteratorType;

  IteratorType itr( image3D, region );

  itr.GoToBegin();

  PixelType pixelValue = -1024;

  while( !itr.IsAtEnd() )
    {
    itr.Set( pixelValue++ );
    ++itr;
    }

  image3D->SetDirection( direction );

  Writer3DType::Pointer writer3D = Writer3DType::New();

  writer3D->SetInput( image3D );

  writer3D->SetFileName( argv[1] );
  std::cout << "Writing " << argv[1] << std::endl;
  writer3D->Update();

  writer3D->SetFileName( argv[2] );
  std::cout << "Writing " << argv[2] << std::endl;
  writer3D->Update();

  Reader2DType::Pointer reader2D = Reader2DType::New();
  
  std::cout << "Reading " << argv[1] << std::endl;
  reader2D->SetFileName( argv[1] );

  try
    {
    reader2D->Update();
    }
  catch (itk::ExceptionObject & e)
    {
    std::cerr << e << std::endl;
    return EXIT_FAILURE;
    }
  
  Image2DType::ConstPointer image2D = reader2D->GetOutput();
   
  Image2DType::DirectionType directionCosines = image2D->GetDirection();

  std::cout << directionCosines << std::endl;

  typedef itk::ImageFileWriter< Image2DType >   Writer2DType;
  Writer2DType::Pointer writer2D = Writer2DType::New();
  writer2D->SetFileName( argv[3] );
  writer2D->SetInput( image2D );

  try
    {
    writer2D->Update();
    }
  catch (itk::ExceptionObject & e)
    {
    std::cerr << e << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
