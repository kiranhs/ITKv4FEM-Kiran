/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkMaskNegatedImageFilterTest.cxx
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
#include "itkImage.h"

#include "itkImageRegionIteratorWithIndex.h"
#include "itkMaskNegatedImageFilter.h"

int itkMaskNegatedImageFilterTest(int, char* [] ) 
{

  // Define the dimension of the images
  const unsigned int myDimension = 3;

  // Declare the types of the images
  typedef itk::Image<float, myDimension>  myImageType1;
  typedef itk::Image<unsigned short, myDimension>  myImageType2;
  typedef itk::Image<float, myDimension>  myImageType3;

  // Declare the type of the index to access images
  typedef itk::Index<myDimension>         myIndexType;

  // Declare the type of the size 
  typedef itk::Size<myDimension>          mySizeType;

  // Declare the type of the Region
  typedef itk::ImageRegion<myDimension>        myRegionType;

  // Create two images
  myImageType1::Pointer inputImageA  = myImageType1::New();
  myImageType2::Pointer inputImageB  = myImageType2::New();
  
  // Define their size, and start index
  mySizeType size;
  size[0] = 2;
  size[1] = 2;
  size[2] = 2;

  myIndexType start;
  start[0] = 0;
  start[1] = 0;
  start[2] = 0;

  myRegionType region;
  region.SetIndex( start );
  region.SetSize( size );

  // Initialize Image A
  inputImageA->SetLargestPossibleRegion( region );
  inputImageA->SetBufferedRegion( region );
  inputImageA->SetRequestedRegion( region );
  inputImageA->Allocate();

  // Initialize Image B
  inputImageB->SetLargestPossibleRegion( region );
  inputImageB->SetBufferedRegion( region );
  inputImageB->SetRequestedRegion( region );
  inputImageB->Allocate();


  // Declare Iterator types apropriated for each image 
  typedef itk::ImageRegionIteratorWithIndex<myImageType1>  myIteratorType1;
  typedef itk::ImageRegionIteratorWithIndex<myImageType2>  myIteratorType2;
  typedef itk::ImageRegionIteratorWithIndex<myImageType3>  myIteratorType3;

  // Create one iterator for Image A (this is a light object)
  myIteratorType1 it1( inputImageA, inputImageA->GetBufferedRegion() );

  // Initialize the content of Image A
  std::cout << "First operand " << std::endl;
  while( !it1.IsAtEnd() ) 
  {
    it1.Set( 255.0 );
    std::cout << it1.Get() << std::endl;
    ++it1;
  }

  // Create one iterator for Image B (this is a light object)
  myIteratorType2 it2( inputImageB, inputImageB->GetBufferedRegion() );

  // Initialize the content of Image B
  // Set to mask first 2 pixels and last 2 pixels and leave the rest as is
  std::cout << "Second operand " << std::endl;
  for(unsigned int i = 0; i<2; ++i, ++it2) it2.Set( 0 ); 

  while( !it2.IsAtEnd() ) 
  {
    it2.Set( 3 );
    ++it2;
  }

  for(unsigned int i = 0; i< 3; ++i, --it2) it2.Set( 0 );

  it2.GoToBegin();
  while( !it2.IsAtEnd() ) 
    {
    std::cout << it2.Get() << std::endl;
    ++it2;
    }

  // Declare the type for the MaskNegated filter
  typedef itk::MaskNegatedImageFilter<
                           myImageType1,
                           myImageType2,
                           myImageType3  >       myFilterType;
            

  // Create an MaskNegated Filter                                
  myFilterType::Pointer filter = myFilterType::New();


  // Connect the input images
  filter->SetInput1( inputImageA ); 
  filter->SetInput2( inputImageB );
  filter->SetOutsideValue( 50 );

  // Get the Smart Pointer to the Filter Output 
  myImageType3::Pointer outputImage = filter->GetOutput();

  
  // Execute the filter
  filter->Update();
  filter->SetFunctor(filter->GetFunctor());

  // Create an iterator for going through the image output
  myIteratorType3 it3(outputImage, outputImage->GetBufferedRegion());
  
  //  Print the content of the result image
  std::cout << " Result " << std::endl;
  while( !it3.IsAtEnd() ) 
    {
    std::cout << it3.Get() << std::endl;
    ++it3;
    }


  filter->Print( std::cout );

  // All objects should be automatically destroyed at this point
  return EXIT_SUCCESS;

}




