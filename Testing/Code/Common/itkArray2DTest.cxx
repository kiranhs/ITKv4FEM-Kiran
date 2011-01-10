/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkArray2DTest.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <iostream>

#include "itkArray2D.h"


int itkArray2DTest(int, char* [] )
{
  typedef itk::Array2D< double > ArrayType;

  typedef vnl_matrix<double> VnlMatrixType;
  
  const unsigned int rows = 3;
  const unsigned int cols = 4;
  
  ArrayType a( rows, cols);
  VnlMatrixType vm( rows, cols ); 
  
  for( unsigned int r=0; r<rows; r++)
    {
    for( unsigned int c=0; c<cols; c++)
      {
      const double value = static_cast<double>( r + c );
      a(r,c)  = value;
      vm(r,c) = value;
      }
    }

  const double tolerance = 1e-6;

  // test copy constructor
  ArrayType b( a );

  for( unsigned int r=0; r<rows; r++)
    {
    for( unsigned int c=0; c<cols; c++)
      {
      double diff = a(r,c) - b(r,c);
      diff = (diff > 0.0 ) ? diff : -diff; // take abs value
      if( diff > tolerance )
        {
        std::cerr << "Error in copy constructor " << std::endl;
        return EXIT_FAILURE;
        }
      }
    }

  // test construction from vnl_matrix
  ArrayType d( vm );

  for( unsigned int r=0; r<rows; r++)
    {
    for( unsigned int c=0; c<cols; c++)
      {
      double diff = d(r,c) - vm(r,c);
      diff = (diff > 0.0 ) ? diff : -diff; // take abs value
      if(  diff  > tolerance )
        {
        std::cerr << "Error in construction from vn_matrix" << std::endl;
        return EXIT_FAILURE;
        }
      }
    }

  // test for assignment from Array2D
  ArrayType e;
  
  e = a;

  for( unsigned int r=0; r<rows; r++)
    {
    for( unsigned int c=0; c<cols; c++)
      {
      double diff = a(r,c) - e(r,c);
      diff = (diff > 0.0 ) ? diff : -diff; // take abs value
      if( diff  > tolerance )
        {
        std::cerr << "Error in assignment from Array2D constructor " << std::endl;
        return EXIT_FAILURE;
        }
      }
    }

  // test for assignment from vnl_matrix
  ArrayType f;
  
  f = vm;

  for( unsigned int r=0; r<rows; r++)
    {
    for( unsigned int c=0; c<cols; c++)
      {
      double diff = f(r,c) - vm(r,c);
      diff = (diff > 0.0 ) ? diff : -diff; // take abs value
      if(  diff > tolerance )
        {
        std::cerr << "Error in assignment from  vn_matrix" << std::endl;
        return EXIT_FAILURE;
        }
      }
    }



  std::cout << "Test Passed ! " << std::endl;      
  return EXIT_SUCCESS;
}
