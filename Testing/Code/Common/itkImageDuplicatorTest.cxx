/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkImageDuplicatorTest.cxx
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

#include <iostream>
#include <itkImageRegionIterator.h>
#include "itkImageDuplicator.h"
#include <itkRGBPixel.h>
#include "itkShiftScaleImageFilter.h"

int itkImageDuplicatorTest(int, char* [] )
{

  /** Create an image image */
  typedef itk::Image<float,3> ImageType;
  std::cout << "Creating simulated image: ";
  ImageType::Pointer m_Image = ImageType::New();
  ImageType::RegionType region;
  ImageType::SizeType size;
  size[0] = 10;
  size[1] = 20;
  size[2] = 30;
  ImageType::IndexType index;
  index.Fill(0);
  region.SetSize(size);
  region.SetIndex(index);
  m_Image->SetRegions( region );
  m_Image->Allocate();
  m_Image->FillBuffer(0);

  itk::ImageRegionIterator<ImageType> it(m_Image,region);
  it.GoToBegin();

  float i = 0;
  while(!it.IsAtEnd())
    {
    it.Set(i);
    i++;
    ++it;
    }

  typedef itk::ShiftScaleImageFilter<ImageType, ImageType> ShiftType;
  ShiftType::Pointer shift = ShiftType::New();
  shift->SetInput( m_Image );
  shift->SetShift( 0.0 );
  shift->SetScale( 1.0 );
  shift->Update(); // need to update before duplicator can run
  
  std::cout << "[DONE]" << std::endl;

  // Test the duplicator
  std::cout << "Testing duplicator with float images: ";
  typedef itk::ImageDuplicator<ImageType> DuplicatorType;
  DuplicatorType::Pointer duplicator = DuplicatorType::New();

  duplicator->SetInputImage(shift->GetOutput());
  duplicator->Update();
  ImageType::Pointer ImageCopy = duplicator->GetOutput();


  itk::ImageRegionIterator<ImageType> it2(ImageCopy,ImageCopy->GetLargestPossibleRegion());
  it2.GoToBegin();

  i = 0;
  while(!it2.IsAtEnd())
    {
    if(it2.Get() != i)
      {
      std::cout << "Error: Pixel value mismatched: " << it2.Get() << " vs. " << i << std::endl;
      return EXIT_FAILURE;
      }
    i++;
    ++it2;
    }

  std::cout << "[DONE]" << std::endl;

  /** Test duplicator after modifying the bulk data of the input */
  std::cout << "Modifying input, testing duplicator again: ";
  shift->SetShift(1);
  shift->Update(); // need to update before duplicator
  duplicator->Update();
  ImageCopy = duplicator->GetOutput();
  
  itk::ImageRegionIterator<ImageType> it2b(ImageCopy,ImageCopy->GetLargestPossibleRegion());
  it2b.GoToBegin();
  i = 0;
  while(!it2b.IsAtEnd())
    {
    if(it2b.Get() != i+1)
      {
      std::cout << "Error: Pixel value mismatched: " << it2b.Get() << " vs. " << i+1 << std::endl;
      return EXIT_FAILURE;
      }
    i++;
    ++it2b;
    }

  std::cout << "[DONE]" << std::endl;

  /** Test duplicator after modifying the bulk data of the input */
  std::cout << "Rerunning duplicator with no changes: ";
  shift->Update(); // need to update before duplicator
  duplicator->Update();
  ImageCopy = duplicator->GetOutput();
  
  itk::ImageRegionIterator<ImageType> it2c(ImageCopy,ImageCopy->GetLargestPossibleRegion());
  it2c.GoToBegin();
  i = 0;
  while(!it2c.IsAtEnd())
    {
    if(it2c.Get() != i+1)
      {
      std::cout << "Error: Pixel value mismatched: " << it2c.Get() << " vs. " << i+1 << std::endl;
      return EXIT_FAILURE;
      }
    i++;
    ++it2c;
    }

  std::cout << "[DONE]" << std::endl;

  /** Create an RGB image image */
  typedef itk::Image<itk::RGBPixel<unsigned char>,3> RGBImageType;
  std::cout << "Creating simulated image: ";
  RGBImageType::Pointer m_RGBImage = RGBImageType::New();
  m_RGBImage->SetRegions( region );
  m_RGBImage->Allocate();

  itk::ImageRegionIterator<RGBImageType> it3(m_RGBImage,region);
  it3.GoToBegin();

  unsigned char r = 0;
  unsigned char g = 1;
  unsigned char b = 2;
  
  while(!it3.IsAtEnd())
    {
    itk::RGBPixel<unsigned char> pixel;
    pixel.SetRed(r);
    pixel.SetGreen(g);
    pixel.SetBlue(b);
    it3.Set(pixel);
    r++;
    if(r==255)
      {
      r=0;
      }
    g++;
    if(g==255)
      {
      g=0;
      }
    b++;
    if(b==255)
      {
      b=0;
      }
    ++it3;
    }

  std::cout << "[DONE]" << std::endl;

  // Test the duplicator

  std::cout << "Testing duplicator with RGB images: ";
  typedef itk::ImageDuplicator<RGBImageType> RGBDuplicatorType;
  RGBDuplicatorType::Pointer RGBduplicator = RGBDuplicatorType::New();

  RGBduplicator->SetInputImage(m_RGBImage);
  RGBduplicator->Update();
  RGBImageType::Pointer RGBImageCopy = RGBduplicator->GetOutput();


  itk::ImageRegionIterator<RGBImageType> it4(RGBImageCopy,RGBImageCopy->GetLargestPossibleRegion());
  it3.GoToBegin();

  r = 0;
  g = 1;
  b = 2;

  while(!it4.IsAtEnd())
    {

    itk::RGBPixel<unsigned char> pixel = it4.Get();
    if(pixel.GetRed() != r)
      {
      std::cout << "Error: Pixel R value mismatched: " << (float)pixel.GetRed()  << " vs. " << (float)r << std::endl;
      return EXIT_FAILURE;
      }
    if(pixel.GetGreen() != g)
      {
      std::cout << "Error: Pixel G value mismatched: " << (float)pixel.GetGreen()  << " vs. " << (float)g << std::endl;
      return EXIT_FAILURE;
      }
    if(pixel.GetBlue() != b)
      {
      std::cout << "Error: Pixel B value mismatched: " << (float)pixel.GetBlue()  << " vs. " << (float)b << std::endl;
      return EXIT_FAILURE;
      }
    r++;
    if(r==255)
      {
      r=0;
      }
    g++;
    if(g==255)
      {
      g=0;
      }
    b++;
    if(b==255)
      {
      b=0;
      }
    ++it4;
    }

  std::cout << "[DONE]" << std::endl;



  return EXIT_SUCCESS;

}
