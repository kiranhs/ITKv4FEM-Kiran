/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkMetaFEMObjectConverter.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMetaFEMObjectConverter_h
#define __itkMetaFEMObjectConverter_h

//
// metaFEMObject.h must be included before itkFEMObjectSpatialObject.h
// to avoid an Internal Compiler Error in Visual Studio 6.0
//
#include "metaFEMObject.h"
#include "itkFEMObjectSpatialObject.h"

namespace itk 
{

template <unsigned int NDimensions = 3>
class ITK_EXPORT MetaFEMObjectConverter
{

public:

  MetaFEMObjectConverter();
  ~MetaFEMObjectConverter() {};

  typedef itk::FEMObjectSpatialObject<NDimensions>  SpatialObjectType;

  typedef typename SpatialObjectType::Pointer     SpatialObjectPointer;

  SpatialObjectPointer ReadMeta(const char* name);

  bool WriteMeta(SpatialObjectType* spatialObject,const char* name);

  SpatialObjectPointer MetaFEMObjectToFEMObjectSpatialObject(MetaFEMObject * femobject);

  MetaFEMObject* FEMObjectSpatialObjectToMetaFEMObject(SpatialObjectType * spatialObject);

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
  #include "itkMetaFEMObjectConverter.txx"
#endif


#endif
