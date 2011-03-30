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

#include "itkFEMLoadElementBase.h"

namespace itk
{
namespace fem
{

// Explicit New() method, used here because we need to split the itkNewMacro()
// in order to overload the CreateAnother() method.
LoadElement::Pointer LoadElement::New(void)
{
  Pointer smartPtr = ::itk::ObjectFactory< Self >::Create();
  if(smartPtr.IsNull())
  {
    smartPtr = static_cast<Pointer>(new Self);
  }
  smartPtr->UnRegister();
  return smartPtr;
}

// Overload the CreateAnother() method
::itk::LightObject::Pointer LoadElement::CreateAnother(void) const
{
  ::itk::LightObject::Pointer smartPtr;
  Pointer copyPtr = Self::New().GetPointer();
  
  for (unsigned int i=0; i < this->m_Element.size(); i++ )
  {
    copyPtr->AddNextElement( this->m_Element[i] );
  }
  copyPtr->SetGlobalNumber( this->GetGlobalNumber() );
  
  smartPtr = static_cast<Pointer>(copyPtr);
  
  return smartPtr;
}

void LoadElement::AddNextElement(Element::ConstPointer e)
{
  this->m_Element.push_back(e);
}

unsigned int LoadElement::GetNumberOfElements(void)
{
  return this->m_Element.size();
}

Element::ConstPointer LoadElement::GetElement(int i)
{
  return this->m_Element[i];
}

std::vector< Element::ConstPointer >& LoadElement::GetElementArray()
{
  return this->m_Element;
}

}
}  // end namespace itk::fem
