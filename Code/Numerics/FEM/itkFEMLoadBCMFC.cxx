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
  lhs.clear();

  /** Set the correct weight */
  lhs.push_back( MFCTerm(element, dof, 1.0) );
  rhs = val;
}

void LoadBCMFC::SetIndex(int ind)
{
  this->Index = ind;
}

int LoadBCMFC::GetIndex()
{
  return this->Index;
}

void LoadBCMFC::AddLeftHandSideTerm(LoadBCMFC::MFCTerm term)
{
  this->lhs.push_back(term);
}

void LoadBCMFC::AddRightHandSideTerm(Element::Float term)
{
  // should check if the contents are overwritten if size is reset
  this->rhs.set_size(this->rhs.size() + 1);
  this->rhs.put(this->rhs.size() - 1, term);
}

int LoadBCMFC::GetNumberOfLeftHandSideTerms()
{
  return this->lhs.size();
}

int LoadBCMFC::GetNumberOfRightHandSideTerms()
{
  return this->rhs.size();
}

itk::fem::LoadBCMFC::MFCTerm LoadBCMFC::GetLeftHandSideTerm(int lhs)
{
  return this->lhs.at(lhs);
}

Element::Float LoadBCMFC::GetRightHandSideTerm(int rhs)
{
  return this->rhs.get(rhs);
}

std::vector< itk::fem::LoadBCMFC::MFCTerm >& LoadBCMFC::GetLeftHandSideArray()
{
  return this->lhs;
}

vnl_vector< Element::Float >& LoadBCMFC::GetRightHandSideArray()
{
  return this->rhs;
}

}
}  // end namespace itk::fem
