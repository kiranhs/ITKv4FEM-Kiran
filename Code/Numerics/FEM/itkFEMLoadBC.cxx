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

void LoadBC::SetDegreeOfFreedom(int dof)
{
  this->m_dof = dof;
}

int LoadBC::GetDegreeOfFreedom()
{
  return this->m_dof;
}

void LoadBC::SetValue(const vnl_vector< Element::Float > val)
{
  this->m_value = val;
}

vnl_vector< Element::Float > LoadBC::GetValue()
{
  return this->m_value;
}

void LoadBC::SetElement(Element::ConstPointer element)
{
  this->m_element = element;
}

Element::ConstPointer LoadBC::GetElement()
{
  return this->m_element;
}

FEM_CLASS_REGISTER(LoadBC)
}
}  // end namespace itk::fem
