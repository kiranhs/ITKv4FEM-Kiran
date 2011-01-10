/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkSceneSpatialObjectTest.cxx
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

/** This is a test file for the itkSceneSpatialObject class. */
#include "itkSceneSpatialObject.h"
#include "itkEllipseSpatialObject.h"
#include "itkGroupSpatialObject.h"

int itkSceneSpatialObjectTest(int, char* [])
{
  // Create the SceneSpatialObject
  typedef itk::SceneSpatialObject<3>  SceneSpatialObjectType;
  SceneSpatialObjectType::Pointer SceneSpatialObject = SceneSpatialObjectType::New();
  SceneSpatialObject->Print(std::cout);

  // Create two ellipses to put in the SceneSpatialObject
  typedef itk::EllipseSpatialObject<3>   EllipseType;
  EllipseType::Pointer ellipse1 = EllipseType::New();
  EllipseType::Pointer ellipse2 = EllipseType::New();

  SceneSpatialObject->AddSpatialObject(ellipse1);
  SceneSpatialObject->AddSpatialObject(ellipse2);

  if(SceneSpatialObject->GetNumberOfObjects() !=2 )
  {
    std::cout << "[FAILURE]" << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << "[PASSED]" << std::endl;

  SceneSpatialObject->GetMTime(); // coverage
  std::cout << SceneSpatialObject << std::endl;

  // Test spatial objects for coverage
  typedef itk::GroupSpatialObject<3> SpatialObjectType;
  SpatialObjectType::Pointer object = SpatialObjectType::New();

  std::cout << "Testing Typename: ";
  if(strcmp(object->GetTypeName(),"GroupSpatialObject"))
  {
    std::cout << "[FAILURE]" << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "[PASSED]" << std::endl;

  object->GetRequestedRegion();
  object->GetBufferedRegion();
  object->GetLargestPossibleRegion();
  object->SetBoundingBoxChildrenDepth(0);

  std::cout << "Testing  BoundingBoxChildren depth: ";
  if(object->GetBoundingBoxChildrenDepth() != 0)
  {
    std::cout << "[FAILURE]" << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "[PASSED]" << std::endl;

  std::cout << "Testing BoundingBoxChildrenName: ";
  if(object->GetBoundingBoxChildrenName() != "")
  {
    std::cout << "[FAILURE]" << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "[PASSED]" << std::endl;

  std::cout << "Testing Set/GetParentID: ";
  object->SetParentId(3);

  if(object->GetParentId() != 3)
  {
    std::cout << "[FAILURE]" << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "[PASSED]" << std::endl;

  std::cout << "Testing Set/GetSpacing: ";
  double spacing[3];
  spacing[0] = 1;
  spacing[1] = 2;
  spacing[2] = 3;
 
  object->SetSpacing(spacing);
  const double* res_spacing = object->GetSpacing();
  if((res_spacing[0] != 1) ||
     (res_spacing[1] != 2) ||
     (res_spacing[2] != 3) )
  {
    std::cout << "[FAILURE]" << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << "Testing Clear(): ";
  // Testing the clear function
  SceneSpatialObject->Clear();
  if(SceneSpatialObject->GetNumberOfObjects() != 0)
    {
    std::cout << "[FAILURE]" << std::endl;
    return EXIT_FAILURE;
    }

  SceneSpatialObject->Print(std::cout);
  std::cout << "[PASSED]" << std::endl;
  std::cout << "[DONE]" << std::endl;

  return EXIT_SUCCESS;

}
