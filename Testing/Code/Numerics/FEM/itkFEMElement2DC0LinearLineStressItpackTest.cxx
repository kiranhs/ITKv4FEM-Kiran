/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFEMElement2DStrainTest.cxx
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

#include "itkFEM.h"
#include "itkFEM.h"
#include "itkFEMLinearSystemWrapperItpack.h"
#include "itkFEMLinearSystemWrapperVNL.h"
#include "itkFEMSolver.h"

#include <iostream>
using std::ofstream;
using std::ifstream;

//  Example taken from 'Fundamentals of the Finite ELement Method' - Grandin
int itkFEMElement2DC0LinearQuadrilateralStrainItpackTest(int argc, char *argv[])
{
  typedef itk::fem::Solver SolverType;
  typedef SolverType *     SolverPointerType;
  SolverPointerType m_Solver = new SolverType;
  ifstream          fileInput;

  itk::fem::LinearSystemWrapperItpack WrapperItpack;
  WrapperItpack.SetMaximumNonZeroValuesInMatrix(100);

  fileInput.open("C:/Research/ITKGit/ITK/Testing/Data/Input/FEM/2DC0LinearQuadrilateralStrainTest.fem");
  m_Solver->Read(fileInput);
  m_Solver->GenerateGFN();
  m_Solver->SetLinearSystemWrapper(&WrapperItpack);
  m_Solver->AssembleK();
  m_Solver->DecomposeK();
  m_Solver->AssembleF();
  m_Solver->Solve();

  float soln[8];
  for ( int i = 0; i < 8; i++ )
    {
    soln[i] = m_Solver->GetSolution(i);
    }

  std::cout << "Test PASSED!" << std::endl;
  return EXIT_SUCCESS;
}
