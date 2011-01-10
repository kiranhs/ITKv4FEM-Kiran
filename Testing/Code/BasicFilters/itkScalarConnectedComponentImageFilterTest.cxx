/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkScalarConnectedComponentImageFilterTest.cxx
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

#include "itkScalarConnectedComponentImageFilter.h"
#include "itkRelabelComponentImageFilter.h"
#include "itkImageFileReader.h"
#include "itkVTKImageIO.h"
#include "itkImageFileWriter.h"
#include "itkImageRegionIterator.h"
#include "itkSimpleFilterWatcher.h"
#include "itkImageRegionIterator.h"
#include "itkMersenneTwisterRandomVariateGenerator.h"

int itkScalarConnectedComponentImageFilterTest(int argc, char* argv[] )
{
  if( argc < 4 )
    {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputImage  outputImage distance_threshold [fully_connected] [minimum_object_size]" << std::endl;
    return EXIT_FAILURE;
    }

  typedef   unsigned short  InternalPixelType;
  typedef   bool  MaskPixelType;
  const     unsigned int    Dimension = 2;
  
  typedef itk::Image< InternalPixelType, Dimension >  InternalImageType;
  typedef itk::Image< MaskPixelType, Dimension >  MaskImageType;
  typedef itk::Image<unsigned short,Dimension> OutputImageType;

  typedef itk::RGBPixel<unsigned char>   RGBPixelType;
  typedef itk::Image<RGBPixelType, Dimension>    RGBImageType;

  typedef itk::ImageFileReader< InternalImageType > ReaderType;
  typedef itk::ImageFileWriter<  RGBImageType  > WriterType;

  
  typedef itk::ScalarConnectedComponentImageFilter< InternalImageType, OutputImageType, MaskImageType > FilterType;
  typedef itk::RelabelComponentImageFilter< OutputImageType, OutputImageType > RelabelType;
  
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  FilterType::Pointer filter = FilterType::New();
  RelabelType::Pointer relabel = RelabelType::New();
  
  itk::SimpleFilterWatcher watcher(filter);
  watcher.QuietOn();

  reader->SetFileName( argv[1] );
  reader->Update();
  
  // create a mask containing the upper left hand corner and
  // a chunk out of the middle
  MaskImageType::Pointer mask = MaskImageType::New();
  mask->SetRegions(reader->GetOutput()->GetLargestPossibleRegion());
  mask->CopyInformation(reader->GetOutput());
  mask->Allocate();
  mask->FillBuffer(itk::NumericTraits<MaskPixelType>::Zero);

  MaskImageType::RegionType maskRegion = mask->GetLargestPossibleRegion();
  MaskImageType::SizeType maskSize = maskRegion.GetSize();

  MaskImageType::RegionType region;
  MaskImageType::SizeType size;
  MaskImageType::IndexType index;
  
  // use upper left corner
  index.Fill(0);
  for (unsigned int i=0; i<MaskImageType::ImageDimension; i++)
    {
    size[i] = static_cast<unsigned long> (0.5 * maskSize[i]);
    }
  region.SetIndex (index);
  region.SetSize (size);

  itk::ImageRegionIterator<MaskImageType> mit(mask,region);
  while (!mit.IsAtEnd())
    {
    mit.Set(itk::NumericTraits<MaskPixelType>::max());
    ++mit;
    }

  // use middle section
  for (unsigned int i=0; i<MaskImageType::ImageDimension; i++)
    {
    index[i] = static_cast<long> (0.375 * maskSize[i]);
    size[i] = static_cast<unsigned long> (0.25 * maskSize[i]);
    }
  region.SetIndex (index);
  region.SetSize (size);

  itk::ImageRegionIterator<MaskImageType> mit2(mask,region);
  while (!mit2.IsAtEnd())
    {
    mit2.Set(itk::NumericTraits<MaskPixelType>::max());
    ++mit2;
    }

  filter->SetInput (reader->GetOutput());
  filter->SetMaskImage (mask);
  filter->SetDistanceThreshold( atoi(argv[3]) );
  filter->SetFunctor(filter->GetFunctor());

  if (argc > 4)
    {
    int fullyConnected = atoi( argv[4] );
    filter->SetFullyConnected( fullyConnected );
    }
  relabel->SetInput( filter->GetOutput() );
  if (argc > 5)
    {
    int minSize = atoi( argv[5] );
    relabel->SetMinimumObjectSize( minSize );
    std::cerr << "minSize: " << minSize << std::endl;
    }
  
  try
    {
    relabel->Update();
    }
  catch( itk::ExceptionObject & excep )
    {
    std::cerr << "Relabel: exception caught !" << std::endl;
    std::cerr << excep << std::endl;
    }

  // Remap the labels to viewable colors
  RGBImageType::Pointer colored = RGBImageType::New();
  colored->SetRegions( filter->GetOutput()->GetBufferedRegion() );
  colored->Allocate();

  unsigned short numObjects = relabel->GetNumberOfObjects();
  
  std::vector<RGBPixelType> colormap;
  RGBPixelType px;
  colormap.resize( numObjects+1 );
  itk::Statistics::MersenneTwisterRandomVariateGenerator::GetInstance()->SetSeed(1031571);
  itk::Statistics::MersenneTwisterRandomVariateGenerator::Pointer rvgen = itk::Statistics::MersenneTwisterRandomVariateGenerator::New();
  for (unsigned short i=0; i < colormap.size(); ++i)
    {
    px.SetRed(
      static_cast<unsigned char>(255*rvgen->GetUniformVariate( 0.3333, 1.0 ) ));
    px.SetGreen(
      static_cast<unsigned char>(255*rvgen->GetUniformVariate( 0.3333, 1.0 ) ));
    px.SetBlue(
      static_cast<unsigned char>(255*rvgen->GetUniformVariate( 0.3333, 1.0 ) ));
    colormap[i] = px;
    }
  
  itk::ImageRegionIterator<OutputImageType>
    it(relabel->GetOutput(), relabel->GetOutput()->GetBufferedRegion());
  itk::ImageRegionIterator<RGBImageType> cit(colored,
                                             colored->GetBufferedRegion());
  
  while( !it.IsAtEnd() )
    {
    if (it.Get() == 0)
      {
      cit.Set(RGBPixelType(static_cast<unsigned char>(0)));
      }
    else
      {
      cit.Set( colormap[it.Get()] );
      }
    ++it;
    ++cit;
    }
  
  try
    {
    writer->SetInput (colored);
    writer->SetFileName( argv[2] );
    writer->Update();
    }
  catch( itk::ExceptionObject & excep )
    {
    std::cerr << "Exception caught !" << std::endl;
    std::cerr << excep << std::endl;
    }

  return EXIT_SUCCESS;
}
