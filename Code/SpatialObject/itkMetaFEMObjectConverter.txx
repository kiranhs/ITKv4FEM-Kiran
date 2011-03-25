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
#ifndef __itkMetaFEMObjectConverter_txx
#define __itkMetaFEMObjectConverter_txx

#include "itkMetaFEMObjectConverter.h"

#include "itkFEMElementBase.h"
#include "itkFEMLightObject.h"
#include "itkFEMObjectFactory.h"
#include "itkFEMMaterialLinearElasticity.h"
#include "itkFEMLoadEdge.h"
#include "itkFEMLoadGrav.h"
#include "itkFEMFactoryBase.h"
#include "itkObjectFactoryBase.h"

namespace itk  
{

/** Constructor */ 
template <unsigned int NDimensions>
MetaFEMObjectConverter<NDimensions>
::MetaFEMObjectConverter()
{
  itk::FEMFactoryBase::RegisterDefaultTypes();
}

/** Convert a metaFEMObject into an FEMObject SpatialObject  */
template <unsigned int NDimensions>
typename MetaFEMObjectConverter<NDimensions>::SpatialObjectPointer
MetaFEMObjectConverter<NDimensions>
::MetaFEMObjectToFEMObjectSpatialObject(MetaFEMObject * femobject)
{ 
  SpatialObjectPointer spatialObject = SpatialObjectType::New();
  typedef itk::fem::FEMObject<NDimensions>  FEMObjectType;

  typedef typename FEMObjectType::Pointer     FEMObjectPointer;

  FEMObjectPointer myFEMObject = FEMObjectType::New();
  itk::LightObject::Pointer a = 0;
    
  // copy all the node information
  typedef typename MetaFEMObject::NodeListType NodeListType;
  const NodeListType nodelist = femobject->GetNodeList();
  
  typename NodeListType::const_iterator it_nodes = nodelist.begin();
    
  while(it_nodes != nodelist.end())
  {
	  FEMObjectNode *node = (*it_nodes);
	  a = ObjectFactoryBase::CreateInstance ( "Node" );
	 
	  // create a new object of the correct class
	  //a = FEMOF::Create(clID);
	  itk::fem::Node::Pointer o1 = dynamic_cast< itk::fem::Node * >( &*a );
	  o1->SetGlobalNumber(node->m_GN);
	  itk::fem::Element::VectorType pt(node->m_Dim);
	  for (unsigned int i=0; i<node->m_Dim; i++)
	  {
		pt[i] = node->m_X[i];
	  }
	  o1->SetCoordinates(pt);
	  myFEMObject->AddNextNode(&*o1);
	  it_nodes++;    
  }
 
   // copy all the material information
   // as of now linear elastic material property is the only
   // material property type. other types could be added in the 
   // future.
  typedef typename MetaFEMObject::MaterialListType MaterialListType;
  const MaterialListType materiallist = femobject->GetMaterialList();
  
  typename MaterialListType::const_iterator it_material = materiallist.begin();
    
  while(it_material != materiallist.end())
  {
    FEMObjectMaterial *material = (*it_material);
    a = ObjectFactoryBase::CreateInstance ( "MaterialLinearElasticity" );

	  itk::fem::MaterialLinearElasticity::Pointer o1 = 
		dynamic_cast< itk::fem::MaterialLinearElasticity * >( &*a );
	  o1->SetGlobalNumber(material->m_GN);
	  o1->SetYoungsModulus(material->E); /* Young modulus */
	  o1->SetPoissonsRatio(material->nu);
	  o1->SetCrossSectionalArea(material->A);   /* Crossection area */
	  o1->SetMomentOfInertia(material->I);    /* Moment of inertia */
	  o1->SetThickness(material->h);
	  o1->SetDensityHeatProduct(material->RhoC);
	  myFEMObject->AddNextMaterial(&*o1);
	  it_material++;    
  } 
  
    // copy all the Element information
  typedef typename MetaFEMObject::ElementListType ElementListType;
  const ElementListType elementlist = femobject->GetElementList();
  
  typename ElementListType::const_iterator it_elements = elementlist.begin();
    
  while(it_elements != elementlist.end())
  {
    FEMObjectElement *element = (*it_elements);
    a = ObjectFactoryBase::CreateInstance ( element->m_ElementName );

	  itk::fem::Element::Pointer o1 = dynamic_cast< itk::fem::Element * >( &*a );
	    o1->SetGlobalNumber(element->m_GN);
	  int numNodes = element->m_NumNodes;
	  for (int i=0; i<numNodes; i++)
	  {
		o1->SetNode(i, &*myFEMObject->GetNodeWithGlobalNumber(element->m_NodesId[i]));
	  }
		o1->SetMaterial( &*myFEMObject->GetMaterialWithGlobalNumber(element->m_MaterialGN) );
		myFEMObject->AddNextElement( &*o1);
		  it_elements++;    
  } 

    // copy all the load and boundary condition information
  typedef typename MetaFEMObject::LoadListType LoadListType;
  const LoadListType loadlist = femobject->GetLoadList();
  
  typename LoadListType::const_iterator it_load = loadlist.begin();
     
   while(it_load != loadlist.end())
   {
     FEMObjectLoad *load = (*it_load);
     a = ObjectFactoryBase::CreateInstance ( load->m_LoadName );

	  std::string loadname = std::string(load->m_LoadName);
	  if(loadname == "LoadNode")
	  {
		  itk::fem::LoadNode::Pointer o1 = 
			dynamic_cast< itk::fem::LoadNode * >( &*a );
		  o1->SetGlobalNumber(load->m_GN);
		  
		  o1->SetElement(&*myFEMObject->GetElementWithGlobalNumber(load->m_ElementGN));
		  
		  o1->SetNode(load->m_NodeNumber);
		  
		  int dim = load->m_Dim;
		  vnl_vector< double > F(dim);
		  for (int i=0; i<dim; i++)
		  {
			F[i] = load->m_ForceVector[i];
		  }
		  o1->SetForce(F);
	      myFEMObject->AddNextLoad( &*o1);
	      goto out;
	  }

	  if(loadname == "LoadBC")
	  {
		  itk::fem::LoadBC::Pointer o1 = 
			dynamic_cast< itk::fem::LoadBC * >( &*a );
		  o1->SetGlobalNumber(load->m_GN);
		  
		  o1->SetDegreeOfFreedom(load->m_DOF);
		  
		  o1->SetElement(&*myFEMObject->GetElementWithGlobalNumber(load->m_ElementGN));
		  	  
		  int numRHS = load->m_NumRHS;
		  vnl_vector< double > F(numRHS);
		  for (int i=0; i<numRHS; i++)
		  {
			F[i] = load->m_RHS[i];
		  }
		  o1->SetValue(F);
	      myFEMObject->AddNextLoad( &*o1);
	      goto out;
	  }
	  
	  if(loadname == "LoadBCMFC")
	  {
		  itk::fem::LoadBCMFC::Pointer o1 = 
			dynamic_cast< itk::fem::LoadBCMFC * >( &*a );
		  o1->SetGlobalNumber(load->m_GN);

		int NumLHS;
		int elementGN;
		int DOF;
		float Value;
		NumLHS = load->m_NumLHS;
		
	for ( int i = 0; i < NumLHS; i++ )
    {
    FEMObjectMFCTerm *mfcTerm = dynamic_cast< FEMObjectMFCTerm * > (&*load->m_LHS[i]);
    elementGN = mfcTerm->m_ElementGN;

    DOF = mfcTerm->m_DOF;

    Value = mfcTerm->m_Value;
	
    o1->GetLeftHandSideArray().push_back( 
    itk::fem::LoadBCMFC::MFCTerm(&*myFEMObject->GetElementWithGlobalNumber(elementGN), DOF, Value) );
    }

	int NumRHS = load->m_NumRHS;
  
		for (int i=0; i<NumRHS; i++)
		{
		o1->GetRightHandSideArray().set_size(o1->GetRightHandSideArray().size() + 1);
		o1->GetRightHandSideArray().put(o1->GetRightHandSideArray().size() - 1, load->m_RHS[i]);
		}		  

	      myFEMObject->AddNextLoad( &*o1);
	      goto out;
	  }

	  if(loadname == "LoadEdge")
	  {
		itk::fem::LoadEdge::Pointer o1 = 
			dynamic_cast< itk::fem::LoadEdge * >( &*a );
		  o1->SetGlobalNumber(load->m_GN);
		  
		 int numRows;
				
		o1->AddNextElement(&*myFEMObject->GetElementWithGlobalNumber(load->m_ElementGN));
		o1->SetGlobalNumber(load->m_GN);
		o1->SetEdge(load->m_EdgeNumber);
		
		METAIO_STL::vector< METAIO_STL::vector<float> > force = load->m_ForceMatrix;
				
		numRows = force.size();
		if(numRows)
		{
			METAIO_STL::vector<float> forcevector = force[0];
			int numCols = forcevector.size();
			o1->GetForce().set_size(numRows, numCols);
			for ( int i = 0; i < numRows; i++ )
			{
				forcevector = force[i];
				for ( int j = 0; j < numCols; j++ )
				{
					o1->GetForce()[i][j] = forcevector[j];
				}
			}
	      myFEMObject->AddNextLoad( &*o1);
		}
		goto out;
	  }	
	  
	  if(loadname == "LoadGravConst")
	  {
		itk::fem::LoadGravConst::Pointer o1 = 
			dynamic_cast< itk::fem::LoadGravConst * >( &*a );
		  o1->SetGlobalNumber(load->m_GN);
		
		for (int i=0; i<load->m_NumElements; i++)
		{
			o1->GetElementArray().push_back(&*myFEMObject->GetElementWithGlobalNumber(load->m_Elements[i]));
		}
	    
	    o1->GetForce().set_size(load->m_Dim);	
		for(int i=0; i<load->m_Dim; i++)
		{
			o1->GetForce()[i] = load->m_ForceVector[i];
		}				  						
	      myFEMObject->AddNextLoad( &*o1);
	      goto out;
	  }	  
 
	if(loadname == "LoadLandmark")
	  {
		itk::fem::LoadLandmark::Pointer o1 = 
			dynamic_cast< itk::fem::LoadLandmark * >( &*a );
		  o1->SetGlobalNumber(load->m_GN);
		  o1->SetEta(load->m_Variance);
		  o1->GetElementArray().resize(1);
		  		  
		  int dim = load->m_Undeformed.size();
		  vnl_vector<double> source;
      vnl_vector<double> target;
      vnl_vector<double> point;
      vnl_vector<double> force;
      
      source.set_size(dim);
      target.set_size(dim);
      point.set_size(dim);
      force.set_size(dim);
      for (int i=0; i<dim; i++)
		  {
        source[i] = load->m_Deformed[i];
        target[i] = load->m_Undeformed[i];
        point[i]  = load->m_Deformed[i];
        force[i] = load->m_Undeformed[i] - load->m_Deformed[i];
        
      }
      //FIXME - Check Source and Target
      o1->SetSource( source );
      o1->SetTarget( target );
      o1->SetPoint( point );
      o1->SetForce( force );
      
      /*
		  o1->GetSource().set_size(dim);
		  o1->GetPoint().set_size(dim);
		  o1->GetTarget().set_size(dim);
		  o1->GetForce().set_size(dim);
		  
		  for (int i=0; i<dim; i++)
		  {
			o1->GetSource()[i] = load->m_Deformed[i];
			o1->GetPoint()[i] = load->m_Deformed[i];
			o1->GetTarget()[i] = load->m_Undeformed[i];
			o1->GetForce()[i] = load->m_Undeformed[i] - load->m_Deformed[i];
		  }
      */
		 myFEMObject->AddNextLoad( &*o1);		  
	  } 
	  out:
	  it_load++;    
  } 
    
  spatialObject->SetFEMObject(myFEMObject);
	
  return spatialObject;
}

/** Convert an FEMObject SpatialObject into a metaFEMObject */
template <unsigned int NDimensions>
MetaFEMObject*
MetaFEMObjectConverter<NDimensions>
::FEMObjectSpatialObjectToMetaFEMObject(SpatialObjectType * spatialObject)
{ 
  typedef itk::fem::FEMObject<NDimensions>  FEMObjectType;
  typedef typename FEMObjectType::Pointer   FEMObjectPointer;
  typedef itk::fem::FEMObjectFactory< itk::fem::FEMLightObject > FEMOF;

  FEMObjectPointer SOFEMObject = spatialObject->GetFEMObject();

  MetaFEMObject* FEMObject = new MetaFEMObject(NDimensions);
  
  // copy the relevant info from spatial object to femobject
  
  // copy node info.
  int numNodes = SOFEMObject->GetNumberOfNodes();
  for (int i=0; i<numNodes; i++)
  {
	FEMObjectNode *Node = new FEMObjectNode(NDimensions);
	itk::fem::Node::Pointer SONode = SOFEMObject->GetNode(i);
	itk::fem::Element::VectorType pt = SONode->GetCoordinates();
	
	Node->m_GN = SONode->GetGlobalNumber();
	for (unsigned int j=0; j<NDimensions; j++)
	{
		Node->m_X[j] = pt[j];
	}
	FEMObject->GetNodeList().push_back(Node);	
  }

   // copy material info.
   int numMaterial = SOFEMObject->GetNumberOfMaterials();
   for (int i=0; i<numMaterial; i++)
   {
   	itk::fem::Material::Pointer SOMaterial = SOFEMObject->GetMaterial(i);
	FEMObjectMaterial *Material = new FEMObjectMaterial;
	
	// check for the material type
  std::string mat_name = SOMaterial->GetNameOfClass();
	if(mat_name == "MaterialLinearElasticity")
	{
	    strcpy(Material->m_MaterialName, mat_name.c_str());
		itk::fem::MaterialLinearElasticity::Pointer SOMaterialCast = 
		dynamic_cast< itk::fem::MaterialLinearElasticity * >( &*SOMaterial );
		
		Material->m_GN = SOMaterialCast->GetGlobalNumber();
		Material->E = SOMaterialCast->GetYoungsModulus();
		Material->A = SOMaterialCast->GetCrossSectionalArea();
		Material->I = SOMaterialCast->GetMomentOfInertia();
		Material->nu = SOMaterialCast->GetPoissonsRatio();
		Material->h = SOMaterialCast->GetThickness();
		Material->RhoC = SOMaterialCast->GetDensityHeatProduct();
		FEMObject->GetMaterialList().push_back(Material);	
	}	
   }
   
   // copy element info.
  int numElements = SOFEMObject->GetNumberOfElements();
  for (int i=0; i<numElements; i++)
  {
  	itk::fem::Element::Pointer SOElement = SOFEMObject->GetElement(i);
  	int numNodes = SOElement->GetNumberOfNodes();
	FEMObjectElement *Element = new FEMObjectElement(numNodes);

	Element->m_GN = SOElement->GetGlobalNumber();
	Element->m_Dim = NDimensions;
	Element->m_NumNodes = numNodes;

  std::string element_name = SOElement->GetNameOfClass();
	strcpy(Element->m_ElementName, element_name.c_str());
	Element->m_MaterialGN = SOElement->GetMaterial()->GetGlobalNumber();	
	for (int j=0; j<numNodes; j++)
	{
		Element->m_NodesId[j] = SOElement->GetNode(j)->GetGlobalNumber();
	}
	FEMObject->GetElementList().push_back(Element);	
  } 

  // copy load/bc info.
    int numLoads = SOFEMObject->GetNumberOfLoads();
   for (int i=0; i<numLoads; i++)
   {
   	itk::fem::Load::Pointer SOLoad = SOFEMObject->GetLoad(i);
	FEMObjectLoad *Load = new FEMObjectLoad;
	
	// check for the load/bc type
  std::string load_name = SOLoad->GetNameOfClass();
	strcpy(Load->m_LoadName, load_name.c_str());
	if(load_name == "LoadNode")
	{
		  itk::fem::LoadNode::Pointer SOLoadCast = 
			dynamic_cast< itk::fem::LoadNode * >( &*SOLoad );
			
		  Load->m_GN = SOLoadCast->GetGlobalNumber();
		  Load->m_ElementGN = SOLoadCast->GetElement()->GetGlobalNumber();		  		  
		  Load->m_NodeNumber = SOLoadCast->GetNode();
		  		  
		  int dim = SOLoadCast->GetForce().size();
		  Load->m_ForceVector.resize(dim);
		  Load->m_Dim = dim;
		  for (int j=0; j<dim; j++)
		  {
			Load->m_ForceVector[j] = SOLoadCast->GetForce()[j];
		  }
		FEMObject->GetLoadList().push_back(Load);			  
	}
	
	if(load_name == "LoadBC")
	{
		  itk::fem::LoadBC::Pointer SOLoadCast = 
			dynamic_cast< itk::fem::LoadBC * >( &*SOLoad );
			
		  Load->m_GN = SOLoadCast->GetGlobalNumber();
		  Load->m_DOF = SOLoadCast->GetDegreeOfFreedom();
		  Load->m_ElementGN = SOLoadCast->GetElement()->GetGlobalNumber();
		  		  		  	  
		  int numRHS = SOLoadCast->GetValue().size();
		  Load->m_RHS.resize(numRHS);
		  Load->m_NumRHS = numRHS;	  
		  for (int j=0; j<numRHS; j++)
		  {
			Load->m_RHS[j] = SOLoadCast->GetValue()[j];
		  } 
		FEMObject->GetLoadList().push_back(Load);			  
	}
	
	if(load_name == "LoadBCMFC")
	{
		int elementGN;
		int DOF;
		float Value;
	   
	  itk::fem::LoadBCMFC::Pointer SOLoadCast = 
		dynamic_cast< itk::fem::LoadBCMFC * >( &*SOLoad );
		
		Load->m_GN = SOLoadCast->GetGlobalNumber();
			
		Load->m_NumLHS = SOLoadCast->GetNumberOfLeftHandSideTerms();

		for ( int i = 0; i < Load->m_NumLHS; i++ )
		{
			/** set the global number of element that we're applying the load to */
			elementGN = SOLoadCast->GetLeftHandSideTerm(i).m_element->GetGlobalNumber();
			
			/** set the dof within that element */
			DOF = SOLoadCast->GetLeftHandSideTerm(i).dof;

			/** set weight */
			Value = SOLoadCast->GetLeftHandSideTerm(i).value;

			/** add a new MFCTerm to the lhs */
			FEMObjectMFCTerm *mfcTerm = new FEMObjectMFCTerm(elementGN, DOF, Value);
			Load->m_LHS.push_back(mfcTerm);
		}

		/** set the rhs */
		Load->m_NumRHS = SOLoadCast->GetNumberOfRightHandSideTerms();
		Load->m_RHS.resize(Load->m_NumRHS);
		for (int i=0; i<Load->m_NumRHS; i++)
		{
			Load->m_RHS[i] = SOLoadCast->GetRightHandSideTerm(i);
		}
		FEMObject->GetLoadList().push_back(Load);			  
	}
	
	if(load_name == "LoadEdge")
	{
		itk::fem::LoadEdge::Pointer SOLoadCast = 
		dynamic_cast< itk::fem::LoadEdge * >( &*SOLoad );
		
		int numRows, numCols;

		Load->m_GN = SOLoadCast->GetGlobalNumber();
		
		Load->m_ElementGN = SOLoadCast->GetElementArray()[0]->GetGlobalNumber();
		Load->m_EdgeNumber = SOLoadCast->GetEdge();
		
		vnl_matrix< itk::fem::Element::Float > force = SOLoadCast->GetForce();
		
		numRows = force.rows();
		numCols = force.columns();

		for ( int i = 0; i < numRows; i++ )
		{
			METAIO_STL::vector<float> F(numCols);
			for ( int j = 0; j < numCols; j++ )
			{
				F[j] = force[i][j];
			}
			Load->m_ForceMatrix.push_back(F);
		}
		FEMObject->GetLoadList().push_back(Load);			  
	}
	
	if(load_name == "LoadGravConst")
	{
		itk::fem::LoadGravConst::Pointer SOLoadCast = 
		dynamic_cast< itk::fem::LoadGravConst * >( &*SOLoad );
		
		Load->m_GN = SOLoadCast->GetGlobalNumber();
		
		int numElements	= SOLoadCast->GetElementArray().size();
		Load->m_NumElements = numElements;	
		int elementGN;
		for (int i=0; i<numElements; i++)
		{
			elementGN = SOLoadCast->GetElementArray()[i]->GetGlobalNumber();
			Load->m_Elements.push_back(elementGN);
		}
		
		Load->m_Dim = SOLoadCast->GetForce().size();		
		for (int i=0; i<Load->m_Dim; i++)
		{
			Load->m_ForceVector.push_back(SOLoadCast->GetForce()[i]);
		}
				
		FEMObject->GetLoadList().push_back(Load);			  
	}				
	
	if(load_name == "LoadLandmark")
	{
		itk::fem::LoadLandmark::Pointer SOLoadCast = 
		dynamic_cast< itk::fem::LoadLandmark * >( &*SOLoad );
		
		Load->m_GN = SOLoadCast->GetGlobalNumber();
	
		Load->m_Variance = SOLoadCast->GetEta();
		  		  
		int dim = SOLoadCast->GetSource().size();
		
		Load->m_Undeformed.resize(dim);
		Load->m_Deformed.resize(dim);
		  		  
	  for (int i=0; i<dim; i++)
	  {
		Load->m_Deformed[i] = SOLoadCast->GetSource()[i];
		Load->m_Undeformed[i] = SOLoadCast->GetTarget()[i];
	  }	
	  FEMObject->GetLoadList().push_back(Load);			  
	}			
   }  
  FEMObject->ID(spatialObject->GetId());
  if(spatialObject->GetParent())
    {
    FEMObject->ParentID(spatialObject->GetParent()->GetId());
    }
  return FEMObject;
}


/** Read a meta file give the type */
template <unsigned int NDimensions> 
typename MetaFEMObjectConverter<NDimensions>::SpatialObjectPointer
MetaFEMObjectConverter<NDimensions>
::ReadMeta(const char* name)
{
  SpatialObjectPointer spatialObject;
  MetaFEMObject* FEMObject = new MetaFEMObject();
  FEMObject->Read(name);
  FEMObject->PrintInfo();

  spatialObject = MetaFEMObjectToFEMObjectSpatialObject(FEMObject);

  return spatialObject;
}


/** Write a meta FEMObject file */
template <unsigned int NDimensions>   
bool
MetaFEMObjectConverter<NDimensions>
::WriteMeta(SpatialObjectType* spatialObject,const char* name)
{
  MetaFEMObject* FEMObject = FEMObjectSpatialObjectToMetaFEMObject(spatialObject);
  FEMObject->Write(name);
  return true;
}

} // end namespace itk 


#endif
