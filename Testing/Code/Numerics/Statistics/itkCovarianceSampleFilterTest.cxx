/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkCovarianceSampleFilterTest.cxx
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

#include "itkImageToListSampleFilter.h"
#include "itkCovarianceSampleFilter.h"
#include "itkImageRegionIterator.h"
#include "itkFixedArray.h"
#include "itkVector.h"
#include "itkMeanSampleFilter.h"
#include "itkNumericTraitsFixedArrayPixel.h"

int itkCovarianceSampleFilterTest(int, char* [] )
{
  std::cout << "CovarianceSampleFilter Test \n \n";
  std::string whereFail = "";

  // Now generate an image
  enum { MeasurementVectorSize = 3 };
  typedef float MeasurementType;

  typedef itk::FixedArray< MeasurementType, MeasurementVectorSize > MeasurementVectorType;
  typedef itk::Image< MeasurementVectorType, 3 >                    ImageType;
  typedef itk::Image< unsigned char, 3 >                            MaskImageType;

  ImageType::Pointer image = ImageType::New();
  ImageType::RegionType region;
  ImageType::SizeType size;
  ImageType::IndexType index;
  index.Fill(0);
  size.Fill(5);
  region.SetIndex(index);
  region.SetSize(size);


  image->SetBufferedRegion(region);
  image->Allocate();

  typedef itk::ImageRegionIterator< ImageType > ImageIterator;
  ImageIterator iter(image, region);

  unsigned int count = 0;
  MeasurementVectorType temp;
  temp.Fill(0);

  // fill the image
  while (!iter.IsAtEnd())
    {
    temp[0] = count;
    iter.Set(temp);
    ++iter;
    ++count;
    }

  // creates an ImageToListAdaptor object
  typedef  itk::Statistics::ImageToListSampleFilter< ImageType, MaskImageType > ImageToListSampleFilterType;

  ImageToListSampleFilterType::Pointer sampleGeneratingFilter = ImageToListSampleFilterType::New();

  sampleGeneratingFilter->SetInput( image );

  try
    {
    sampleGeneratingFilter->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr<< excp << std::endl;
    return EXIT_FAILURE;
    }

  typedef ImageToListSampleFilterType::ListSampleType                 ListSampleType;
  typedef itk::Statistics::CovarianceSampleFilter< ListSampleType >   CovarianceSampleFilterType;

  CovarianceSampleFilterType::Pointer covarianceFilter = CovarianceSampleFilterType::New();

  std::cout << "GetNameOfClass() = " << covarianceFilter->GetNameOfClass() << std::endl;

  //Invoke update before adding an input. An exception should be
  try
    {
    covarianceFilter->Update();
    std::cerr << "Exception should have been thrown since \
                 Update() is invoked without setting an input " << std::endl;
    return EXIT_FAILURE;
    }
  catch ( itk::ExceptionObject & excp )
    {
    std::cerr << "Exception caught: " << excp << std::endl;
    }

  covarianceFilter->ResetPipeline();

  if ( covarianceFilter->GetInput() != NULL )
    {
    std::cerr << "GetInput() should return NULL if the input \
                     has not been set" << std::endl;
    return EXIT_FAILURE;
    }

  covarianceFilter->SetInput( sampleGeneratingFilter->GetOutput() );
  try
    {
    covarianceFilter->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr<< excp << std::endl;
    return EXIT_FAILURE;
    }

  covarianceFilter->Print( std::cout );

  const double   epsilon = 1e-6;

  // CHECK THE RESULTS
  const CovarianceSampleFilterType::MeasurementVectorDecoratedType * meanDecorator =
                                                covarianceFilter->GetMeanOutput();

  CovarianceSampleFilterType::MeasurementVectorType    mean  = meanDecorator->Get();
  std::cout << "Mean:   " << mean << std::endl;
  CovarianceSampleFilterType::MeasurementVectorType    mean2 = covarianceFilter->GetMean();

  if ( ( vcl_fabs( mean[0] - mean2[0]) > epsilon )  ||
       ( vcl_fabs( mean[1] - mean2[1]) > epsilon)  ||
       ( vcl_fabs( mean[2] - mean2[2]) > epsilon) )
    {
    std::cerr << "Mean parameter value retrieved using GetMean() and the decorator\
                  are not the same:: " <<  mean << "," << mean2 << std::endl;
    return EXIT_FAILURE;
    }


  const CovarianceSampleFilterType::MatrixDecoratedType * decorator = covarianceFilter->GetCovarianceMatrixOutput();
  CovarianceSampleFilterType::MatrixType    covarianceMatrix  = decorator->Get();

  std::cout << "Covariance matrix:   " << covarianceMatrix << std::endl;


  typedef itk::Statistics::MeanSampleFilter< ListSampleType > MeanSampleFilterType;
  MeanSampleFilterType::Pointer meanFilter = MeanSampleFilterType::New();
  meanFilter->SetInput( sampleGeneratingFilter->GetOutput());

  try
    {
    meanFilter->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << "Exception caught: " << excp << std::endl;
    }

  MeanSampleFilterType::MeasurementVectorType meanCalculatedUsingMeanSampleFilter = meanFilter->GetMean();

  if ( ( vcl_fabs( meanCalculatedUsingMeanSampleFilter[0] - mean[0]) > epsilon )  ||
       ( vcl_fabs( meanCalculatedUsingMeanSampleFilter[1] - mean[1]) > epsilon)  ||
       ( vcl_fabs( meanCalculatedUsingMeanSampleFilter[2] - mean[2]) > epsilon) )
    {
    std::cerr << "Mean calculated using the MeanSampleFilter is different from\
                 the once calculated using the covariance filter " << std::endl;
    return EXIT_FAILURE;
    }

  std::cout << "Test passed." << std::endl;
  return EXIT_SUCCESS;
}
