/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkShapeKeepNObjectsLabelMapFilterTest1.cxx
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
#include "itkShapeLabelObject.h"
#include "itkShapeLabelObjectAccessors.h"
#include "itkLabelMap.h"

#include "itkShapeKeepNObjectsLabelMapFilter.h"
#include "itkLabelImageToShapeLabelMapFilter.h"
#include "itkLabelMapToLabelImageFilter.h"

#include "itkTestingMacros.h"

int itkShapeKeepNObjectsLabelMapFilterTest1(int argc, char * argv[])
{
  if( argc != 6 )
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " input output";
    std::cerr << " reverseOrdering attribute numberOfObjectsToKeep";
    std::cerr << std::endl;
    return EXIT_FAILURE;
    }

  const unsigned int dim = 3;
 
  typedef unsigned char PixelType;
 
  typedef itk::Image< PixelType, dim > ImageType;

  typedef itk::ShapeLabelObject< PixelType, dim >           ShapeLabelObjectType;
  typedef itk::LabelMap< ShapeLabelObjectType >             LabelMapType;

  typedef itk::ImageFileReader< ImageType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );
 
  typedef itk::LabelImageToShapeLabelMapFilter< ImageType, LabelMapType> I2LType;
  I2LType::Pointer i2l = I2LType::New();
  i2l->SetInput( reader->GetOutput() );

  typedef itk::ShapeKeepNObjectsLabelMapFilter< LabelMapType > LabelOpeningType;
  LabelOpeningType::Pointer opening = LabelOpeningType::New();
 
  //testing boolean macro for ReverseOrdering
  opening->ReverseOrderingOn();
  TEST_SET_GET_VALUE( true, opening->GetReverseOrdering() ); 

  opening->ReverseOrderingOff();
  TEST_SET_GET_VALUE( false, opening->GetReverseOrdering() );

  //testing get and set macros or ReverseOrdering 
  bool reverseOrdering = atoi( argv[3] );
  opening->SetReverseOrdering( reverseOrdering );
  TEST_SET_GET_VALUE( reverseOrdering , opening->GetReverseOrdering() ); 

  //testing get and set macros for Attribute 
  LabelOpeningType::AttributeType attribute = atoi( argv[4] );
  TRY_EXPECT_NO_EXCEPTION( opening->SetAttribute( attribute ) );
  try
    {
    TEST_SET_GET_VALUE( attribute, opening->GetAttribute() );
    }
  catch (itk::ExceptionObject &exc)
    {
    std::cerr << "Unexpected exception detected: "  << exc;
    return EXIT_FAILURE;
    }
  opening->SetNumberOfObjects( atoi(argv[5]) );
  opening->SetInput( i2l->GetOutput() );

  itk::SimpleFilterWatcher watcher(opening, "filter");

  typedef itk::LabelMapToLabelImageFilter< LabelMapType, ImageType> L2IType;
  L2IType::Pointer l2i = L2IType::New();
  l2i->SetInput( opening->GetOutput() );

  typedef itk::ImageFileWriter< ImageType > WriterType;

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( l2i->GetOutput() );
  writer->SetFileName( argv[2] );
  writer->UseCompressionOn();
  
  TRY_EXPECT_NO_EXCEPTION( writer->Update() );

  return EXIT_SUCCESS;
}
