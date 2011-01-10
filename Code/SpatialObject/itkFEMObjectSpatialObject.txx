/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFEMObjectSpatialObject.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkFEMObjectSpatialObject_txx
#define __itkFEMObjectSpatialObject_txx


#include "itkFEMObjectSpatialObject.h"

namespace itk
{

/** Constructor */
template< unsigned int TDimension>
FEMObjectSpatialObject< TDimension>
::FEMObjectSpatialObject()
{
  this->SetTypeName("FEMObjectSpatialObject");
  m_FEMObject = FEMObjectType::New();
}

/** Destructor */
template< unsigned int TDimension>
FEMObjectSpatialObject< TDimension>
::~FEMObjectSpatialObject()
{
}

/** Set the femobject in the spatial object */
template< unsigned int TDimension>
void
FEMObjectSpatialObject< TDimension>
::SetFEMObject(FEMObjectType * femobject )
{
  if( !femobject )
    {
    return;
    }

  m_FEMObject = femobject;
}

/** Get the femobject inside the spatial object */
template< unsigned int TDimension>
typename FEMObjectSpatialObject< TDimension >::FEMObjectType *
FEMObjectSpatialObject< TDimension>
::GetFEMObject( void )
{
  return m_FEMObject.GetPointer();
}

/** Print the object */
template< unsigned int TDimension>
void
FEMObjectSpatialObject< TDimension>
::PrintSelf( std::ostream& os, Indent indent ) const
{
  Superclass::PrintSelf(os,indent);
  os << "FEMObject: " << std::endl;
  os << indent << m_FEMObject << std::endl;
}

/** Get the modification time */
/*template< unsigned int TDimension>
unsigned long
FEMObjectSpatialObject< TDimension>
::GetMTime( void ) const
{
  unsigned long latestMTime = Superclass::GetMTime();
  unsigned long femobjectMTime = m_FEMObject->GetMTime();

  if( femobjectMTime > latestMTime )
    {
    latestMTime = femobjectMTime;
    }

  return latestMTime;
}*/

} // end namespace itk

#endif //__FEMObjectSpatialObject_txx
