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
  copyPtr->m_Element = this->m_Element;
  copyPtr->SetGlobalNumber( this->GetGlobalNumber() );
  
  smartPtr = static_cast<Pointer>(copyPtr);
  
  return smartPtr;
}

void LoadEdge::ApplyLoad(Element::ConstPointer element, Element::VectorType & Fe)
{
  const unsigned int NnDOF = element->GetNumberOfDegreesOfFreedomPerNode();
  //const unsigned int Nnodes = element->GetNumberOfNodes();
  const unsigned int EdgeNum = this->GetEdge();
  
  vnl_matrix<itk::fem::Element::Float> Force = this->GetForce();
  
  const std::vector< std::vector<int> > EdgeIds = element->GetEdgeIds();
  
  Fe.set_size( element->GetNumberOfDegreesOfFreedom() );
  Fe.fill(0.0);
  
  //int NEdges = EdgeIds.size();
  int NEdgePts = (EdgeIds[0]).size();
  int EdgePt;
  // access the edge points.
  for (int i=0; i<NEdgePts; i++)
  {
    EdgePt = (EdgeIds[EdgeNum])[i];
    for (unsigned int j=0; j<NnDOF; j++)
    {
      Fe[NnDOF*EdgePt + j] = Fe[NnDOF*EdgePt + j] + Force[i][j];
    }	  
  }
  
}
  
}
}  // end namespace itk::fem
