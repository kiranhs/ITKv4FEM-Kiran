/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
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
template< unsigned int TDimension>
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
}

} // end namespace itk

#endif //__FEMObjectSpatialObject_txx
