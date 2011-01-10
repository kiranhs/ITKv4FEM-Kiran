/*=========================================================================

Program:   Insight Segmentation & Registration Toolkit
Module:    itkSampleClassifierFilterTest1.cxx
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

#include "itkArray.h"
#include "itkVariableLengthVector.h"
#include "itkListSample.h"
#include "itkSampleClassifierFilter.h"
#include "itkMaximumDecisionRule2.h"
#include "itkDistanceToCentroidMembershipFunction.h"
#include "itkMeasurementVectorTraits.h"


// ADD DistanceToCentroidMembershipFunction (with the added SetDistanceMetric() method
// ADD EuclideanDistanceMetri
// Create two classes with their respective DistanceToCentroidMembershipFunction and two separate centroids
// ADD MinimumDecisionRule
// Run that classification.

int itkSampleClassifierFilterTest1( int, char * [] )
{

  const unsigned int numberOfComponents = 3;
  typedef float      MeasurementType;

  const unsigned int numberOfClasses = 3;

  typedef itk::Array< MeasurementType > MeasurementVectorType;
  typedef itk::Statistics::ListSample< MeasurementVectorType > SampleType;

  typedef itk::Statistics::SampleClassifierFilter< SampleType > FilterType;

  FilterType::Pointer filter = FilterType::New();

  SampleType::Pointer sample = SampleType::New();
  sample->SetMeasurementVectorSize( numberOfComponents );

  // Test GetInput() before setting the input
  if( filter->GetInput() != NULL )
    {
    std::cerr << "GetInput() should have returned NULL" << std::endl;
    return EXIT_FAILURE;
    }

  // Test GetOutput() before creating the output
  if( filter->GetOutput() == NULL )
    {
    std::cerr << "GetOutput() should have returned NON-NULL" << std::endl;
    return EXIT_FAILURE;
    }

  //Add measurement vectors
  MeasurementVectorType v1(numberOfComponents);
  v1[0] = 0;
  v1[1] = 0;
  v1[2] = 0;
  sample->PushBack( v1 );

  MeasurementVectorType v2(numberOfComponents);
  v2[0] = 1;
  v2[1] = 1;
  v2[2] = 1;
  sample->PushBack( v2 );

  MeasurementVectorType v3(numberOfComponents);
  v3[0] = 2;
  v3[1] = 2;
  v3[2] = 2;
  sample->PushBack( v3 );


  filter->SetInput( sample );

  if( filter->GetInput() != sample.GetPointer() )
    {
    std::cerr << "GetInput() didn't matched SetInput()" << std::endl;
    return EXIT_FAILURE;
    }

  filter->SetNumberOfClasses( numberOfClasses );

  if( filter->GetNumberOfClasses() != numberOfClasses )
    {
    std::cerr << "GetNumberOfClasses() didn't matched SetNumberOfClasses()" << std::endl;
    return EXIT_FAILURE;
    }

  typedef FilterType::ClassLabelVectorObjectType               ClassLabelVectorObjectType;
  typedef FilterType::ClassLabelVectorType                     ClassLabelVectorType;
  typedef FilterType::MembershipFunctionVectorObjectType       MembershipFunctionVectorObjectType;
  typedef FilterType::MembershipFunctionVectorType             MembershipFunctionVectorType;

  typedef itk::Statistics::DistanceToCentroidMembershipFunction< MeasurementVectorType >
                                                               MembershipFunctionType;

  typedef MembershipFunctionType::Pointer                      MembershipFunctionPointer;

  ClassLabelVectorObjectType::Pointer  classLabelsObject = ClassLabelVectorObjectType::New();
  filter->SetClassLabels( classLabelsObject );

  MembershipFunctionVectorObjectType::Pointer membershipFunctionsObject =
                                        MembershipFunctionVectorObjectType::New();
  filter->SetMembershipFunctions( membershipFunctionsObject );

  //Run the filter without specifiying any membership functions. An exception
  //should be thrown since there will be a mismatch between the number of classes
  //and membership functions
  try
    {
    filter->Update();
    std::cerr << "Attempting to run a classification with unequal"
              << " number of membership functions and number of classes,"
              << " should throw an exception" << std::endl;
    return EXIT_FAILURE;
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    }


  // Add three membership functions and rerun the filter
  MembershipFunctionVectorType &  membershipFunctionsVector = membershipFunctionsObject->Get();

  MembershipFunctionPointer membershipFunction1 = MembershipFunctionType::New();
  membershipFunction1->SetMeasurementVectorSize( numberOfComponents );
  MembershipFunctionType::CentroidType    centroid1;
  itk::Statistics::MeasurementVectorTraits::SetLength( centroid1, numberOfComponents );
  membershipFunction1->SetCentroid( centroid1 );
  membershipFunctionsVector.push_back( membershipFunction1.GetPointer() );

  MembershipFunctionPointer membershipFunction2 = MembershipFunctionType::New();
  membershipFunction1->SetMeasurementVectorSize( numberOfComponents );
  MembershipFunctionType::CentroidType    centroid2;
  itk::Statistics::MeasurementVectorTraits::SetLength( centroid2, numberOfComponents );
  membershipFunction2->SetCentroid( centroid2 );
  membershipFunctionsVector.push_back( membershipFunction2.GetPointer() );

  MembershipFunctionPointer membershipFunction3 = MembershipFunctionType::New();
  membershipFunction3->SetMeasurementVectorSize( numberOfComponents );
  MembershipFunctionType::CentroidType    centroid3;
  itk::Statistics::MeasurementVectorTraits::SetLength( centroid3, numberOfComponents );
  membershipFunction3->SetCentroid( centroid3 );
  membershipFunctionsVector.push_back( membershipFunction3.GetPointer() );

  try
    {
    filter->Update();
    std::cerr << "Attempting to run a classification with unequal"
              << " number of class labels and number of classes,"
              << " should throw an exception" << std::endl;
    return EXIT_FAILURE;
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    }


  // Add three class labels and rerun the filter
  ClassLabelVectorType & classLabelVector  = classLabelsObject->Get();

  typedef FilterType::ClassLabelType        ClassLabelType;

  ClassLabelType  class1 = 0;
  classLabelVector.push_back( class1 );

  ClassLabelType  class2 = 1;
  classLabelVector.push_back( class2 );

  ClassLabelType  class3 = 2;
  classLabelVector.push_back( class3 );


  //Run the filter without setting a decision rule. An exception should be
  //thrown
  try
    {
    filter->Update();
    std::cerr << "Attempting to run a classification without setting"
              << "decision rule, should throw an exception" << std::endl;
    return EXIT_FAILURE;
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    }

  //Set a decision rule type
  typedef itk::Statistics::MaximumDecisionRule2  DecisionRuleType;

  DecisionRuleType::Pointer    decisionRule = DecisionRuleType::New();
  filter->SetDecisionRule( decisionRule );

  if( filter->GetDecisionRule() != decisionRule )
    {
    std::cerr << "Get/Set Decision rule error! " << std::endl;
    return EXIT_FAILURE;
    }


  try
    {
    filter->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }

  // Test GetOutput() after creating the output
  if( filter->GetOutput() == NULL )
    {
    std::cerr << "GetOutput() should have returned NON-NULL" << std::endl;
    return EXIT_FAILURE;
    }

  std::cout << "Test passed." << std::endl;
  return EXIT_SUCCESS;
}
