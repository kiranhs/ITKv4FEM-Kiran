/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkGradientRecursiveGaussianFilterTest2.cxx
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


// Test the filter with 1-D images.


#include "itkImage.h"
#include "itkGradientRecursiveGaussianImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkCovariantVector.h"


int itkGradientRecursiveGaussianFilterTest2(int, char* [] ) 
{

  // Define the dimension of the images
  const unsigned int myDimension = 1;

  // Declare the types of the images
  typedef itk::Image<float, myDimension>           myImageType;

  // Declare the type of the index to access images
  typedef itk::Index<myDimension>             myIndexType;

  // Declare the type of the size 
  typedef itk::Size<myDimension>              mySizeType;

  // Declare the type of the Region
  typedef itk::ImageRegion<myDimension>        myRegionType;

  // Create the image
  myImageType::Pointer inputImage  = myImageType::New();

  
  // Define their size, and start index
  mySizeType size;
  size[0] = 64;

  myIndexType start;
  start.Fill(0);

  myRegionType region;
  region.SetIndex( start );
  region.SetSize( size );

  // Initialize Image A
  inputImage->SetLargestPossibleRegion( region );
  inputImage->SetBufferedRegion( region );
  inputImage->SetRequestedRegion( region );
  inputImage->Allocate();

  // Declare Iterator type for the input image
  typedef itk::ImageRegionIteratorWithIndex<myImageType>  myIteratorType;

  // Create one iterator for the Input Image A (this is a light object)
  myIteratorType it( inputImage, inputImage->GetRequestedRegion() );

  // Initialize the content of Image A
  while( !it.IsAtEnd() ) 
    {
    it.Set( 0.0 );
    ++it;
    }

  size[0] = 32;

  start[0] = 16;

  // Create one iterator for an internal region
  region.SetSize( size );
  region.SetIndex( start );
  myIteratorType itb( inputImage, region );

  // Initialize the content the internal region
  while( !itb.IsAtEnd() ) 
    {
    itb.Set( 100.0 );
    ++itb;
    }

  // Declare the type for the 
  typedef itk::GradientRecursiveGaussianImageFilter< 
                                            myImageType >  myFilterType;
            
  typedef myFilterType::OutputImageType myGradientImageType;


  // Create a  Filter                                
  myFilterType::Pointer filter = myFilterType::New();


  // Connect the input images
  filter->SetInput( inputImage ); 

  // Select the value of Sigma
  filter->SetSigma( 2.5 ); 

  
  // Execute the filter
  filter->Update();

  // Get the Smart Pointer to the Filter Output 
  // It is important to do it AFTER the filter is Updated
  // Because the object connected to the output may be changed
  // by another during GenerateData() call
  myGradientImageType::Pointer outputImage = filter->GetOutput();

  // Declare Iterator type for the output image
  typedef itk::ImageRegionIteratorWithIndex<
                                 myGradientImageType>  myOutputIteratorType;

  // Create an iterator for going through the output image
  myOutputIteratorType itg( outputImage, 
                            outputImage->GetRequestedRegion() );
  
  //  Print the content of the result image
  std::cout << " Result " << std::endl;
  itg.GoToBegin();
  while( !itg.IsAtEnd() ) 
    {
    std::cout << itg.Get();
    ++itg;
    }


  // All objects should be automatically destroyed at this point
  return EXIT_SUCCESS;

}




