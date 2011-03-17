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

#include "itkFEMLoadEdge.h"

namespace itk
{
namespace fem
{

void LoadEdge::SetEdge(int edge)
{
  this->m_Edge = edge;
}

int LoadEdge::GetEdge()
{
  return this->m_Edge;
}

void LoadEdge::SetForce(const vnl_matrix< itk::fem::Element::Float > force)
{
  this->m_Force = force;
}

vnl_matrix< itk::fem::Element::Float >& LoadEdge::GetForce()
{
  return this->m_Force;
}

  
// Explicit New() method, used here because we need to split the itkNewMacro()
// in order to overload the CreateAnother() method.
LoadEdge::Pointer LoadEdge::New(void)
{
  Pointer smartPtr = ::itk::ObjectFactory< Self >::Create();
  if(smartPtr.IsNull())
  {
    smartPtr = static_cast<Pointer>(new Self);
  }
  smartPtr->UnRegister();
  return smartPtr;
}

// Explicit New() method, used here because we need to split the itkNewMacro()
// in order to overload the CreateAnother() method.  
::itk::LightObject::Pointer LoadEdge::CreateAnother(void) const
{
  ::itk::LightObject::Pointer smartPtr;
  Pointer copyPtr = Self::New().GetPointer();
  
  copyPtr->m_Edge = this->m_Edge;
  copyPtr->m_Force = this->m_Force;
  copyPtr->el = this->el;
  copyPtr->GN = this->GN;
  
  smartPtr = static_cast<Pointer>(copyPtr);
  
  return smartPtr;
}
  
FEM_CLASS_REGISTER(LoadEdge)
}
}  // end namespace itk::fem
