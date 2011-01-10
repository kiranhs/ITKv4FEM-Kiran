/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFEMLandmarkLoadImplementationTest.cxx
  Language:  C++
  Date: $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

// disable debug warnings in MS compiler
#ifdef _MSC_VER
#pragma warning(disable: 4786)
#endif

#include <iostream>
#include "itkFEMLoadImplementationGenericLandmarkLoad.h"
#include "itkFEMElement2DC0LinearQuadrilateralMembrane.h"
#include "itkFEM.h"
#include "itkFEMLinearSystemWrapperItpack.h"
#include "itkFEMSolver.h"

#include <iostream>
using std::ofstream;
using std::ifstream;

//
int itkFEMLandmarkLoadImplementationTest(int, char *[])
{
  typedef itk::fem::Solver SolverType;
  typedef SolverType *     SolverPointerType;
  SolverPointerType S = new SolverType;
  S->InitializeLinearSystemWrapper();

  itk::fem::Node::Pointer n1;

  n1 = itk::fem::Node::New();
  itk::fem::Element::VectorType pt(2);

  pt[0] = 0.;
  pt[1] = 0.;
  n1->SetGlobalNumber(0);
  n1->SetCoordinates(pt);

  S->AddNextNode( itk::fem::FEMP< itk::fem::Node >(&*n1) );

  n1 = itk::fem::Node::New();
  pt[0] = 1.;
  pt[1] = 1.;
  n1->SetGlobalNumber(1);
  n1->SetCoordinates(pt);
  S->AddNextNode( itk::fem::FEMP< itk::fem::Node >(&*n1) );

  n1 = itk::fem::Node::New();
  pt[0] = 3.;
  pt[1] = 2.;
  n1->SetGlobalNumber(2);
  n1->SetCoordinates(pt);
  S->AddNextNode( itk::fem::FEMP< itk::fem::Node >(&*n1) );

  n1 = itk::fem::Node::New();
  pt[0] = 0.;
  pt[1] = 3.;
  n1->SetGlobalNumber(3);
  n1->SetCoordinates(pt);
  S->AddNextNode( itk::fem::FEMP< itk::fem::Node >(&*n1) );

  S->GetNodeArray().Renumber();

  //std::cout << "Nodes\n";

  itk::fem::MaterialLinearElasticity::Pointer m;
  m = itk::fem::MaterialLinearElasticity::New();
  m->SetGlobalNumber(0);
  m->SetYoungsModulus(30000.0);
  m->SetCrossSectionalArea(0.02);
  m->SetMomentOfInertia(0.004);
  S->AddNextMaterial( itk::fem::FEMP< itk::fem::Material >(&*m) );

  //std::cout << "Material\n";

  itk::fem::Element2DC0LinearQuadrilateralMembrane::Pointer e0 =
    itk::fem::Element2DC0LinearQuadrilateralMembrane::New();

  e0->SetGlobalNumber(0);
  e0->SetNode( 0, &*S->GetNode(0) );
  e0->SetNode( 1, &*S->GetNode(1) );
  e0->SetNode( 2, &*S->GetNode(2) );
  e0->SetNode( 3, &*S->GetNode(3) );
  e0->m_mat = dynamic_cast< itk::fem::MaterialLinearElasticity * >( &*S->GetMaterial(0) );

  //std::cout << "Element\n";

  itk::fem::LoadBC::Pointer l1 = itk::fem::LoadBC::New();
  l1->SetElement(&*e0);
  l1->SetGlobalNumber(0);
  l1->SetDegreeOfFreedom(0);
  l1->SetValue( vnl_vector< double >(1, 0.0) );
  S->AddNextLoad( itk::fem::FEMP< itk::fem::Load >(*&l1) );

  //std::cout << "BC\n";

  itk::fem::LoadLandmark::Pointer lm0 = itk::fem::LoadLandmark::New();
  lm0->SetEta(0.01);
  lm0->SetGlobalNumber(1);
  lm0->SetPoint( vnl_vector< double >(0., 0.) );
  lm0->SetTarget( vnl_vector< double >(0., 1.) );
  lm0->SetForce( lm0->GetTarget() / ( lm0->GetEta() * lm0->GetEta() ) );
  lm0->AddNextElement(&*e0);
  S->AddNextLoad( itk::fem::FEMP< itk::fem::Load >(&*lm0) );

  S->AddNextElement( itk::fem::FEMP< itk::fem::Element >(&*e0) );
  //   std::cout << "Landmark\n";

  //std::cout << "Assembled\n";

  /*   try
       { */
  ofstream fileOutput;
  fileOutput.open("LandmarkLoadImplementation.fem", std::ios::out);
  S->Write(fileOutput);

  S->GenerateGFN();
  S->AssembleK();
  S->DecomposeK();
  S->AssembleF();
  S->Solve();

  /*    }*/
  /*catch (ExceptionObject &e)
    {
    std::cerr << "Exception caught: " << e << "\n";
    return EXIT_FAILURE;
    }*/

  std::cout << "Test PASSED!\n";
  return EXIT_SUCCESS;
}
