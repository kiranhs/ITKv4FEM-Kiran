/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkMergeLabelMapFilterTest1.cxx
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
#include "itkLabelImageToLabelMapFilter.h"
#include "itkMergeLabelMapFilter.h"
#include "itkLabelMapToLabelImageFilter.h"

#include "itkTestingMacros.h"

int itkMergeLabelMapFilterTest1( int argc, char * argv[] )
{
  if( argc != 8 )
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " input1 input2 output background1 background2 method expectfailure" << std::endl;
    return EXIT_FAILURE;
    }

  const unsigned int dim = 2;
  typedef unsigned char PixelType;
  
  typedef itk::Image< PixelType, dim > ImageType;

  typedef itk::LabelObject< PixelType, dim >      LabelObjectType;
  typedef itk::LabelMap< LabelObjectType >        LabelMapType;
  
  typedef itk::ImageFileReader< ImageType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );
  
  typedef itk::LabelImageToLabelMapFilter< ImageType, LabelMapType> I2LType;
  I2LType::Pointer i2l = I2LType::New();
  i2l->SetInput( reader->GetOutput() );

  const PixelType background1 = atoi(argv[4]);
  i2l->SetBackgroundValue( background1 );
  TEST_SET_GET_VALUE( background1, i2l->GetBackgroundValue() );
 
  ReaderType::Pointer reader2 = ReaderType::New();
  reader2->SetFileName( argv[2] );
  I2LType::Pointer i2l2 = I2LType::New();
  i2l2->SetInput( reader2->GetOutput() );

  const PixelType background2 = atoi(argv[5]);
  i2l2->SetBackgroundValue( background2 );
  TEST_SET_GET_VALUE( background2, i2l2->GetBackgroundValue() );
 
  typedef itk::MergeLabelMapFilter< LabelMapType > ChangeType;
  ChangeType::Pointer change = ChangeType::New();
  change->SetInput( i2l->GetOutput() );
  change->SetInput( 1, i2l2->GetOutput() );

  typedef ChangeType::MethodChoice MethodChoice;
  MethodChoice method = static_cast<MethodChoice>( atoi( argv[6] ) );

  change->SetMethod( ChangeType::STRICT );
  TEST_SET_GET_VALUE( ChangeType::STRICT, change->GetMethod() );

  change->SetMethod( ChangeType::KEEP );
  TEST_SET_GET_VALUE( ChangeType::KEEP, change->GetMethod() );

  change->SetMethod( method ); 
  itk::SimpleFilterWatcher watcher6(change, "filter");

  typedef itk::LabelMapToLabelImageFilter< LabelMapType, ImageType> L2IType;
  L2IType::Pointer l2i = L2IType::New();
  l2i->SetInput( change->GetOutput() );

  typedef itk::ImageFileWriter< ImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( l2i->GetOutput() );
  writer->SetFileName( argv[3] );
  writer->UseCompressionOn();
  
  bool expectfailure = atoi( argv[7] );

  if( expectfailure )
    {
    TRY_EXPECT_EXCEPTION( writer->Update() );
    }
  else
    {
    TRY_EXPECT_NO_EXCEPTION( writer->Update() );
    }

  TEST_SET_GET_VALUE( background1, change->GetOutput()->GetBackgroundValue() );

  return EXIT_SUCCESS;
}
