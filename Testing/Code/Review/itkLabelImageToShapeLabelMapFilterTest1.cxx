/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkLabelImageToShapeLabelMapFilterTest1.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkSimpleFilterWatcher.h"

#include "itkLabelObject.h"
#include "itkLabelMap.h"
#include "itkLabelImageToShapeLabelMapFilter.h"
#include "itkLabelMapToLabelImageFilter.h"
#include "itkTestingMacros.h"

int itkLabelImageToShapeLabelMapFilterTest1(int argc, char * argv[])
{
  if( argc != 6 )
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputBinaryImage outputShapeLabelMap";
    std::cerr << " backgroundValue computeFeretDiameter computePerimeter";
    std::cerr << std::endl;
    return EXIT_FAILURE;
    }

  const unsigned int dim = 2;
  
  typedef unsigned char PixelType;

  typedef itk::Image< PixelType, dim > ImageType;

  typedef itk::ShapeLabelObject< PixelType, dim >     LabelObjectType;
  typedef itk::LabelMap< LabelObjectType >            LabelMapType;
  
  typedef itk::ImageFileReader< ImageType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );
  
  typedef itk::LabelImageToShapeLabelMapFilter< ImageType, LabelMapType> L2SType;
  L2SType::Pointer l2s = L2SType::New();
  l2s->SetInput( reader->GetOutput() );

  const PixelType backgroundValue = atoi(argv[3]);
  l2s->SetBackgroundValue( backgroundValue );
  TEST_SET_GET_VALUE( backgroundValue, l2s->GetBackgroundValue() );
  
  const bool computeFeretDiameter = atoi( argv[4]);
  l2s->SetComputeFeretDiameter( computeFeretDiameter );
  TEST_SET_GET_VALUE( computeFeretDiameter, l2s->GetComputeFeretDiameter() );

  const bool computePerimeter = atoi( argv[5]);
  l2s->SetComputePerimeter( computePerimeter );
  TEST_SET_GET_VALUE( computePerimeter, l2s->GetComputeFeretDiameter() );


  typedef itk::LabelMapToLabelImageFilter< LabelMapType, ImageType> L2IType;
  L2IType::Pointer l2i = L2IType::New();
  l2i->SetInput( l2s->GetOutput() );

  typedef itk::ImageFileWriter< ImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( l2i->GetOutput() );
  writer->SetFileName( argv[2] );
  writer->UseCompressionOn();

  TRY_EXPECT_NO_EXCEPTION( writer->Update() );

  return EXIT_SUCCESS;
}
