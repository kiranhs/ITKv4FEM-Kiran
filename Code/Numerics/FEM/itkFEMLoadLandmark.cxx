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

/**
 * Find the Element to which the LoadLandmark belongs
 */
Element::ConstPointer LoadLandmark::GetAssignedElement(Element::ArrayType1::Pointer elements)
{
  int numElements = elements->Size();
	for ( int n = 0; n < numElements; n++ )
	  {
		Element::Pointer nel = elements->GetElement(n);
		if ( (nel )->GetLocalFromGlobalCoordinates(m_source, this->m_pt) )
      {
#ifndef FEM_USE_SMART_POINTERS
      return nel;
#else
      return dynamic_cast<const Element *> (&*nel);
#endif
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
    if ( ( *n )->GetLocalFromGlobalCoordinates(m_source, this->m_pt) )
      {
      isFound = true;
      std::cout << "Found: " << ( &**n ) << std::endl;
      this->el[0] = *n;
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
		if ( (nel )->GetLocalFromGlobalCoordinates(m_source, this->m_pt) )
		{
			isFound = true;
			std::cout << "Found: " << ( &*nel ) << std::endl;
			this->el[0] = nel;
		}
	}

	if ( !isFound )
	{
		throw FEMException(__FILE__, __LINE__, "LoadLandmark::Read() - could not find element containing landmark!");
	}
}


void LoadLandmark::SetEta(double e)
{
  this->eta = e;
}

double LoadLandmark::GetEta()
{
  return this->eta;
}

FEM_CLASS_REGISTER(LoadLandmark)
}
}  // end namespace itk::fem
