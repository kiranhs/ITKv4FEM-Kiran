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

#include "itkFEMLoadBCMFC.h"

namespace itk
{
namespace fem
{
/**
 * Fix a DOF to a prescribed value
 */
LoadBCMFC::LoadBCMFC(Element::ConstPointer element, int dof, vnl_vector< Element::Float > val)
{
  m_LeftHandSide.clear();

  /** Set the correct weight */
  m_LeftHandSide.push_back( MFCTerm(element, dof, 1.0) );
  m_RightHandSide = val;
}

void LoadBCMFC::SetIndex(int ind)
{
  this->m_Index = ind;
}

int LoadBCMFC::GetIndex()
{
  return this->m_Index;
}

void LoadBCMFC::AddLeftHandSideTerm(LoadBCMFC::MFCTerm term)
{
  this->m_LeftHandSide.push_back(term);
}

void LoadBCMFC::AddRightHandSideTerm(Element::Float term)
{
  vnl_vector< Element::Float > tmpRightHandSide;
  tmpRightHandSide.set_size( this->m_RightHandSide.size() );
  for (unsigned int i=0;i<this->m_RightHandSide.size();i++)
  {
    tmpRightHandSide[i] = this->m_RightHandSide[i];
  }
  
  this->m_RightHandSide.set_size(this->m_RightHandSide.size() + 1);
  for (unsigned int i=0;i<tmpRightHandSide.size();i++)
  {
    this->m_RightHandSide[i] = tmpRightHandSide[i];
  }
  
  this->m_RightHandSide.put(this->m_RightHandSide.size() - 1, term);
}

int LoadBCMFC::GetNumberOfLeftHandSideTerms()
{
  return this->m_LeftHandSide.size();
}

int LoadBCMFC::GetNumberOfRightHandSideTerms()
{
  return this->m_RightHandSide.size();
}

itk::fem::LoadBCMFC::MFCTerm LoadBCMFC::GetLeftHandSideTerm(int lhs)
{
  return this->m_LeftHandSide.at(lhs);
}

Element::Float LoadBCMFC::GetRightHandSideTerm(int rhs)
{
  return this->m_RightHandSide.get(rhs);
}

std::vector< itk::fem::LoadBCMFC::MFCTerm >& LoadBCMFC::GetLeftHandSideArray()
{
  return this->m_LeftHandSide;
}

vnl_vector< Element::Float >& LoadBCMFC::GetRightHandSideArray()
{
  return this->m_RightHandSide;
}

}
}  // end namespace itk::fem
