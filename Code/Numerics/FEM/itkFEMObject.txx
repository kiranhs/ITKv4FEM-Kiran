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

// disable debug warnings in MS compiler
#ifdef _MSC_VER
#pragma warning(disable: 4786)
#endif

#include "itkFEMObject.h"

#include "itkFEMLoadNode.h"
#include "itkFEMLoadElementBase.h"
#include "itkFEMLoadBC.h"
#include "itkFEMLoadBCMFC.h"
#include "itkFEMLoadEdge.h"
#include "itkFEMLoadGrav.h"
#include "itkFEMLoadLandmark.h"
#include "itkFEMMaterialLinearElasticity.h"
#include "itkFEMFactoryBase.h"
#include "itkObjectFactoryBase.h"

#include <algorithm>

namespace itk
{
namespace fem
{
/*
 * Default constructor for FEMObject class
 */
template<unsigned int VDimension>
FEMObject<VDimension>::FEMObject()
{
  NGFN = 0;
  NMFC = 0;
  this->m_ElementContainer = ElementContainerType::New();
  this->m_NodeContainer = NodeContainerType::New();
  this->m_LoadContainer = LoadContainerType::New();
  this->m_MaterialContainer = MaterialContainerType::New();
}

template<unsigned int VDimension>
FEMObject<VDimension>::~FEMObject()
{
	
}

template<unsigned int VDimension>
void FEMObject<VDimension>::Clear() 
{
  this->m_NodeContainer->Initialize();
  this->m_ElementContainer->Initialize();
  this->m_LoadContainer->Initialize();
  this->m_MaterialContainer->Initialize();

  this->NGFN = 0;
  this->NMFC = 0;
}

template<unsigned int VDimension>
void FEMObject<VDimension>::DeepCopy( FEMObject *Copy) 
{
	this->Clear();
	
	// copy node information
	int numNodes = Copy->GetNumberOfNodes();
	itk::fem::Node::Pointer n1;
	
    itk::fem::Element::VectorType pt(VDimension);
 
	for (int i=0; i<numNodes; i++)
	{
	    n1 = itk::fem::Node::New();
	    for (unsigned int j=0; j<VDimension; j++)
	    {
			pt[j] = Copy->GetNode(i)->GetCoordinates()[j];
	    }    	
	    n1->SetCoordinates(pt);
	    n1->SetGlobalNumber(Copy->GetNode(i)->GetGlobalNumber());
		this->AddNextNode(&*n1);
	}
	
	// copy material information
	int numMat = Copy->GetNumberOfMaterials();
	itk::fem::MaterialLinearElasticity::Pointer m;
	
	for (int i=0; i<numMat; i++)
	{
		itk::fem::MaterialLinearElasticity::Pointer mCopy = dynamic_cast< itk::fem::MaterialLinearElasticity * >( &*Copy->GetMaterial(i) );
		m = itk::fem::MaterialLinearElasticity::New();
		m->SetGlobalNumber(mCopy->GetGlobalNumber());               
		m->SetYoungsModulus(mCopy->GetYoungsModulus()); 
		m->SetPoissonsRatio(mCopy->GetPoissonsRatio());
		m->SetCrossSectionalArea(mCopy->GetCrossSectionalArea()); 
		m->SetMomentOfInertia(mCopy->GetMomentOfInertia());      
		this->AddNextMaterial(&*m);
	}
	
	// copy element information
	int numElements = Copy->GetNumberOfElements();
  itk::LightObject::Pointer a = 0;

	for (int i=0; i<numElements; i++)
	{
	  itk::fem::Element *elCopy = Copy->GetElement(i);
		 // create a new object of the correct class
		 a = ObjectFactoryBase::CreateInstance( elCopy->GetNameOfClass() );

		itk::fem::Element::Pointer o1 = dynamic_cast< itk::fem::Element * >( &*a );
		o1->SetGlobalNumber(elCopy->GetGlobalNumber());
		
		numNodes = elCopy->GetNumberOfNodes();
		for (int j=0; j<numNodes; j++)
		{
			o1->SetNode( j, &*(this->GetNodeWithGlobalNumber(elCopy->GetNode(j)->GetGlobalNumber())));			
		}
		
		int matNum = elCopy->GetMaterial()->GetGlobalNumber();
		o1->SetMaterial(dynamic_cast< itk::fem::MaterialLinearElasticity * >
			( &*(this->GetMaterialWithGlobalNumber(matNum))));
		this->AddNextElement( &*o1);
	}

	// Copy load/bc information
	int numLoads = 	Copy->GetNumberOfLoads();
	
	for (int k=0; k<numLoads; k++)
	{
		 itk::fem::Load *load = Copy->GetLoad(k);
     // create a new object of the correct class
		 a = ObjectFactoryBase::CreateInstance( load->GetNameOfClass() );

	  std::string loadname = std::string(load->GetNameOfClass());
	  if(loadname == "LoadNode")
	  {
		  itk::fem::LoadNode::Pointer lCopy = 
			dynamic_cast< itk::fem::LoadNode * >( &*load );
		  itk::fem::LoadNode::Pointer  o1= itk::fem::LoadNode::New();
		
		  o1->SetGlobalNumber(lCopy->GetGlobalNumber());
		  
		  o1->SetElement(&*this->GetElementWithGlobalNumber(lCopy->GetElement()->GetGlobalNumber()));
		  
		  o1->SetNode(lCopy->GetNode());
		  
		  int dim = VDimension;
		  vnl_vector< double > F(dim);
		  for (int i=0; i<dim; i++)
		  {
			F[i] = lCopy->GetForce()[i];
		  }
		  o1->SetForce(F);
	      this->AddNextLoad( &*o1);
	      goto out;
	  }

	  if(loadname == "LoadBC")
	  {
		  itk::fem::LoadBC::Pointer lCopy = 
			dynamic_cast< itk::fem::LoadBC * >( &*load );
			
		  itk::fem::LoadBC::Pointer  o1= itk::fem::LoadBC::New();

		  o1->SetGlobalNumber(lCopy->GetGlobalNumber());
		  
		  o1->SetDegreeOfFreedom(lCopy->GetDegreeOfFreedom());
		  
		  o1->SetElement(&*this->GetElementWithGlobalNumber(lCopy->GetElement()->GetGlobalNumber()));
		  	  
		  int numRHS = lCopy->GetValue().size();
		  vnl_vector< double > F(numRHS);
		  for (int i=0; i<numRHS; i++)
		  {
			F[i] = lCopy->GetValue()[i];
		  }
		  o1->SetValue(F);
	      this->AddNextLoad( &*o1);
	      goto out;
	  }
	  
	  if(loadname == "LoadBCMFC")
	  {
		  itk::fem::LoadBCMFC::Pointer lCopy = 
			dynamic_cast< itk::fem::LoadBCMFC * >( &*load );
			
		  itk::fem::LoadBCMFC::Pointer  o1= itk::fem::LoadBCMFC::New();
		  o1->SetGlobalNumber(lCopy->GetGlobalNumber());

			int NumLHS;
			int elementGN;
			int DOF;
			float Value;
		
			NumLHS = lCopy->GetNumberOfLeftHandSideTerms();
		
		for ( int i = 0; i < NumLHS; i++ )
		{
			itk::fem::LoadBCMFC::MFCTerm mfcTerm = lCopy->GetLeftHandSideArray()[i];
			elementGN = mfcTerm.m_element->GetGlobalNumber();

			DOF = mfcTerm.dof;

			Value = mfcTerm.value;
	
			o1->GetLeftHandSideArray().push_back( 
			itk::fem::LoadBCMFC::MFCTerm(&*this->GetElementWithGlobalNumber(elementGN), DOF, Value) );
		}

		int NumRHS = lCopy->GetNumberOfRightHandSideTerms();
  
		for (int i=0; i<NumRHS; i++)
		{
			o1->GetRightHandSideArray().set_size(o1->GetRightHandSideArray().size() + 1);
			o1->GetRightHandSideArray().put(o1->GetRightHandSideArray().size() - 1, lCopy->GetRightHandSideArray()[i]);
		}		  
	      this->AddNextLoad( &*o1);
	      goto out;
	  }

	  if(loadname == "LoadEdge")
	  {
		itk::fem::LoadEdge::Pointer lCopy = 
			dynamic_cast< itk::fem::LoadEdge * >( &*load );
			
		itk::fem::LoadEdge::Pointer  o1= itk::fem::LoadEdge::New();

		o1->SetGlobalNumber(lCopy->GetGlobalNumber());
		  
		int numRows, numCols;
				
		o1->AddNextElement(&*this->GetElementWithGlobalNumber(lCopy->GetElement(0)->GetGlobalNumber()));
		o1->SetGlobalNumber(lCopy->GetGlobalNumber());
		o1->SetEdge(lCopy->GetEdge());
		
		vnl_matrix<itk::fem::Element::Float> force = lCopy->GetForce();
				
		numRows = force.rows();
		numCols = force.columns();
		
		if(numRows)
		{
			o1->GetForce().set_size(numRows, numCols);
			for ( int i = 0; i < numRows; i++ )
			{
				for ( int j = 0; j < numCols; j++ )
				{
					o1->GetForce()[i][j] = force[i][j];
				}
			}
			this->AddNextLoad( &*o1);
		}
		goto out;
	  }	
	  
	  if(loadname == "LoadGravConst")
	  {
		itk::fem::LoadGravConst::Pointer lCopy = 
			dynamic_cast< itk::fem::LoadGravConst * >( &*load );
			
		itk::fem::LoadGravConst::Pointer  o1= itk::fem::LoadGravConst::New();

		o1->SetGlobalNumber(lCopy->GetGlobalNumber());
		
		for (unsigned int i=0; i<lCopy->GetElementArray().size(); i++)
		{
			o1->GetElementArray().push_back(&*this->GetElementWithGlobalNumber(
				(lCopy->GetElementArray()[i])->GetGlobalNumber()));
		}
	    
	    int dim = lCopy->GetForce().size();
	    o1->GetForce().set_size(dim);	
		for(int i=0; i<dim; i++)
		{
			o1->GetForce()[i] = lCopy->GetForce()[i];
		}				  						
	      this->AddNextLoad( &*o1);
	  }	  
	  out:
	  ;
  } 
	
}


template<unsigned int VDimension>
void FEMObject<VDimension>::FinalizeMesh()
{
  this->GenerateGFN();
  this->GenerateMFC();
}
/**
 * Assign a global freedom number to each DOF in a system.
 */
template<unsigned int VDimension>
void FEMObject<VDimension>::GenerateMFC()
{
  if ( NGFN <= 0 ) { return; }

  NMFC = 0;  // reset number of MFC in a system

  /**
   * Before we can start the assembly procedure, we need to know,
   * how many boundary conditions if form of MFCs are there in a system.
   */

  // search for MFC's in Loads array, because they affect the master stiffness
  // matrix
  int numLoads = this->m_LoadContainer->Size();
  for ( int l = 0; l < numLoads; l++ )
    {
    if ( LoadBCMFC::Pointer l1 = dynamic_cast< LoadBCMFC * >( &*this->GetLoad(l) ) )
      {
      // store the index of an LoadBCMFC object for later
      l1->SetIndex(NMFC);

      // increase the number of MFC
      NMFC++;
      }
    }
}

/**
 * Assign a global freedom number to each DOF in a system.
 */
template<unsigned int VDimension>
void FEMObject<VDimension>::GenerateGFN()
{
  // Clear the list of elements and global freedom numbers in nodes
  // FIXME: should be removed once Mesh is there
  int numNodes = this->m_NodeContainer->Size();
  for ( int n = 0; n < numNodes; n++ )
    {
    Node::Pointer np = this->GetNode(n);
    np->m_elements.clear();
    np->ClearDegreesOfFreedom();
    }

  int numElements = this->m_ElementContainer->Size();
  for ( int e = 0; e < numElements; e++ ) // step over
                                                                    // all
                                                                    // elements
    {
    // Add the elemens in the nodes list of elements
    // FIXME: should be removed once Mesh is there
    Element::Pointer el = this->GetElement(e);
    unsigned int Npts = el->GetNumberOfNodes();
    for ( unsigned int pt = 0; pt < Npts; pt++ )
      {
      el->GetNode(pt)->m_elements.insert(el);
      }
    }

  /**
   * Assign new ID to every DOF in a system
   */

  // Start numbering DOFs from 0
  NGFN = 0;

  // Step over all elements
  for ( int e = 0; e < numElements; e++ )
    {
    // FIXME: Write a code that checks if two elements are compatible, when they
    // share a node
    Element::Pointer el = GetElement(e);
    for ( unsigned int n = 0; n < el->GetNumberOfNodes(); n++ )
      {
      for ( unsigned int dof = 0; dof < el->GetNumberOfDegreesOfFreedomPerNode(); dof++ )
        {
        if ( el->GetNode(n)->GetDegreeOfFreedom(dof) == Element::InvalidDegreeOfFreedomID )
          {
          el->GetNode(n)->SetDegreeOfFreedom(dof, NGFN);
          NGFN++;
          }
        }
      }
    } // end for e

  //  NGFN=Element::GetGlobalDOFCounter()+1;
  if ( NGFN > 0 )
    {
    return;            // if we got 0 DOF, somebody forgot to define the
                       // system...
    }
}


template<unsigned int VDimension>
void FEMObject<VDimension>::RenumberNodeContainer()
{

  int numNodes = this->m_NodeContainer->Size();
  for (int i = 0; i < numNodes; i++ )
    {
    this->GetNode(i)->SetGlobalNumber(i);
    }
}

template<unsigned int VDimension>
void FEMObject<VDimension>::AddNextElement(Element::Pointer e)
{
  ElementIdentifier size = this->m_ElementContainer->Size();
  this->m_ElementContainer->InsertElement(size, e);
}

template<unsigned int VDimension>
void FEMObject<VDimension>::InsertElement(Element::Pointer e, ElementIdentifier index)
{
    this->m_ElementContainer->InsertElement(index, e);
}

template<unsigned int VDimension>
void FEMObject<VDimension>::AddNextNode(Node::Pointer e)
{
  NodeIdentifier size = this->m_NodeContainer->Size();
  this->m_NodeContainer->InsertElement(size, e);
}

template<unsigned int VDimension>
void FEMObject<VDimension>::InsertNode(Node::Pointer e, NodeIdentifier index)
{
    this->m_NodeContainer->InsertElement(index, e);
}

template<unsigned int VDimension>
void FEMObject<VDimension>::AddNextMaterial(Material::Pointer e)
{
  MaterialIdentifier size = this->m_MaterialContainer->Size();
  this->m_MaterialContainer->InsertElement(size, e);
}

template<unsigned int VDimension>
void FEMObject<VDimension>::InsertMaterial(Material::Pointer e, MaterialIdentifier index)
{
    this->m_MaterialContainer->InsertElement(index, e);
}

template<unsigned int VDimension>
void FEMObject<VDimension>::AddNextLoad(Load::Pointer e)
{
  LoadIdentifier size = this->m_LoadContainer->Size();
  this->m_LoadContainer->InsertElement(size, e);
}

template<unsigned int VDimension>
void FEMObject<VDimension>::InsertLoad(Load::Pointer e, LoadIdentifier index)
{
    this->m_LoadContainer->InsertElement(index, e);
}

template<unsigned int VDimension>
Element::Pointer FEMObject<VDimension>::GetElement(ElementIdentifier index)
{
  return this->m_ElementContainer->GetElement(index);
}

template<unsigned int VDimension>
Element::Pointer FEMObject<VDimension>::GetElementWithGlobalNumber(int globalNumber)
{
 int numElements = this->m_ElementContainer->Size();
 for (int i=0; i<numElements; i++)
 {
	if(this->m_ElementContainer->GetElement(i)->GetGlobalNumber() == globalNumber)
	{
		return	this->m_ElementContainer->GetElement(i);
	}
 }
 return NULL; 
}

template<unsigned int VDimension>
Node::Pointer FEMObject<VDimension>::GetNode(NodeIdentifier index)
{
  return this->m_NodeContainer->GetElement(index);
}

template<unsigned int VDimension>
Node::Pointer FEMObject<VDimension>::GetNodeWithGlobalNumber(int globalNumber)
{
 int numNodes = this->m_NodeContainer->Size();
 for (int i=0; i<numNodes; i++)
 {
	if(this->m_NodeContainer->GetElement(i)->GetGlobalNumber() == globalNumber)
	{
		return	this->m_NodeContainer->GetElement(i);
	}
 }
 return NULL; 
}

template<unsigned int VDimension>
Load::Pointer FEMObject<VDimension>::GetLoad(LoadIdentifier index)
{
  return this->m_LoadContainer->GetElement(index);
}

template<unsigned int VDimension>
Load::Pointer FEMObject<VDimension>::GetLoadWithGlobalNumber(int globalNumber)
{
 int numLoads = this->m_LoadContainer->Size();
 for (int i=0; i<numLoads; i++)
 {
	if(this->m_LoadContainer->GetElement(i)->GetGlobalNumber() == globalNumber)
	{
		return	this->m_LoadContainer->GetElement(i);
	}
 }
 return NULL; 
}

template<unsigned int VDimension>
Material::Pointer FEMObject<VDimension>::GetMaterial(MaterialIdentifier index)
{
  return this->m_MaterialContainer->GetElement(index);
}

template<unsigned int VDimension>
Material::Pointer FEMObject<VDimension>::GetMaterialWithGlobalNumber(int globalNumber)
{
 int numMaterials = this->m_MaterialContainer->Size();
 for (int i=0; i<numMaterials; i++)
 {
	if(this->m_MaterialContainer->GetElement(i)->GetGlobalNumber() == globalNumber)
	{
		return	this->m_MaterialContainer->GetElement(i);
	}
 }
 return NULL; 
}

template<unsigned int VDimension>
void FEMObject<VDimension>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "NGFN: " << this->NGFN << std::endl;
  os << indent << "NMFC: " << this->NMFC << std::endl;
  
  os << indent << "ElementContainer: " << this->m_ElementContainer << std::endl;
  os << indent << "NodeContainer: " << this->m_NodeContainer << std::endl;
  os << indent << "LoadContainer: " << this->m_LoadContainer << std::endl;
  os << indent << "MaterialContainer: " << this->m_MaterialContainer << std::endl;
}

}
} // end namespace itk::fem
