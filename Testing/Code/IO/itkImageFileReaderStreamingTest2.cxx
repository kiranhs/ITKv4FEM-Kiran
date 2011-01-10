/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkImageFileReaderStreamingTest2.cxx
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

#include <fstream>
#include "itkImageFileReader.h"
#include "itkPipelineMonitorImageFilter.h"
#include "itkStreamingImageFilter.h"


#include "itkDifferenceImageFilter.h"
#include "itkExtractImageFilter.h"

typedef unsigned char            PixelType;
typedef itk::Image<PixelType,3>  ImageType;
typedef ImageType::ConstPointer  ImageConstPointer;

namespace { // local namespace

// test is a sub region of the base line,
// just the region defined by the test image is used to compare
bool SameRegionImage( ImageConstPointer test, ImageConstPointer baseline )
{
  
  PixelType intensityTolerance = 0;
  unsigned int radiusTolerance = 0;
  unsigned int numberOfPixelTolerance = 0;

  typedef itk::ExtractImageFilter<ImageType, ImageType> ExtractImageFilter;
  ExtractImageFilter::Pointer extractor = ExtractImageFilter::New();
  extractor->SetExtractionRegion( test->GetLargestPossibleRegion() );
  extractor->SetInput( baseline );
  

  typedef itk::DifferenceImageFilter<ImageType,ImageType> DiffType;
  DiffType::Pointer diff = DiffType::New();
  diff->SetValidInput( extractor->GetOutput());
  diff->SetTestInput(test);
  diff->SetDifferenceThreshold( intensityTolerance );
  diff->SetToleranceRadius( radiusTolerance );
  diff->UpdateLargestPossibleRegion();

   unsigned long status = diff->GetNumberOfPixelsWithDifferences();
   std::cout << "NumberOfPixelsWithDifferences: " << status << std::endl;

   if (status > numberOfPixelTolerance)
     return false;
   return true;
}



}

int itkImageFileReaderStreamingTest2(int argc, char* argv[])
{
  if( argc < 2 )
    {
    std::cerr << "Usage: " << argv[0] << " input " << std::endl;
    return EXIT_FAILURE;
    }
  
  typedef itk::ImageFileReader<ImageType>      ReaderType;

  ReaderType::Pointer baselineReader = ReaderType::New();
  baselineReader->SetFileName( argv[1] );
  baselineReader->UpdateLargestPossibleRegion();

  ImageType::ConstPointer baselineImage = baselineReader->GetOutput();

  ReaderType::Pointer streamingReader = ReaderType::New();
  streamingReader->SetFileName( argv[1] );
  streamingReader->UseStreamingOn();


  typedef itk::PipelineMonitorImageFilter<ImageType> MonitorFilter;
  MonitorFilter::Pointer monitor = MonitorFilter::New();
  monitor->SetInput( streamingReader->GetOutput());
  
  
  typedef itk::ExtractImageFilter<ImageType, ImageType> ExtractImageFilter;
  ExtractImageFilter::Pointer extractor = ExtractImageFilter::New();
  extractor->SetInput( monitor->GetOutput() );

  
  const ImageType::RegionType largestRegion = baselineReader->GetOutput()->GetLargestPossibleRegion();
  ImageType::RegionType ioregion = baselineReader->GetOutput()->GetLargestPossibleRegion();

  streamingReader->Modified();
  ////////////////////////////////////////////////
  // test whole Image
  ioregion.SetIndex(0, largestRegion.GetIndex()[0]);
  ioregion.SetIndex(1, largestRegion.GetIndex()[1]);
  ioregion.SetIndex(2, largestRegion.GetIndex()[2]);
  ioregion.SetSize(0, 1);
  ioregion.SetSize(1, largestRegion.GetSize()[1]);
  ioregion.SetSize(2, 1);

  extractor->SetExtractionRegion( ioregion );
  std::cout << "=== Updating full IORegion ==" << std::endl;
  extractor->Update();
  
  if (!monitor->VerifyAllInputCanStream(1)) 
    {
    std::cout << monitor;
    return EXIT_FAILURE;
    }

  if (!SameRegionImage( extractor->GetOutput(), baselineImage) )
    {
    return EXIT_FAILURE;
    }

  ////////////////////////////////////////////////
  // test fullx1x1
  ioregion.SetIndex(0, largestRegion.GetIndex()[0]);
  ioregion.SetIndex(1, largestRegion.GetIndex()[1]+largestRegion.GetSize()[1]/2 + 1);
  ioregion.SetIndex(2, largestRegion.GetIndex()[2]+largestRegion.GetSize()[2]/2 + 1);
  ioregion.SetSize(0, largestRegion.GetSize()[0]);
  ioregion.SetSize(1, 1);
  ioregion.SetSize(2, 1);
  
  extractor->SetExtractionRegion( ioregion );
  std::cout << "=== Updating fullx1x1 IORegion ==" << std::endl;
  extractor->UpdateLargestPossibleRegion();
  std::cout << "check\n";
  
  if (!monitor->VerifyAllInputCanStream(1)) 
    {
    std::cout << monitor;
    return EXIT_FAILURE;
    }

  if (!SameRegionImage( extractor->GetOutput(), baselineImage) )
    {
    return EXIT_FAILURE;
    }

  
  ////////////////////////////////////////////////
  // test 1xfullx1
  ioregion.SetIndex(0, largestRegion.GetIndex()[0]+largestRegion.GetSize()[0]/2 + 1);
  ioregion.SetIndex(1, largestRegion.GetIndex()[1]);
  ioregion.SetIndex(2, largestRegion.GetIndex()[2]+largestRegion.GetSize()[2]/2 + 1);
  ioregion.SetSize(0, 1);
  ioregion.SetSize(1, largestRegion.GetSize()[1]);
  ioregion.SetSize(2, 1);

  extractor->SetExtractionRegion( ioregion );
  std::cout << "=== Updating 1xfullx1 IORegion ==" << std::endl;
  extractor->UpdateLargestPossibleRegion();
  
  if (!monitor->VerifyAllInputCanStream(1)) 
    {
    std::cout << monitor;
    return EXIT_FAILURE;
    }

  if (!SameRegionImage( extractor->GetOutput(), baselineImage) )
    {
    return EXIT_FAILURE;
    }


  ////////////////////////////////////////////////
  // test 1x1xfull
  ioregion.SetIndex(0, largestRegion.GetIndex()[0]+largestRegion.GetSize()[0]/2 + 1);
  ioregion.SetIndex(1, largestRegion.GetIndex()[1]+largestRegion.GetSize()[1]/2 + 1);
  ioregion.SetIndex(2, largestRegion.GetIndex()[2]);
  ioregion.SetSize(0, 1);
  ioregion.SetSize(1, 1);
  ioregion.SetSize(2, largestRegion.GetSize()[2]);

  extractor->SetExtractionRegion( ioregion ); 
    std::cout << "=== Updating 1x1xfull IORegion ==" << std::endl;
  extractor->UpdateLargestPossibleRegion();
  
  if (!monitor->VerifyAllInputCanStream(1)) 
    {
    std::cout << monitor;
    return EXIT_FAILURE;
    }

  if (!SameRegionImage( extractor->GetOutput(), baselineImage) )
    {
    return EXIT_FAILURE;
    }


  ////////////////////////////////////////////////
  // test fullx3x3
  ioregion.SetIndex(0, largestRegion.GetIndex()[0]);
  ioregion.SetIndex(1, largestRegion.GetIndex()[1]+largestRegion.GetSize()[1]/2 + 1);
  ioregion.SetIndex(2, largestRegion.GetIndex()[2]+largestRegion.GetSize()[2]/2 + 1);
  ioregion.SetSize(0, largestRegion.GetSize()[0]);
  ioregion.SetSize(1, 3);
  ioregion.SetSize(2, 3);
  
  extractor->SetExtractionRegion( ioregion );
  std::cout << "=== Updating fullx3x3 IORegion ==" << std::endl;
  extractor->UpdateLargestPossibleRegion();

  if (!monitor->VerifyAllInputCanStream(1)) 
    {
    std::cout << monitor;
    return EXIT_FAILURE;
    }
  
  if (!SameRegionImage( extractor->GetOutput(), baselineImage) )
    {
    return EXIT_FAILURE;
    }

  
  ////////////////////////////////////////////////
  // test 3xfullx3
  ioregion.SetIndex(0, largestRegion.GetIndex()[0]+largestRegion.GetSize()[0]/2 + 1);
  ioregion.SetIndex(1, largestRegion.GetIndex()[1]);
  ioregion.SetIndex(2, largestRegion.GetIndex()[2]+largestRegion.GetSize()[2]/2 + 1);
  ioregion.SetSize(0, 3);
  ioregion.SetSize(1, largestRegion.GetSize()[1]);
  ioregion.SetSize(2, 3);

  extractor->SetExtractionRegion( ioregion );
  std::cout << "=== Updating 3xfullx3 IORegion ==" << std::endl;
  extractor->UpdateLargestPossibleRegion();

  if (!monitor->VerifyAllInputCanStream(1)) 
    {
    std::cout << monitor;
    return EXIT_FAILURE;
    }

  if (!SameRegionImage( extractor->GetOutput(), baselineImage) )
    {
    return EXIT_FAILURE;
    }


  ////////////////////////////////////////////////
  // test 3x3xfull
  ioregion.SetIndex(0, largestRegion.GetIndex()[0]+largestRegion.GetSize()[0]/2 + 1);
  ioregion.SetIndex(1, largestRegion.GetIndex()[1]+largestRegion.GetSize()[1]/2 + 1);
  ioregion.SetIndex(2, largestRegion.GetIndex()[2]);
  ioregion.SetSize(0, 3);
  ioregion.SetSize(1, 3);
  ioregion.SetSize(2, largestRegion.GetSize()[2]);

  extractor->SetExtractionRegion( ioregion ); 
    std::cout << "=== Updating 3x3xfull IORegion ==" << std::endl;
  extractor->UpdateLargestPossibleRegion();
  
if (!monitor->VerifyAllInputCanStream(1)) 
    {
    std::cout << monitor;
    return EXIT_FAILURE;
    }

  if (!SameRegionImage( extractor->GetOutput(), baselineImage) )
    {
    return EXIT_FAILURE;
    }


  return EXIT_SUCCESS;
}
