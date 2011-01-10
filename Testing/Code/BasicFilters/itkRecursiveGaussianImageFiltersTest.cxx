/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkRecursiveGaussianImageFiltersTest.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

// Disable warning for long symbol names in this file only
#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif


#include <itkImage.h>
#include <itkRecursiveGaussianImageFilter.h>
#include <itkImageRegionIteratorWithIndex.h>
#include <itkImageRegionConstIterator.h>


int itkRecursiveGaussianImageFiltersTest(int, char* [] ) 
{

  {  // 3D test

  // Define the dimension of the images
  const unsigned int myDimension = 3;

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
  size[0] = 100;
  size[1] = 100;
  size[2] = 100;

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

  // Declare Iterator types apropriated for each image 
  typedef itk::ImageRegionIteratorWithIndex<myImageType>  myIteratorType;


  // Create one iterator for the Input Image A (this is a light object)
  myIteratorType it( inputImage, inputImage->GetRequestedRegion() );

  // Initialize the content of Image A
  std::cout << "Input Image initialization " << std::endl;
  while( !it.IsAtEnd() ) 
  {
    it.Set( 0.0 );
    ++it;
  }

  size[0] = 60;
  size[1] = 60;
  size[2] = 60;

  start[0] = 20;
  start[1] = 20;
  start[2] = 20;

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




  // Declare the type for the  Gaussian  filter
  typedef itk::RecursiveGaussianImageFilter<
                                              myImageType,
                                              myImageType
                                                        >  myGaussianFilterType;
            

  // Create a  Filter                                
  myGaussianFilterType::Pointer filter = myGaussianFilterType::New();


  // Connect the input images
  filter->SetInput( inputImage ); 
  filter->SetDirection( 2 );  // apply along Z
  filter->SetOrder( myGaussianFilterType::ZeroOrder );

  
  // Execute the filter
  std::cout << "Executing Smoothing filter...";
  filter->Update();
  std::cout << " Done !" << std::endl;


  // Create a  Filter                                
  myGaussianFilterType::Pointer filter1 = myGaussianFilterType::New();


  // Connect the input images
  filter1->SetInput( inputImage ); 
  filter1->SetDirection( 2 );  // apply along Z
  filter1->SetOrder( myGaussianFilterType::FirstOrder );

  
  // Execute the filter1
  std::cout << "Executing First Derivative filter...";
  filter1->Update();
  std::cout << " Done !" << std::endl;



  // Create a  Filter                                
  myGaussianFilterType::Pointer filter2 = myGaussianFilterType::New();


  // Connect the input images
  filter2->SetInput( inputImage ); 
  filter2->SetDirection( 2 );  // apply along Z
  filter2->SetOrder( myGaussianFilterType::SecondOrder );
  
  // Execute the filter2
  std::cout << "Executing Second Derivative filter...";
  filter2->Update();
  std::cout << " Done !" << std::endl;
  }


 
  { // Test normalizations factors using a 1D image
  std::cout << "Test normalizations factors using a 1-D image" << std::endl;

  typedef float                         PixelType;
  typedef itk::Image< PixelType, 1 >    ImageType;

  typedef ImageType::SizeType           SizeType;
  typedef ImageType::IndexType          IndexType;
  typedef ImageType::RegionType         RegionType;
  typedef ImageType::SpacingType        SpacingType;

  typedef itk::NumericTraits< PixelType >::RealType    PixelRealType;

  SizeType size;
  size[0] = 21;

  IndexType start;
  start[0] = 0;

  RegionType region;
  region.SetIndex( start );
  region.SetSize( size );

  SpacingType spacing;
  spacing[0] = 1.0;

  ImageType::Pointer inputImage = ImageType::New();
  inputImage->SetRegions( region );
  inputImage->Allocate();
  inputImage->SetSpacing( spacing );
  inputImage->FillBuffer( itk::NumericTraits< PixelType >::Zero );

  IndexType index;
  index[0] = ( size[0] - 1 ) / 2;  // the middle pixel
  
  inputImage->SetPixel( index, static_cast< PixelType >( 1000.0 ) );

  typedef itk::RecursiveGaussianImageFilter< 
                            ImageType, ImageType > FilterType;

  FilterType::Pointer filter = FilterType::New();

  filter->SetInput( inputImage );

  std::cout << "Testing normalization across scales...  ";
  { // begin of test for normalization across scales
    
    filter->SetNormalizeAcrossScale( true );

    const double sigmaA = 2.0;
    filter->SetSigma( sigmaA );
    filter->Update();

    const PixelType valueA = filter->GetOutput()->GetPixel( index );

    const double sigmaB = 4.0;
    filter->SetSigma( sigmaB );

    filter->Update();

    const PixelType valueB = filter->GetOutput()->GetPixel( index );

    if( vcl_fabs( (valueB - valueA) / valueA ) > 1e-4 )
      {
      std::cout << "FAILED !" << std::endl;
      std::cerr << "Error, Normalization across scales is failing" << std::endl;
      std::cerr << "Central pixel at sigma = " << sigmaA << " = " << valueA << std::endl;
      std::cerr << "Central pixel at sigma = " << sigmaB << " = " << valueB << std::endl;
      return EXIT_FAILURE;
      }
    else
      {
      std::cout << "PASSED !" << std::endl;
      }

    } // end of test for normalization across scales

    std::cout << "Testing derivatives normalization " << std::endl;

    { // begin of test for normalization among derivatives
    filter->SetNormalizeAcrossScale( false );

    // Since one side of the Gaussian is monotonic we can 
    // use the middle-value theorem: The value of the derivative at
    // index[0] - 2 must be bounded by the estimation of the derivative
    // at index[0] -1 and index[0] -3. In the following we compute an
    // estimation of derivatives by partial differences at this two 
    // positions and use them as bounds for the value of the first order
    // derivative returned by the filter.
    
    const double sigmaC = 3.0;
    filter->SetSigma( sigmaC );

    filter->SetZeroOrder();
    filter->Update();

    index[0] = ( size[0] - 1 ) / 2;  // the middle pixel
    const PixelRealType valueA = filter->GetOutput()->GetPixel( index ); 

    index[0] -= 2;
    const PixelRealType valueB = filter->GetOutput()->GetPixel( index ); 

    index[0] -= 2;
    const PixelRealType valueC = filter->GetOutput()->GetPixel( index ); 

    const PixelRealType derivativeLowerBound = ( valueA - valueB ) / 2.0;
    const PixelRealType derivativeUpperBound = ( valueB - valueC ) / 2.0;

    // Now let's get the first derivative value computed by the filter
    filter->SetFirstOrder();
    filter->Update();


    index[0] = ( size[0] - 1 ) / 2;  // the middle pixel
    index[0] -= 2;

    const PixelRealType derivativeValue = filter->GetOutput()->GetPixel( index ); 

    std::cout << "   first derivative normalization...  ";
    if( ( derivativeLowerBound > derivativeValue ) || 
        ( derivativeUpperBound < derivativeValue )  )
      {
      std::cout << "FAILED !" << std::endl;
      std::cerr << "The value of the first derivative at index " << index[0] << std::endl;
      std::cerr << "is = " << derivativeValue << std::endl;
      std::cerr << "which is outside the bounds = [ " << derivativeLowerBound;
      std::cerr << " : " << derivativeUpperBound << " ] " << std::endl;
      return EXIT_FAILURE;
      }
    else 
      {
      std::cout << "PASSED !" << std::endl;
      }


    // Now do the similar testing between First Derivative and Second 
    // derivative.
    filter->SetFirstOrder();
    filter->Update();

    index[0] = ( size[0] - 1 ) / 2;  // the middle pixel 
    const PixelRealType value1A = filter->GetOutput()->GetPixel( index ); 

    index[0] -= 2;
    const PixelRealType value1B = filter->GetOutput()->GetPixel( index ); 

    index[0] -= 2;
    const PixelRealType value1C = filter->GetOutput()->GetPixel( index ); 

    // NOTE that the second derivative in this region is monotonic but decreasing.
    const PixelRealType secondDerivativeLowerBound = ( value1A - value1B ) / 2.0;
    const PixelRealType secondDerivativeUpperBound = ( value1B - value1C ) / 2.0;

    // Now let's get the second derivative value computed by the filter
    filter->SetSecondOrder();
    filter->Update();


    index[0] = (( size[0] - 1 ) / 2) - 2; // where to sample the second derivative

    const PixelRealType secondDerivativeValue = filter->GetOutput()->GetPixel( index ); 

    std::cout << "   second derivative normalization...  ";
    if( ( secondDerivativeLowerBound > secondDerivativeValue ) || 
        ( secondDerivativeUpperBound < secondDerivativeValue )  )
      {
      std::cout << "FAILED !" << std::endl;
      std::cerr << "The value of the second derivative at index " << index[0] << std::endl;
      std::cerr << "is = " << secondDerivativeValue << std::endl;
      std::cerr << "which is outside the bounds = [ " << secondDerivativeLowerBound;
      std::cerr << " : " << secondDerivativeUpperBound << " ] " << std::endl;
      return EXIT_FAILURE;
      }
    else 
      {
      std::cout << "PASSED !" << std::endl;
      }


    } // end of test for normalization among derivatives
  

    
  // Print out all the values for the zero, first and second order
  filter->SetNormalizeAcrossScale( false );
  filter->SetSigma( 2.0 );

  ImageType::ConstPointer outputImage = filter->GetOutput();
  typedef itk::ImageRegionConstIterator< ImageType > IteratorType;
  IteratorType  it( outputImage, outputImage->GetBufferedRegion() );

  std::cout << std::endl << std::endl;
  std::cout << "Smoothed image " << std::endl;
  filter->SetZeroOrder();
  filter->Update();
  it.GoToBegin();
  while( ! it.IsAtEnd() )
    {
    std::cout << it.Get() << std::endl;
    ++it;
    }

  // Now compute the first derivative
  std::cout << std::endl << std::endl;
  std::cout << "First Derivative " << std::endl;
  filter->SetFirstOrder();
  filter->Update();
  it.GoToBegin();
  while( ! it.IsAtEnd() )
    {
    std::cout << it.Get() << std::endl;
    ++it;
    }


  // Now compute the first derivative
  std::cout << std::endl << std::endl;
  std::cout << "Second Derivative " << std::endl;
  filter->SetSecondOrder();
  filter->Update();
  it.GoToBegin();
  while( ! it.IsAtEnd() )
    {
    std::cout << it.Get() << std::endl;
    ++it;
    }


  }

  
  // All objects should be automatically destroyed at this point
  return EXIT_SUCCESS;

}




