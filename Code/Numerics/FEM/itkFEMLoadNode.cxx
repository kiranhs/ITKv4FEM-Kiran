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

#include "itkFEMLoadNode.h"

namespace itk
{
namespace fem
{

// Explicit New() method, used here because we need to split the itkNewMacro()
// in order to overload the CreateAnother() method.
LoadNode::Pointer LoadNode::New(void)
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
::itk::LightObject::Pointer LoadNode::CreateAnother(void) const
{
  ::itk::LightObject::Pointer smartPtr;
  Pointer copyPtr = Self::New().GetPointer();
  
  copyPtr->m_element = this->m_element;
  copyPtr->m_pt = this->m_pt;
  copyPtr->F = this->F;
  copyPtr->GN = this->GN;
  
  smartPtr = static_cast<Pointer>(copyPtr);
  
  return smartPtr;
}
  
void LoadNode::SetNode(int num)
{
  this->m_pt = num;
}

int LoadNode::GetNode()
{
  return this->m_pt;
}

void LoadNode::SetForce(const vnl_vector< Float > force)
{
  this->F = force;
}

vnl_vector< itk::fem::Element::Float > LoadNode::GetForce()
{
  return this->F;
}

Element::ConstPointer LoadNode::GetElement()
{
  return this->m_element;
}

void LoadNode::SetElement(Element::ConstPointer el)
{
  this->m_element = el;
}

FEM_CLASS_REGISTER(LoadNode)
}
}  // end namespace itk::fem
