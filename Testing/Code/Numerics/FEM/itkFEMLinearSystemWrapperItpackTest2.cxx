/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFEMLinearSystemWrapperItpackTest2.cxx
  Language:  C++
  Date:      $Date$
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

#include "itkFEMLinearSystemWrapperItpack.h"
#include <iostream>
#include <stdlib.h>

/* Testing for linear system wrappers */
int itkFEMLinearSystemWrapperItpackTest2(int argc, char *argv[])
{
  /* loop vars for printing */
  unsigned int i;
  unsigned int j;

  /* declare wrapper */
  itk::fem::LinearSystemWrapperItpack it;

  /* system parameters */
  unsigned int N = 3;
  unsigned int nMatrices =  1;
  unsigned int nVectors =   1;
  unsigned int nSolutions = 1;

  /* Set up the system */
  it.SetSystemOrder(N);
  it.SetNumberOfMatrices(nMatrices);
  it.SetNumberOfVectors(nVectors);
  it.SetNumberOfSolutions(nSolutions);

  /* Set max non zeros in any matrix */
  it.SetMaximumNonZeroValuesInMatrix(9);

  /* Initialize memory */
  for ( i = 0; i < nMatrices; i++ )
    {
    it.InitializeMatrix(i);
    }
  for ( i = 0; i < nVectors; i++ )
    {
    it.InitializeVector(i);
    }
  for ( i = 0; i < nSolutions; i++ )
    {
    it.InitializeSolution(i);
    }

  /*     matrix 0
   * |11  0  0|
   * | 0 22  0|
   * | 0  0 33|
   */
  it.SetMatrixValue(0, 0, 11, 0);
  it.SetMatrixValue(1, 1, 22, 0);
  it.SetMatrixValue(2, 2, 33, 0);

  /* print matrix 0 */
  std::cout << "Matrix 0" << std::endl;
  for ( i = 0; i < N; i++ )
    {
    for ( j = 0; j < N; j++ )
      {
      std::cout << it.GetMatrixValue(i, j, 0) << " ";
      }
    std::cout << std::endl;
    }
  std::cout << std::endl;

  /* Vector 0 = [1 2 3 ] */
  it.SetVectorValue(0, 1, 0);
  it.SetVectorValue(1, 2, 0);
  it.SetVectorValue(2, 3, 0);

  /* print Vector 0 */
  std::cout << "Vector 0" << std::endl;
  for ( i = 0; i < N; i++ )
    {
    std::cout << it.GetVectorValue(i, 0) << " ";
    }
  std::cout << std::endl << std::endl;

  if ( argc > 1 )
    {
    int method = atoi(argv[1]);
    switch ( method )
      {
      case 0:
        it.JacobianConjugateGradient();
        break;
      case 1:
        it.JacobianSemiIterative();
        break;
      case 2:
        it.SuccessiveOverrelaxation();
        break;
      case 3:
        it.SymmetricSuccessiveOverrelaxationConjugateGradient();
        break;
      case 4:
        it.SymmetricSuccessiveOverrelaxationSuccessiveOverrelaxation();
        break;
      case 5:
        it.ReducedSystemConjugateGradient();
        break;
      case 6:
        it.ReducedSystemSemiIteration();
        break;
      }
    }

  /* solve system */
  std::cout << "Solve for x in: Matrix 0 * x = Vector 0" << std::endl;
  it.Solve();
  std::cout << "Solution 0" << std::endl;
  for ( i = 0; i < N; i++ )
    {
    std::cout << it.GetSolutionValue(i, 0) << " ";
    }
  std::cout << std::endl << std::endl;

  /* destroy matrix,vector,solution */
  it.DestroyMatrix(0);
  it.DestroyVector(0);
  it.DestroySolution(0);

  std::cout << "Done." << std::endl;

  return EXIT_SUCCESS;
}
