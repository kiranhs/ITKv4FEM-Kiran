/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFEMObjectSpatialObject.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkFEMObjectSpatialObject_h
#define __itkFEMObjectSpatialObject_h

#include "itkFEMObject.h"
#include "itkExceptionObject.h"
#include "itkSpatialObject.h"

namespace itk
{
  
/** \class FEMObjectSpatialObject
 * \brief Implementation of an femobject as spatial object.
 *
 * This class combines functionalities from a spatial object,
 * and an femobject.
 *
 * \sa SpatialObject CompositeSpatialObject
 */

template < unsigned int TDimension = 3>
class ITK_EXPORT FEMObjectSpatialObject : public SpatialObject< TDimension >
{

public:
 
  typedef FEMObjectSpatialObject< TDimension > Self;
  typedef SpatialObject< TDimension >                 Superclass;
  typedef SmartPointer< Self >                        Pointer;
  typedef SmartPointer< const Self >                  ConstPointer;

  typedef itk::fem::FEMObject< TDimension >					  FEMObjectType;
  typedef typename FEMObjectType::Pointer      FEMObjectPointer;
 
  /** Method for creation through the object factory. */
  itkNewMacro( Self );

  /** Run-time type information (and related methods). */
  itkTypeMacro( FEMObjectSpatialObject, SpatialObject );

  /** Set the femobject. */
  void SetFEMObject( FEMObjectType * femobject );

  /** Get a pointer to the femobject currently attached to the object. */
  FEMObjectType * GetFEMObject( void );

protected:
  FEMObjectSpatialObject(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  FEMObjectPointer m_FEMObject;

  FEMObjectSpatialObject();
  virtual ~FEMObjectSpatialObject();

  void PrintSelf( std::ostream& os, Indent indent ) const;

};

} // end of namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkFEMObjectSpatialObject.txx"
#endif

#endif //__itkFEMObjectSpatialObject_h
