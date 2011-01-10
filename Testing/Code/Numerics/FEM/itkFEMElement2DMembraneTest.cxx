/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFEMElement2DMembraneTest.cxx
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

#include "itkFEMElement2DC0LinearQuadrilateralMembrane.h"
#include "itkFEMElementBase.h"

#include <iostream>

//
int itkFEMElement2DMembraneTest(int, char *argv[])
{
  typedef itk::fem::Node    NodeType;
  typedef itk::fem::Element ElementType;

  typedef itk::fem::MaterialLinearElasticity ElasticityType;

  NodeType::Pointer       n0, n1, n2, n3;
  ElementType::VectorType pt(2);

  n0 = NodeType::New();
  pt[0] = 0.;
  pt[1] = 0.;
  n0->SetCoordinates(pt);

  n1 = NodeType::New();
  pt[0] = 1.;
  pt[1] = 1.;
  n1->SetCoordinates(pt);

  n2 = NodeType::New();
  pt[0] = 3.;
  pt[1] = 2.;
  n2->SetCoordinates(pt);

  n3 = NodeType::New();
  pt[0] = 0.;
  pt[1] = 3.;
  n3->SetCoordinates(pt);

  ElasticityType::Pointer m;
  m = ElasticityType::New();
  m->SetGlobalNumber(0);
  m->SetYoungsModulus(3000.0);
  m->SetCrossSectionalArea(0.02);
  m->SetMomentOfInertia(0.004);

  typedef itk::fem::Element2DC0LinearQuadrilateralMembrane MembraneElementType;
  MembraneElementType::Pointer e0 = MembraneElementType::New();

  e0->SetGlobalNumber(0);
  e0->SetNode(0, &*n0);
  e0->SetNode(1, &*n1);
  e0->SetNode(2, &*n2);
  e0->SetNode(3, &*n3);
  e0->SetMaterial( dynamic_cast< ElasticityType * >( &*m ) );

  ElementType::MatrixType D;
  ElementType::MatrixType Me;

  e0->GetMassMatrix(Me);
  e0->GetMaterialMatrix(D);
  std::cout << "Mass matrix: " << std::endl << Me << std::endl;
  std::cout << "Material matrix: " << std::endl << D << std::endl;
  std::cout << "#dof per node = " << e0->GetNumberOfDegreesOfFreedomPerNode() << std::endl;

#ifndef FEM_USE_SMART_POINTERS
  delete e0;
  delete m;
  delete n0;
  delete n1;
  delete n2;
  delete n3;
#endif

  std::cout << "Test PASSED!\n";
  return EXIT_SUCCESS;
}
