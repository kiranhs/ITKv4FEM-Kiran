/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit (ITK)
  Module:    itkLevelSetNeighborhoodExtractorTest.cxx
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

#include "itkLevelSetNeighborhoodExtractor.h"
#include "itkFastMarchingImageFilter.h"

int itkLevelSetNeighborhoodExtractorTest(int, char* [] )
{
  const unsigned int ImageDimension = 2;
  typedef float PixelType;
  typedef itk::Image<PixelType,ImageDimension> ImageType;

  // Create an input image using fastmarching
  typedef itk::FastMarchingImageFilter<ImageType> SourceType;
  SourceType::Pointer source = SourceType::New();

  ImageType::SizeType size;
  size.Fill( 17 );
  
  source->SetOutputSize( size );

  SourceType::NodeType node;
  ImageType::IndexType index;
  index.Fill( 8 );
  
  node.SetIndex( index );
  node.SetValue( -4.0 );

  typedef SourceType::NodeContainer NodeContainerType;
  NodeContainerType::Pointer container = NodeContainerType::New();

  container->InsertElement( 0, node );

  source->SetTrialPoints( container );
  source->CollectPointsOn();
  source->Update();
  
  typedef itk::LevelSetNeighborhoodExtractor<ImageType> ExtractorType;
  ExtractorType::Pointer extractor = ExtractorType::New();

  extractor->SetInputLevelSet( source->GetOutput() );
  extractor->SetLevelSetValue( 0.0 );
  extractor->NarrowBandingOff();

  extractor->Locate();

  typedef NodeContainerType::ConstIterator Iterator;
  Iterator iter;
  Iterator iterEnd;

  std::cout << "Inside Points" << std::endl;
  iter    = extractor->GetInsidePoints()->Begin();
  iterEnd = extractor->GetInsidePoints()->End();
  for( ; iter != iterEnd; iter++ )
    {
    std::cout << iter.Value().GetIndex() << " ";
    std::cout << iter.Value().GetValue() << std::endl;
    }
  
  std::cout << "Outside Points" << std::endl;
  iter    = extractor->GetOutsidePoints()->Begin();
  iterEnd = extractor->GetOutsidePoints()->End();
  for( ; iter != iterEnd; iter++ )
    {
    std::cout << iter.Value().GetIndex() << " ";
    std::cout << iter.Value().GetValue() << std::endl;
    }

  // exercise Print
  extractor->Print( std::cout );

  // exercise Get methods
  std::cout << "InputLevelSet: " << extractor->GetInputLevelSet() << std::endl;
  std::cout << "LevelSetValue: " << extractor->GetLevelSetValue() << std::endl;
  std::cout << "NarrowBandwidth: " << extractor->GetNarrowBandwidth() << std::endl;
  std::cout << "NarrowBanding: " << extractor->GetNarrowBanding() << std::endl;
  std::cout << "InputNarrowBand: " << extractor->GetInputNarrowBand() << std::endl;

  // exercise error handling
  bool passed;
  std::cout << "Testing NULL inputs" << std::endl;

  try
    {
    passed = false;
    extractor->SetInputLevelSet( NULL );
    extractor->Locate();
    }
  catch( itk::ExceptionObject& err )
    {
    std::cout << err << std::endl;
    passed = true;
    extractor->SetInputLevelSet( source->GetOutput() );
    }
 
  if ( !passed )
    {
    std::cout << "Test failed" << std::endl;
    return EXIT_FAILURE;
    }

  try
    {
    passed = false;
    extractor->NarrowBandingOn();
    extractor->SetInputNarrowBand( NULL );
    extractor->Locate();
    }
  catch( itk::ExceptionObject& err )
    {
    std::cout << err << std::endl;
    passed = true;
    extractor->NarrowBandingOff();
    }
 
  if ( !passed )
    {
    std::cout << "Test failed" << std::endl;
    return EXIT_FAILURE;
    }

  std::cout << "Test passed." << std::endl;
  return EXIT_SUCCESS;

}
