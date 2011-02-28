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

#include "itkFEM.h"
#include "itkFEMSolver1.h"
#include "itkFEMObject.h"
#include "itkFEMObjectSpatialObject.h"
#include "itkGroupSpatialObject.h"
#include "itkSpatialObject.h"
#include "itkSpatialObjectReader.h"
#include "itkSpatialObjectWriter.h"

int itkFEMLoadBCMFCTestUser(int argc, char *argv[])
{
  typedef itk::fem::Solver1<2>    Solver2DType;
  Solver2DType::Pointer solver = Solver2DType::New();
  
	
	typedef itk::fem::FEMObject<2> FEMObjectType;
	FEMObjectType::Pointer femObject = FEMObjectType::New();

  itk::fem::LinearSystemWrapperVNL vnlSolver;

  vnlSolver.SetMaximumNonZeroValuesInMatrix(1000, 1000);

  itk::fem::Node::Pointer n1;

  itk::fem::Element::VectorType pt(2);

  n1 = itk::fem::Node::New();
  pt[0] = 0.0;
  pt[1] = 0.0;
  n1->SetCoordinates(pt);
  n1->SetGlobalNumber(0);
  femObject->AddNextNode(&*n1);

  n1 = itk::fem::Node::New();
  pt[0] = 1500.0;
  pt[1] = 0.0;
  n1->SetCoordinates(pt);
  n1->SetGlobalNumber(1);
  femObject->AddNextNode(&*n1);

  n1 = itk::fem::Node::New();
  pt[0] = 3000.0;
  pt[1] = 0.0;
  n1->SetCoordinates(pt);
  n1->SetGlobalNumber(2);
  femObject->AddNextNode(&*n1);

  n1 = itk::fem::Node::New();
  pt[0] = 3000.0;
  pt[1] = 3000.0;
  n1->SetCoordinates(pt);
  n1->SetGlobalNumber(3);
  femObject->AddNextNode(&*n1);

  n1 = itk::fem::Node::New();
  pt[0] = 0.0;
  pt[1] = 4500.0;
  n1->SetCoordinates(pt);
  n1->SetGlobalNumber(4);
  femObject->AddNextNode(&*n1);

  itk::fem::MaterialLinearElasticity::Pointer m;
  m = itk::fem::MaterialLinearElasticity::New();
  m->SetGlobalNumber(0);               /* Global number of the material */
  m->SetYoungsModulus(200000000000.0); /* Young modulus */
  m->SetPoissonsRatio(0.3);
  m->SetCrossSectionalArea(2000.0); /* Crossection area */
  m->SetMomentOfInertia(1.0);      /* Momemt of inertia */
  femObject->AddNextMaterial(&*m);

  m = itk::fem::MaterialLinearElasticity::New();
  m->SetGlobalNumber(1);         /* Global number of the material */
  m->SetYoungsModulus(200000.0); /* Young modulus */
  m->SetPoissonsRatio(0.3);
  m->SetCrossSectionalArea(1200.0); /* Crossection area */
  m->SetMomentOfInertia(1.0);      /* Momemt of inertia */
  femObject->AddNextMaterial(&*m);

  m = itk::fem::MaterialLinearElasticity::New();
  m->SetGlobalNumber(2);        /* Global number of the material */
  m->SetYoungsModulus(70000.0); /* Young modulus */
  m->SetPoissonsRatio(0.3);
  m->SetCrossSectionalArea(900.0); /* Crossection area */
  m->SetMomentOfInertia(1.0);     /* Momemt of inertia */
  femObject->AddNextMaterial(&*m);

  itk::fem::Element2DC0LinearLineStress::Pointer e1;

  e1 = itk::fem::Element2DC0LinearLineStress::New();
  e1->SetGlobalNumber(0);
  e1->SetNode( 0, &*femObject->GetNode(0) );
  e1->SetNode( 1, &*femObject->GetNode(1) );
  e1->m_mat = dynamic_cast< itk::fem::MaterialLinearElasticity * >( &*femObject->GetMaterial(0) );
  femObject->AddNextElement( &*e1);

  e1 = itk::fem::Element2DC0LinearLineStress::New();
  e1->SetGlobalNumber(1);
  e1->SetNode( 0, &*femObject->GetNode(1) );
  e1->SetNode( 1, &*femObject->GetNode(2) );
  e1->SetMaterial( dynamic_cast< itk::fem::MaterialLinearElasticity * >( &*femObject->GetMaterial(0) ) );
  femObject->AddNextElement( &*e1);

  e1 = itk::fem::Element2DC0LinearLineStress::New();
  e1->SetGlobalNumber(2);
  e1->SetNode( 0, &*femObject->GetNode(1) );
  e1->SetNode( 1, &*femObject->GetNode(3) );
  e1->SetMaterial( dynamic_cast< itk::fem::MaterialLinearElasticity * >( &*femObject->GetMaterial(2) ) );
  femObject->AddNextElement( &*e1);

  e1 = itk::fem::Element2DC0LinearLineStress::New();
  e1->SetGlobalNumber(3);
  e1->SetNode( 0, &*femObject->GetNode(0) );
  e1->SetNode( 1, &*femObject->GetNode(4) );
  e1->SetMaterial( dynamic_cast< itk::fem::MaterialLinearElasticity * >( &*femObject->GetMaterial(1) ) );
  femObject->AddNextElement( &*e1);

  itk::fem::LoadBC::Pointer l1;

  l1 = itk::fem::LoadBC::New();
  l1->SetGlobalNumber(0);
  l1->SetElement( &*femObject->GetElement(2) );
  l1->SetDegreeOfFreedom(2);
  l1->SetValue( vnl_vector< double >(1, 0.0) );
  femObject->AddNextLoad( &*l1);

  l1 = itk::fem::LoadBC::New();
  l1->SetGlobalNumber(1);
  l1->SetElement( &*femObject->GetElement(2) );
  l1->SetDegreeOfFreedom(3);
  l1->SetValue( vnl_vector< double >(1, 0.0) );
  femObject->AddNextLoad( &*l1);

  l1 = itk::fem::LoadBC::New();
  l1->SetGlobalNumber(2);
  l1->SetElement( &*femObject->GetElement(3) );
  l1->SetDegreeOfFreedom(2);
  l1->SetValue( vnl_vector< double >(1, 0.0) );
  femObject->AddNextLoad( &*l1);

  l1 = itk::fem::LoadBC::New();
  l1->SetGlobalNumber(3);
  l1->SetElement( &*femObject->GetElement(3) );
  l1->SetDegreeOfFreedom(3);
  l1->SetValue( vnl_vector< double >(1, 0.0) );
  femObject->AddNextLoad( &*l1);

  itk::fem::LoadNode::Pointer l2;

  l2 = itk::fem::LoadNode::New();
  l2->SetGlobalNumber(4);
  l2->SetElement( &*femObject->GetElement(1) );
  l2->SetNode(0);
  vnl_vector< double > F(2);
  F[0] = 0;
  F[1] = 30000;
  l2->SetForce(F);
  femObject->AddNextLoad( &*l2 );

  itk::fem::LoadBCMFC::Pointer bcmfc = itk::fem::LoadBCMFC::New();
  bcmfc->SetGlobalNumber(5);
//	itk::fem::LoadBCMFC bcmfc;
  bcmfc->AddLeftHandSideTerm( itk::fem::LoadBCMFC::MFCTerm(&*femObject->GetElement(0), 1, 1) );
  bcmfc->AddLeftHandSideTerm( itk::fem::LoadBCMFC::MFCTerm(&*femObject->GetElement(1), 3, -1) );
  bcmfc->AddRightHandSideTerm(0.0);
  femObject->AddNextLoad( &*bcmfc );
	femObject->FinalizeMesh();

  solver->SetInput( femObject );
  solver->Update( );
  
 
  int numDOF = femObject->GetNumberOfDegreesOfFreedom();
  vnl_vector<float> soln(numDOF);
  float exectedResult[10] = {0.283525, 0.0, 0.283525, 1.70115, 0.283525, 0.0, 0.0, 0.0, 0.0, 0.0};

  bool foundError = false;
  for ( int i = 0; i < numDOF; i++ )
  {
	  soln[i] = femObject->GetSolution(i);
	  //std::cout << "Solution[" << i << "]:" << soln[i] << std::endl;
	  if (abs(exectedResult[i]-soln[i]) > 0.000001)
	  {
	    std::cout << "ERROR: Index " << i << ". Expected " << exectedResult[i] << " Solution " << soln[i] << std::endl;
	    foundError = true;
	  }
  }
  
  if (foundError)
  {
    std::cout << "Test FAILED!" << std::endl;
    return EXIT_FAILURE;
  }
 
	// to write the deformed mesh
  // Testing the fe mesh validity
 /* typedef itk::FEMObjectSpatialObject<2>    FEMObjectSpatialObjectType;
	FEMObjectSpatialObjectType::Pointer femSODef = FEMObjectSpatialObjectType::New();
	femSODef->SetFEMObject(solver->GetOutput());
	typedef itk::SpatialObjectWriter<2>    SpatialObjectWriterType;
	typedef SpatialObjectWriterType::Pointer            SpatialObjectWriterPointer;
	SpatialObjectWriterPointer SpatialWriter = SpatialObjectWriterType::New();
	SpatialWriter->SetInput(femSODef);
	SpatialWriter->SetFileName( argv[2] );
	SpatialWriter->Update();*/
  std::cout << "Test PASSED!" << std::endl;
  return EXIT_SUCCESS;
}
