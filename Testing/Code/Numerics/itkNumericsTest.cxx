/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkNumericsTest.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include <iostream>

#include "vnl/vnl_matrix.h"
#include <vnl/algo/vnl_svd.h>



template <typename T>
void print_vnl_matrix(T& mat)
{
  std::cout << mat;
  for(unsigned int r = 0; r < mat.rows(); r++)
    {
    for(unsigned int c = 0; c < mat.columns(); c++)
      std::cout << mat(r, c) << " ";
    std::cout << std::endl;
    }
}


template <class D>  // D is often double or float
vnl_matrix<D> solve_with_warning(vnl_matrix<D>const& M,
         vnl_matrix<D>const& B)
{
  // Take svd of vnl_matrix<D> M, setting singular values
  // smaller than 1e-8 to 0, and hold the result.
  vnl_svd<D> svd(M, 1e-8);
  // Check for rank-deficiency
  if (svd.singularities() > 1)
    std::cout << "Warning: Singular matrix, condition = " << svd.well_condition() << std::endl;
  return svd.solve(B);
}


int test_svd() {
  double data[] = { 1, 1, 1,  1, 2, 3,  1, 3, 6};
  vnl_matrix<double> M (data, 3, 3);
  vnl_matrix<double> B (3, 1, 7.0); // column vector [7 7 7]^T
  vnl_matrix<double> result = solve_with_warning(M,B);
  std::cout << "Original svd problem solution" << std::endl;
  print_vnl_matrix(result);
  M(2,2)=5; result = solve_with_warning(M,B);
  std::cout << std::endl << "Modified svd problem solution" << std::endl;
  print_vnl_matrix(result);
  return 0;
}

int itkNumericsTest(int, char* [] )
{
  test_svd();
  double data[] = { 1, 1, 1,  1, 2, 3,  1, 3, 6};
  vnl_matrix<double> mat(data, 3, 3);
  std::cout << std::endl << "A matrix" << std::endl;
  for(unsigned int r = 0; r < mat.rows(); r++)
    {
    for(unsigned int c = 0; c < mat.rows(); c++)
      {
      std::cout << mat(r, c) << " ";
      }
    std::cout << std::endl;
    }

  return 0;
}



