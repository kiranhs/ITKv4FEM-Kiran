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

#include "itkFEMLoadLandmark.h"

namespace itk
{
namespace fem
{

// Explicit New() method, used here because we need to split the itkNewMacro()
// in order to overload the CreateAnother() method.
LoadLandmark::Pointer LoadLandmark::New(void)
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
::itk::LightObject::Pointer LoadLandmark::CreateAnother(void) const
{
  ::itk::LightObject::Pointer smartPtr;
  Pointer copyPtr = Self::New().GetPointer();
  
  //Copy Load Contents
  copyPtr->m_Eta = this->m_Eta;
  copyPtr->m_Point = this->m_Point;
  copyPtr->m_Target = this->m_Target;
  copyPtr->m_Source = this->m_Source;
  copyPtr->m_Force = this->m_Force;
  copyPtr->m_Solution = this->m_Solution;
  for (unsigned int i=0; i < this->m_Element.size(); i++ )
  {
    copyPtr->AddNextElement( this->m_Element[i] );
  }
  copyPtr->SetGlobalNumber( this->GetGlobalNumber() );
  
  smartPtr = static_cast<Pointer>(copyPtr);
  
  return smartPtr;
}


/**
 * Find the Element to which the LoadLandmark belongs
 */
Element::ConstPointer LoadLandmark::GetAssignedElement(Element::ArrayType1::Pointer elements)
{
  int numElements = elements->Size();
	for ( int n = 0; n < numElements; n++ )
	  {
		Element::Pointer nel = elements->GetElement(n);
		if ( (nel )->GetLocalFromGlobalCoordinates(m_Source, this->m_Point) )
      {
      return dynamic_cast<const Element *> (&*nel);
      }
    }

  return NULL;
}


/**
 * Find the Element to which the LoadLandmark belongs
 */

void LoadLandmark::AssignToElement(Element::ArrayType::Pointer elements)
{
  bool isFound = false;

  // Compute & store the local coordinates of the undeformed point and
  // the pointer to the element

  for ( Element::ArrayType::const_iterator n = elements->begin();
        n != elements->end() && !isFound; n++ )
    {
    if ( ( *n )->GetLocalFromGlobalCoordinates(m_Source, this->m_Point) )
      {
      isFound = true;
      std::cout << "Found: " << ( &**n ) << std::endl;
      this->m_Element[0] = *n;
      }
    }

  if ( !isFound )
    {
    throw FEMException(__FILE__, __LINE__, "LoadLandmark::Read() - could not find element containing landmark!");
    }
}

void LoadLandmark::AssignToElement(Element::ArrayType1::Pointer elements)
{
	bool isFound = false;

	// Compute & store the local coordinates of the undeformed point and
	// the pointer of the element
	
	int numElements = elements->Size();
	for ( int n = 0;
		n < numElements && !isFound; n++ )
	{
		Element::Pointer nel = elements->GetElement(n);
		if ( (nel )->GetLocalFromGlobalCoordinates(m_Source, this->m_Point) )
		{
			isFound = true;
			std::cout << "Found: " << ( &*nel ) << std::endl;
			this->m_Element[0] = nel;
		}
	}

	if ( !isFound )
	{
		throw FEMException(__FILE__, __LINE__, "LoadLandmark::Read() - could not find element containing landmark!");
	}
}


void LoadLandmark::SetEta(double e)
{
  this->m_Eta = e;
}

double LoadLandmark::GetEta()
{
  return this->m_Eta;
}

void LoadLandmark::ApplyLoad(Element::ConstPointer element, Element::VectorType & Fe)
{
  const unsigned int NnDOF = element->GetNumberOfDegreesOfFreedomPerNode();
  const unsigned int Nnodes = element->GetNumberOfNodes();
  
  Element::VectorType force(NnDOF, 0.0);
  Element::VectorType disp(NnDOF, 0.0);
  Element::VectorType new_source (NnDOF, 0.0);
  Element::VectorType shapeF;
  
  Fe.set_size( element->GetNumberOfDegreesOfFreedom() );
  Fe.fill(0.0);
  
  // Retrieve the local coordinate at which the force acts
  Element::VectorType pt = this->GetPoint();
  
  // Retrieve the stored solution
  Solution::ConstPointer sol = this->GetSolution();
  
  // Determine the displacement at point pt
  const unsigned int TotalSolutionIndex = 1;
  disp = element->InterpolateSolution(pt, ( *sol ), TotalSolutionIndex);
  
  // Convert the source to global coordinates
  new_source = this->GetSource() + disp;
  
  // Calculate the new force
  // changes made - kiran
  //load->m_Force =  disp;
  //force =  (load->m_Target-new_source) / load->eta;
  this->SetForce(disp);
  force =  ( this->GetTarget() - new_source ) / this->GetEta();
  // changes made - kiran
  
  //  std::cout << " disp " << disp <<  std::endl;
  //force /= vcl_sqrt(fmag);
  new_source = ( this->GetTarget() - new_source );
  //  std::cout << " force = " << force <<  " distance  " <<
  // new_source.magnitude() << std::endl;
  
  Element::Float curdist = new_source.magnitude();
  if ( curdist < 1.0 )
  {
    force.fill(0.0);
  }
  std::cout <<  " LM distance  " << curdist << std::endl;
  
  // "Integrate" at the location of the point load
  shapeF = element->ShapeFunctions(pt);
  
  // Calculate the equivalent nodal loads
  for ( unsigned int n = 0; n < Nnodes; n++ )
  {
    for ( unsigned int d = 0; d < NnDOF; d++ )
    {
      Fe[n * NnDOF + d] += shapeF[n] * force[d];
    }
  }
} 
  
}
}  // end namespace itk::fem
