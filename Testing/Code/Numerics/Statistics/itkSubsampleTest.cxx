/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkSubsampleTest.cxx
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
#include "itkSubsample.h"
#include "itkRandomImageSource.h"
#include "itkImageRegionIterator.h"
#include "itkScalarToArrayCastImageFilter.h"
#include "itkFixedArray.h"
#include "itkListSample.h"

int itkSubsampleTest(int, char* [] )
{
  std::cout << "Subsample Test \n \n";
  bool pass = true;
  std::string whereFail = "";

  typedef itk::Image< float, 3 > FloatImage;

  // Now generate a random image
  typedef itk::RandomImageSource<FloatImage> SourceType;
  SourceType::Pointer source = SourceType::New();
  unsigned long size[3] = {17, 8, 20};
  unsigned long totalSize = size[0] * size[1] * size[2];

  source->SetSize(size);
  float minValue = -100.0;
  float maxValue = 1000.0;

  source->SetMin( static_cast< FloatImage::PixelType >( minValue ) );
  source->SetMax( static_cast< FloatImage::PixelType >( maxValue ) );
  source->Update();


  // creat a new image with array pixel type from the source
  typedef itk::FixedArray< FloatImage::PixelType, 1 > ArrayPixelType;

  typedef itk::Image< ArrayPixelType, 3 > ArrayPixelImageType;
  typedef itk::Image< unsigned char, 3 >  MaskPixelImageType;

  typedef itk::ScalarToArrayCastImageFilter< FloatImage, ArrayPixelImageType >
    ImageCastFilterType;
  ImageCastFilterType::Pointer castFilter = ImageCastFilterType::New();
  castFilter->SetInput(source->GetOutput());
  castFilter->Update();

  typedef  itk::Statistics::ImageToListSampleFilter< ArrayPixelImageType, MaskPixelImageType >
    ImageToListSampleFilterType;

  ImageToListSampleFilterType::Pointer filter = ImageToListSampleFilterType::New();
  filter->SetInput(castFilter->GetOutput());

  try
    {
    filter->Update();
    }
  catch ( itk::ExceptionObject & excp )
    {
    std::cerr << "Exception caught: " << excp << std::endl;
    return EXIT_FAILURE;
    }

  typedef ImageToListSampleFilterType::ListSampleType  ListSampleType;

  typedef itk::Statistics::Subsample< ListSampleType >  SubsampleType;

  SubsampleType::Pointer subsample = SubsampleType::New();

  std::cout << subsample->GetNameOfClass() << std::endl;


  const ImageToListSampleFilterType::ListSampleType * listSample = filter->GetOutput();

  subsample->Print( std::cout );

  subsample->SetSample( listSample );

  subsample->Print( std::cout );


  //Initialize the subsample with sample
  subsample->InitializeWithAllInstances();

  //Clear and  repopulate the subsample container
  subsample->Clear();

  // add only the first half of instances of the sample
  for (ListSampleType::InstanceIdentifier id = 0;
       id < static_cast< ListSampleType::InstanceIdentifier >
         (listSample->Size() / 2);
       id++)
    {
    subsample->AddInstance(id);
    }

  // test if an exception is thrown, if a sample with id outside the
  // range of the Sample is added to the SubSample
  int idOutisdeRange = listSample->Size() + 2;

  try
    {
    subsample->AddInstance( idOutisdeRange );
    std::cerr << "Exception should have been thrown since \
      an instance outside the range of the sample container is added" << std::endl;
    return EXIT_FAILURE;
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << "Expected Exception caught: " << excp << std::endl;
    }

  typedef SubsampleType::MeasurementVectorType
                                                    MeasurementVectorType;
  try
    {
    MeasurementVectorType vec = subsample->GetMeasurementVector( idOutisdeRange );
    std::cerr << "Exception should have been thrown since \
      the id specified is outside the range of the sample container" << std::endl;
    return EXIT_FAILURE;
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << "Expected Exception caught: " << excp << std::endl;
    }

  typedef SubsampleType::AbsoluteFrequencyType            AbsoluteFrequencyType;

  try
    {
    // Purposely calling GetFrequency() method prematurely in order to trigger an exception.
    subsample->GetFrequency( idOutisdeRange );
    std::cerr << "Exception should have been thrown since \
      the id specified is outside the range of the sample container" << std::endl;
    return EXIT_FAILURE;
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << "Expected Exception caught: " << excp << std::endl;
    }

  // try swaping indices out of range
  try
    {
    subsample->Swap( 2000000,50 );
    std::cerr << "Exception should have been thrown since \
      the indices specified to be swapped are outside the range\
      of the sample container" << std::endl;
    return EXIT_FAILURE;
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << "Expected Exception caught: " << excp << std::endl;
    }

  // try accessing a measurement vector by index that is out of range
  try
    {
    unsigned int index = listSample->Size() + 2;
    MeasurementVectorType measurementVector =
            subsample->GetMeasurementVectorByIndex( index );
    std::cerr << "Exception should have been thrown since \
      the index specified is outside the range of the sample container" << std::endl;
    return EXIT_FAILURE;
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << "Expected Exception caught: " << excp << std::endl;
    }

  // try accessing a measurement vector frequency by index that is out of range
  try
    {
    unsigned int index = listSample->Size() + 2;
    SubsampleType::AbsoluteFrequencyType frequency =
            subsample->GetFrequencyByIndex( index );
    std::cout << "Frequency: " << frequency << std::endl;
    std::cerr << "Exception should have been thrown since \
      the index specified is outside the range\
      of the sample container" << std::endl;
    return EXIT_FAILURE;
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << "Expected Exception caught: " << excp << std::endl;
    }

  // try accessing a instance identifier of a measurement vector
  // using an index that is out of range
  try
    {
    unsigned int index = listSample->Size() + 2;
    ListSampleType::InstanceIdentifier id = subsample->GetInstanceIdentifier( index );
    std::cerr << "Exception should have been thrown since \
      the index specified is outside the range of the sample container" << std::endl;
    std::cout << "Instance identifier: " << id << std::endl;
    return EXIT_FAILURE;
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << "Expected Exception caught: " << excp << std::endl;
    }

  if ((totalSize / 2) != subsample->Size())
    {
    pass = false;
    whereFail = "Size()";
    }

  std::cout << subsample->GetTotalFrequency() << std::endl;

  ArrayPixelImageType::IndexType index;
  index.Fill(2);// index {2, 2, 2} = instance identifier (offset from image)
  ArrayPixelImageType::PixelType pixel = filter->GetInput()->GetPixel(index);
  ListSampleType::InstanceIdentifier ind =
    static_cast< FloatImage::OffsetValueType >(filter->GetInput()
                                               ->ComputeOffset(index));

  if (pixel[0] != subsample->GetMeasurementVector(ind)[0])
    {
    pass = false;
    whereFail = "GetMeasurementVector()";
    }

  // iterator test
  typedef itk::ImageRegionConstIterator< ArrayPixelImageType > ImageIterator;
  ImageIterator i_iter(filter->GetInput(),
                       filter->GetInput()->GetLargestPossibleRegion());

  SubsampleType::Iterator s_iter = subsample->Begin();
  unsigned int count = 0;
  while (count < subsample->Size())
    {
    if (i_iter.Get()[0] != s_iter.GetMeasurementVector()[0])
      {
      pass = false;
      whereFail = "Iterator: GetMeasurementVector()";
      }
    ++count;
    ++i_iter;
    ++s_iter;
    }

  if (s_iter != subsample->End())
    {
    pass = false;
    whereFail = "Iterator: End()";
    }

  if( !pass )
    {
    std::cout << "Test failed in " << whereFail << "." << std::endl;
    return EXIT_FAILURE;
    }

  std::cout << "Test passed." << std::endl;
  return EXIT_SUCCESS;

}
