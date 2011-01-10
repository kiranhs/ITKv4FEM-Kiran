/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkExpImageFilterAndAdaptorTest.cxx
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


#include <itkImage.h>
#include <itkExpImageFilter.h>
#include <itkExpImageAdaptor.h>
#include <itkImageRegionIteratorWithIndex.h>
#include <itkSubtractImageFilter.h>


int itkExpImageFilterAndAdaptorTest(int, char* [] ) 
{

  // Define the dimension of the images
  const unsigned int ImageDimension = 3;

  // Declare the types of the images
  typedef itk::Image<float, ImageDimension>  InputImageType;
  typedef itk::Image<float, ImageDimension>  OutputImageType;

  
  
  // Declare Iterator types apropriated for each image 
  typedef itk::ImageRegionIteratorWithIndex<
                                  InputImageType>  InputIteratorType;

  typedef itk::ImageRegionIteratorWithIndex<
                                  OutputImageType>  OutputIteratorType;



  // Declare the type of the index to access images
  typedef itk::Index<ImageDimension>         IndexType;

  // Declare the type of the size 
  typedef itk::Size<ImageDimension>          SizeType;

  // Declare the type of the Region
  typedef itk::ImageRegion<ImageDimension>   RegionType;

  // Create two images
  InputImageType::Pointer inputImage  = InputImageType::New();
  
  // Define their size, and start index
  SizeType size;
  size[0] = 2;
  size[1] = 2;
  size[2] = 2;

  IndexType start;
  start[0] = 0;
  start[1] = 0;
  start[2] = 0;

  RegionType region;
  region.SetIndex( start );
  region.SetSize( size );

  // Initialize Image A
  inputImage->SetLargestPossibleRegion( region );
  inputImage->SetBufferedRegion( region );
  inputImage->SetRequestedRegion( region );
  inputImage->Allocate();
  // Create one iterator for the Input Image (this is a light object)
  InputIteratorType it( inputImage, inputImage->GetBufferedRegion() );

  // Initialize the content of Image A
  const double value = vnl_math::pi / 6.0;
  std::cout << "Content of the Input " << std::endl;
  it.GoToBegin();
  while( !it.IsAtEnd() ) 
    {
    it.Set( value );
    std::cout << it.Get() << std::endl;
    ++it;
    }

  // Declare the type for the Exp filter
  typedef itk::ExpImageFilter< InputImageType,
                               OutputImageType  >  FilterType;
            

  // Create an ADD Filter                                
  FilterType::Pointer filter = FilterType::New();


  // Connect the input images
  filter->SetInput( inputImage ); 
  filter->SetNumberOfThreads(1);

  // Get the Smart Pointer to the Filter Output 
  OutputImageType::Pointer outputImage = filter->GetOutput();

  
  // Execute the filter
  filter->Update();
  filter->SetFunctor(filter->GetFunctor());

  // Create an iterator for going through the image output
  OutputIteratorType ot(outputImage, outputImage->GetRequestedRegion());
  
  //  Check the content of the result image
  std::cout << "Verification of the output " << std::endl;
  const OutputImageType::PixelType epsilon = 1e-6;
  ot.GoToBegin();
  it.GoToBegin();
  while( !ot.IsAtEnd() ) 
    {
    std::cout <<  ot.Get() << " = ";
    std::cout <<  vcl_exp( it.Get() )  << std::endl; 
    const InputImageType::PixelType  input  = it.Get();
    const OutputImageType::PixelType output = ot.Get();
    const OutputImageType::PixelType exponential  = vcl_exp(input);
    if( vcl_fabs( exponential - output ) > epsilon )
      {
      std::cerr << "Error in itkExpImageFilterTest " << std::endl;
      std::cerr << " vcl_exp( " << input << ") = " << exponential << std::endl;
      std::cerr << " differs from " << output;
      std::cerr << " by more than " << epsilon << std::endl;
      return EXIT_FAILURE;
      }
    ++ot;
    ++it;
    }


  //---------------------------------------
  // This section tests for ExpImageAdaptor
  //---------------------------------------

  typedef itk::ExpImageAdaptor<InputImageType,
                          OutputImageType::PixelType>  AdaptorType;

  AdaptorType::Pointer expAdaptor = AdaptorType::New();

  expAdaptor->SetImage( inputImage );

  typedef itk::SubtractImageFilter<
                        OutputImageType,
                        AdaptorType,
                        OutputImageType   > DiffFilterType;

  DiffFilterType::Pointer diffFilter = DiffFilterType::New();

  diffFilter->SetInput1( outputImage );
  diffFilter->SetInput2( expAdaptor  );

  diffFilter->Update();

  // Get the Smart Pointer to the Diff filter Output
  OutputImageType::Pointer diffImage = diffFilter->GetOutput();

  //  Check the content of the diff image
  std::cout << "Comparing the results with those of an Adaptor" << std::endl;
  std::cout << "Verification of the output " << std::endl;

  // Create an iterator for going through the image output
  OutputIteratorType dt(diffImage, diffImage->GetRequestedRegion());
  
  dt.GoToBegin();
  while( !dt.IsAtEnd() ) 
    {
    std::cout <<  dt.Get() << std::endl;
    const OutputImageType::PixelType diff = dt.Get();
    if( vcl_fabs( diff ) > epsilon )
      {
      std::cerr << "Error in itkExpImageFilterTest " << std::endl;
      std::cerr << "Comparing results with Adaptors" << std::endl;
      std::cerr << " difference = " << diff << std::endl;
      std::cerr << " differs from 0 ";
      std::cerr << " by more than " << epsilon << std::endl;
      return EXIT_FAILURE;
      }
    ++dt;
    }

  
  return EXIT_SUCCESS;
}
