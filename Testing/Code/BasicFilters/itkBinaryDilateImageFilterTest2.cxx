/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkBinaryDilateImageFilterTest2.cxx
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


#include <stdlib.h>
#include <itkImage.h>
#include <itkFastIncrementalBinaryDilateImageFilter.h>
#include <itkBinaryCrossStructuringElement.h>
#include <itkImageRegionIterator.h>
#include <itkExceptionObject.h>
#include "itkFilterWatcher.h"

int itkBinaryDilateImageFilterTest2(int, char* [] ) 
{
  unsigned int i;
  
  // Define the dimension of the images
  const unsigned int myDimension = 2;

  // Define the values of the input images
  const unsigned short fgValue = 1;
  const unsigned short bgValue = 0;

  // Declare the types of the images
  typedef itk::Image<unsigned short, myDimension>  myImageType;

  // Declare the type of the index to access images
  typedef itk::Index<myDimension>         myIndexType;

  // Declare the type of the size 
  typedef itk::Size<myDimension>          mySizeType;

  // Declare the type of the Region
  typedef itk::ImageRegion<myDimension>        myRegionType;

  // Create an image
  myImageType::Pointer inputImage  = myImageType::New();
  
  // Define their size, and start index
  mySizeType size;
  size[0] = 20;
  size[1] = 20;

  myIndexType start;
  start[0] = 0;
  start[1] = 0;

  myRegionType region;
  region.SetIndex( start );
  region.SetSize( size );

  // Initialize Image
  inputImage->SetRegions( region );
  inputImage->Allocate();

  // Declare Iterator types apropriated for each image 
  typedef itk::ImageRegionIterator<myImageType>  myIteratorType;

  // Create one iterator for image (this is a light object)
  myIteratorType it( inputImage, inputImage->GetBufferedRegion() );

  // Initialize the content of Image
  std::cout << "Input image " << std::endl;
  inputImage->FillBuffer(bgValue);

  myImageType::IndexType ind;
  ind[0] = 10;
  ind[1] = 10;
  inputImage->SetPixel(ind, fgValue);

  ind[0] = 0;
  ind[1] = 0;
  inputImage->SetPixel(ind, fgValue);
  ind[0] = 19;
  ind[1] = 0;
  inputImage->SetPixel(ind, fgValue);
  ind[0] = 19;
  ind[1] = 19;
  inputImage->SetPixel(ind, fgValue);
  ind[0] = 0;
  ind[1] = 19;
  inputImage->SetPixel(ind, fgValue);

  ind[0] = 0;
  ind[1] = 10;
  inputImage->SetPixel(ind, fgValue);
  ind[0] = 10;
  ind[1] = 0;
  inputImage->SetPixel(ind, fgValue);
  ind[0] = 19;
  ind[1] = 10;
  inputImage->SetPixel(ind, fgValue);
  ind[0] = 10;
  ind[1] = 19;
  inputImage->SetPixel(ind, fgValue);

  i = 0;
  it.GoToBegin();
  while ( !it.IsAtEnd() )
    {
    std::cout << it.Get() << "  ";
    ++it;

    if (++i % 20 == 0)
      {
      std::cout << std::endl;
      }
    }
  
  // Declare the type for the structuring element
  typedef itk::BinaryCrossStructuringElement<unsigned short, myDimension>
    myKernelType;
  
  // Declare the type for the morphology Filter
  typedef itk::FastIncrementalBinaryDilateImageFilter<myImageType, myImageType, myKernelType>
    myFilterType;

  // Create the filter
  myFilterType::Pointer filter = myFilterType::New();
  //FilterWatcher filterWatcher(filter);

  // Create the structuring element
  myKernelType ball;
  ball.CreateStructuringElement();
  
  // Connect the input image
  filter->SetInput( inputImage );
  filter->SetKernel( ball );
  filter->SetDilateValue( fgValue );
  
  // Get the Smart Pointer to the Filter Output 
  myImageType::Pointer outputImage = filter->GetOutput();


  // Test the itkGetMacro
  unsigned short value = filter->GetDilateValue();
  std::cout << "filter->GetDilateValue(): " << value << std::endl;

  // Execute the filter
  try
    {
    filter->Update();
    // Create an iterator for going through the image output
    myIteratorType it2(outputImage, outputImage->GetBufferedRegion());
  
    //  Print the content of the result image
    std::cout << "Result " << std::endl;
    i=0;
    while( !it2.IsAtEnd() ) 
      {
      std::cout << it2.Get() << "  ";
      ++it2;
    
      if (++i % 20 == 0)
        {
        std::cout << std::endl;
        }
      }
   }

  catch (itk::ExceptionObject& e)
    {
    std::cerr << "Exception caught during filter Update\n"  << e;
    return -1;
    }

  // All objects should be automatically destroyed at this point

  return EXIT_SUCCESS;

}




