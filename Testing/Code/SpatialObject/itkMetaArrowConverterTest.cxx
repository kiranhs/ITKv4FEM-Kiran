/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkMetaArrowConverterTest.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  
  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#include "itkMetaArrowConverter.h"
#include "itkGroupSpatialObject.h"
#include <iostream>

/**
 * This is a test file for the itkMetaArrowConverter class.
 * 
 * 
 * This test creates a sample ArrowSpatialObject and a sample MetaArrow and
 * Converts between the two, testing for conversion completeness.  At the 
 * moment, this means testing to make sure length and color are properly 
 * converted, but it may also include testing direction and position.  The 
 * test also runs the read and write methods to test them.
 * 
 * Notes: Parent cannot be converted from MetaObject to SpatialObject since
 *        MetaObject only holds a parent id rather than a parent object.  
 *        Only the ParentID can be properly converted.
 */
int itkMetaArrowConverterTest(int ac, char* av[])
{
    
  // check number of arguments
  if (ac != 2)
    {
    std::cout << "Must specify output path as argument" << std::endl;
    return EXIT_FAILURE;
    }
  
  
  // typedefs
  const unsigned int Dimensions = 3;
  typedef itk::ArrowSpatialObject<Dimensions> SpatialObjectType;
  typedef itk::GroupSpatialObject<Dimensions> SpatialObjectParentType;
  typedef itk::MetaArrowConverter<Dimensions> ConverterType;
  
  // instantiate new converter and object (I don't think I'm supposed to do it like this in ITK!!!)
  ConverterType* converter = new ConverterType();
  
  
  //
  // create the test data
  //
  
  // direction
  SpatialObjectType::VectorType direction;
  direction[0] = 0;
  direction[1] = 1;
  direction[2] = 2;
  double mDirection[3];
  mDirection[0] = 0;
  mDirection[1] = 1;
  mDirection[2] = 2;
  
  // position
  SpatialObjectType::PointType position;
  position[0] = -1;
  position[1] = -2;
  position[2] = -3;
  double mPosition[3];
  mPosition[0] = -1;
  mPosition[1] = -2;
  mPosition[2] = -3;
  
  // length
  double length = 2.3;
  
  // color
  float color[4];
  color[0] = 1;
  color[1] = .5;
  color[2] = .25;
  color[3] = 1;
  
  // set up itkArrow
  SpatialObjectType::Pointer itkArrow = SpatialObjectType::New();
  itkArrow->SetDirection(direction);
  itkArrow->SetPosition(position);
  itkArrow->SetLength(length);
  itkArrow->GetProperty()->SetRed(color[0]);
  itkArrow->GetProperty()->SetGreen(color[1]);
  itkArrow->GetProperty()->SetBlue(color[2]);
  itkArrow->GetProperty()->SetAlpha(color[3]);
  SpatialObjectParentType::Pointer itkParent = SpatialObjectParentType::New();
  itkParent->SetId(1);
  itkParent->AddSpatialObject(itkArrow);
  
  // set up metaArrow
  MetaArrow* metaArrow = new MetaArrow(Dimensions);
  metaArrow->Length((float)length);
  metaArrow->Position((const double*)mPosition);
  metaArrow->Direction((const double*)mDirection);
  metaArrow->Color((const float*)color);
  metaArrow->ParentID(itkParent->GetId());
  
  // precision limit for comparing floats and doubles
  double precisionLimit = .000001;
  
  
  //
  // test itk to metaArrow
  //
  MetaArrow* newMetaArrow = converter->ArrowSpatialObjectToMetaArrow(itkArrow);
  
  // check length
  double metaLength = newMetaArrow->Length();
  
  //if (metaLength != (float)length)
  if (vcl_fabs(metaLength - length) > precisionLimit)
    {
    std::cout << "Conversion to MetaArrow failed to convert length [FAILED]" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] SpatialObject -> MetaObject: length" << std::endl;
  
  // check color
  const float* newMetaColor = newMetaArrow->Color();
  if (newMetaColor[0] != color[0] || newMetaColor[1] != color[1] || 
      newMetaColor[2] != color[2] || newMetaColor[3] != color[3])
    {
    std::cout << "Conversion to MetaArrow failed to convert color [FAILED]" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] SpatialObject -> MetaObject: color" << std::endl;
  
  // check parent id
  if (newMetaArrow->ParentID() != itkArrow->GetParent()->GetId())
    {
    std::cout << "Conversion to MetaArrow failed to convert parent [FAILED]" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] SpatialObject -> MetaObject: parent id" << std::endl;
  
  // check position
  const double* metaPosition = newMetaArrow->Position();
  if (vcl_fabs(metaPosition[0] - position[0]) > precisionLimit ||
      vcl_fabs(metaPosition[1] - position[1]) > precisionLimit || 
      vcl_fabs(metaPosition[2] - position[2]) > precisionLimit)
    {
    std::cout << "Conversion to MetaArrow failed to convert position [FAILED]" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] SpatialObject -> MetaObject: position" << std::endl;
  
  // check direction (note: need to normalize before comparing)
  SpatialObjectType::VectorType directionNorm = direction;
  directionNorm.Normalize();
  const double* newMetaDirection = newMetaArrow->Direction();
  SpatialObjectType::VectorType newMetaDirectionNorm;
  newMetaDirectionNorm[0] = newMetaDirection[0];
  newMetaDirectionNorm[1] = newMetaDirection[1];
  newMetaDirectionNorm[2] = newMetaDirection[2];
  
  // normalize if the vector isn't all zeros
  if (newMetaDirection[0] != 0 || newMetaDirection[1] != 0 || newMetaDirection[2] != 0)
    {
    newMetaDirectionNorm.Normalize();
    }
  
  if (vcl_fabs(newMetaDirectionNorm[0] - directionNorm[0]) > precisionLimit
      || vcl_fabs(newMetaDirectionNorm[1] - directionNorm[1]) > precisionLimit
      || vcl_fabs(newMetaDirectionNorm[2] - directionNorm[2])  > precisionLimit)
    {
    std::cout << "Conversion to SpatialObject failed to convert direction [FAILED]" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] SpatialObject -> MetaObject: direction" << std::endl;
  
  
  //
  // test metaArrow to itk
  //  
  SpatialObjectType::Pointer newItkArrow = converter->MetaArrowToArrowSpatialObject(metaArrow);
  
  // check length
  if (vcl_fabs(newItkArrow->GetLength() - metaArrow->Length()) > precisionLimit)
    {
    std::cout << "Conversion to SpatialObject failed to convert length [FAILED]" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] MetaObject -> SpatialObject: length" << std::endl;
  
  // check color
  if (newItkArrow->GetProperty()->GetRed() != color[0] || 
      newItkArrow->GetProperty()->GetGreen() != color[1] || 
      newItkArrow->GetProperty()->GetBlue() != color[2] || 
      newItkArrow->GetProperty()->GetAlpha() != color[3])
    {
    std::cout << "Conversion to SpatialObject failed to convert color [FAILED]" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] MetaObject -> SpatialObject: color" << std::endl;
  
  // check parent id
  if (newItkArrow->GetParentId() != itkParent->GetId())
    {
    std::cout << "Conversion to SpatialObject failed to convert parent id [FAILED]" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] MetaObject -> SpatialObject: parent id" << std::endl;
  
  // check position
  SpatialObjectType::PointType itkPosition = newItkArrow->GetPosition();
  if (vcl_fabs(itkPosition[0] - mPosition[0]) > precisionLimit || 
      vcl_fabs(itkPosition[1] - mPosition[1]) > precisionLimit ||
      vcl_fabs(itkPosition[2] - mPosition[2]) > precisionLimit)
    {
    std::cout << "Conversion to SpatialObject failed to convert position [FAILED]" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] MetaObject -> SpatialObject: position" << std::endl;
  
  // check direction (note: need to normalize before comparing)
  SpatialObjectType::VectorType itkDirectionNorm = newItkArrow->GetDirection();
  itkDirectionNorm.Normalize();
  SpatialObjectType::VectorType mDirectionNorm = newItkArrow->GetDirection();
  mDirectionNorm[0] = mDirection[0];
  mDirectionNorm[1] = mDirection[1];
  mDirectionNorm[2] = mDirection[2];
  if (mDirection[0] != 0 || mDirection[1] != 0 || mDirection[2] != 0)
    {
    mDirectionNorm.Normalize();  
    }
  
  if (vcl_fabs(itkDirectionNorm[0] - mDirectionNorm[0]) > precisionLimit
      || vcl_fabs(itkDirectionNorm[1] - mDirectionNorm[1]) > precisionLimit
      || vcl_fabs(itkDirectionNorm[2] - mDirectionNorm[2])  > precisionLimit)
    {
    std::cout << "Conversion to SpatialObject failed to convert direction [FAILED]" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] MetaObject -> SpatialObject: direction" << std::endl;
  
  
  //
  // test writing
  //
  if (!converter->WriteMeta(itkArrow, av[1]))
    {
    std::cout << "Didn't write properly [FAILED]" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] SpatialObject write as MetaObject" << std::endl;
  
  
  
  //
  // test reading
  //
  SpatialObjectType::Pointer reLoad = converter->ReadMeta(av[1]);
  
  // check length
  if (vcl_fabs(reLoad->GetLength() - length) > precisionLimit)
    {
    std::cout << "Didn't read length properly [FAILED]" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] Reading: length" << std::endl;
  
  // check color
  if (reLoad->GetProperty()->GetRed() != color[0] || 
      reLoad->GetProperty()->GetGreen() != color[1] || 
      reLoad->GetProperty()->GetBlue() != color[2] || 
      reLoad->GetProperty()->GetAlpha() != color[3])
    {
    std::cout << "Didn't read color properly [FAILED]" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] Reading: color" << std::endl;
  
  // check parent id
  if (reLoad->GetParentId() != itkParent->GetId())
    {
    std::cout << "Didn't read parent id properly [FAILED]" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] Reading: parent id" << std::endl;
  
  // check position
  itkPosition = reLoad->GetPosition();
  if (vcl_fabs(itkPosition[0] - mPosition[0]) > precisionLimit || 
      vcl_fabs(itkPosition[1] - mPosition[1]) > precisionLimit ||
      vcl_fabs(itkPosition[2] - mPosition[2]) > precisionLimit)
    {
    std::cout << "Didn't read position properly [FAILED]" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED] Reading: position" << std::endl;
  
  // check direction (note: need to normalize before comparing)
  SpatialObjectType::VectorType reLoadDirectionNorm = reLoad->GetDirection();
  if (reLoadDirectionNorm[0] != 0 || reLoadDirectionNorm[1] != 0 || reLoadDirectionNorm[2] != 0)
    {
    reLoadDirectionNorm.Normalize();
    }
  
  if (vcl_fabs(reLoadDirectionNorm[0] - directionNorm[0]) > precisionLimit
      || vcl_fabs(reLoadDirectionNorm[1] - directionNorm[1]) > precisionLimit
      || vcl_fabs(reLoadDirectionNorm[2] - directionNorm[2])  > precisionLimit)
    {
    std::cout << "Didn't read direction properly [FAILED]" << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "[PASSED]  Reading: direction" << std::endl;
  
  
  
  // All tests executed successfully
  return EXIT_SUCCESS;
  
}
