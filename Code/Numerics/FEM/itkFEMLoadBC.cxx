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

#include "itkFEMLoadBC.h"

namespace itk
{
namespace fem
{

// Explicit New() method, used here because we need to split the itkNewMacro()
// in order to overload the CreateAnother() method.
LoadBC::Pointer LoadBC::New(void)
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
::itk::LightObject::Pointer LoadBC::CreateAnother(void) const
{
  ::itk::LightObject::Pointer smartPtr;
  Pointer copyPtr = Self::New().GetPointer();
  
  //Copy Load Contents
  copyPtr->m_DegreeOfFreedom = this->m_DegreeOfFreedom;
  copyPtr->m_Value = this->m_Value;
  copyPtr->m_Element = this->m_Element;
  copyPtr->SetGlobalNumber( this->GetGlobalNumber() );
  
  smartPtr = static_cast<Pointer>(copyPtr);
  
  return smartPtr;
}
  
void LoadBC::SetDegreeOfFreedom(int dof)
{
  this->m_DegreeOfFreedom = dof;
}

int LoadBC::GetDegreeOfFreedom()
{
  return this->m_DegreeOfFreedom;
}

void LoadBC::SetValue(const vnl_vector< Element::Float > val)
{
  this->m_Value = val;
}

vnl_vector< Element::Float > LoadBC::GetValue()
{
  return this->m_Value;
}

void LoadBC::SetElement(Element::ConstPointer element)
{
  this->m_Element = element;
}

Element::ConstPointer LoadBC::GetElement()
{
  return this->m_Element;
}

}
}  // end namespace itk::fem
