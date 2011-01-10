/*=========================================================================

   Program:   Insight Segmentation & Registration Toolkit
   Module:    itkTriangleHelperTest.cxx
   Language:  C++
   Date:      $Date$
   Version:   $Revision$

   Copyright (c) Insight Software Consortium. All rights reserved.
   See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

   This software is distributed WITHOUT ANY WARRANTY; without even
   the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
   PURPOSE.  See the above copyright notices for more information.

 =========================================================================*/

#include <itkPoint.h>
#include <itkTriangleHelper.h>
#include <iostream>

int itkTriangleHelperTest( int argc, char* argv[] )
{
  (void) argc;
  (void) argv;

  const unsigned int Dimension = 3;
  typedef double                                CoordRepType;
  typedef itk::Point< CoordRepType, Dimension > PointType;
  typedef PointType::VectorType                 VectorType;
  typedef itk::TriangleHelper< PointType >      TriangleHelperType;

  PointType Org;
  Org.Fill( 0. );

  PointType a;
  a[0] = 1.;
  a[1] = 0.;
  a[2] = 0.;

  PointType b;
  b[0] = 0.5;
  b[1] = 0.5;
  b[2] = 0.;

  PointType c;
  c[0] = -0.5;
  c[1] = 0.5;
  c[2] = 0.;

  if( TriangleHelperType::IsObtuse( a, Org, b ) )
    {
    std::cout <<"This triangle was supposed to be non-obtuse" <<std::endl;
    std::cout <<a <<" * " <<Org <<" * " <<b <<std::endl;
    return EXIT_FAILURE;
    }
  if( !TriangleHelperType::IsObtuse( a, Org, c ) )
    {
    std::cout <<"This triangle was supposed to be obtuse" <<std::endl;
    std::cout <<a <<" * " <<Org <<" * " <<c <<std::endl;
    return EXIT_FAILURE;
    }
  if( TriangleHelperType::IsObtuse( a, Org, a ) )
    {
    std::cout <<"Dummy test" <<std::endl;
    std::cout <<a <<" * " <<Org <<" * " <<a <<std::endl;
    return EXIT_FAILURE;
    }

  VectorType n;
  n[0] = 0.;
  n[1] = 0.;
  n[2] = -1.;

  if( ( n - TriangleHelperType::ComputeNormal( a, Org, b ) ).GetNorm() > 1e-6 )
    {
    std::cout <<"Difference between normals is too large" <<std::endl;
    return EXIT_FAILURE;
    }
  if( ( TriangleHelperType::ComputeNormal( a, Org, a ) ).GetNorm() > 1e-6 )
    {
    std::cout <<"Difference between normals is too large" <<std::endl;
    return EXIT_FAILURE;
    }

  if( vnl_math_abs( TriangleHelperType::Cotangent( a, Org, b ) - 1. ) > 1e-6 )
    {
    return EXIT_FAILURE;
    }
  std::cout <<TriangleHelperType::Cotangent( a, Org, a ) <<std::endl;

  std::cout <<TriangleHelperType::ComputeBarycenter( 1., a, 2., Org, 3., b ) 
    <<std::endl;

  PointType barycenter = 
    TriangleHelperType::ComputeBarycenter( 0., a, 1., Org, -1., b );

  if( ( barycenter.GetVectorFromOrigin() ).GetNorm() > 1e-6 )
    {
    std::cout 
      <<"TriangleHelperType::ComputeBarycenter( 0.,a,1.,Org,-1.,b) FAILED"
      <<std::endl;
    return EXIT_FAILURE;
    }

  if( vnl_math_abs( TriangleHelperType::ComputeAngle( a, Org, b ) 
        - 0.25 * vnl_math::pi ) > 1e-6 )
    {
    std::cout <<"TriangleHelperType::ComputeAngle( a, Org, b ) FAILED"
      <<std::endl;
    return EXIT_FAILURE;
    }

  std::cout <<TriangleHelperType::ComputeGravityCenter( a, Org, b ) 
    <<std::endl;

  PointType Circum;
  Circum[0] = 0.5;
  Circum[1] = 0.;
  Circum[2] = 0.;
 
  barycenter = TriangleHelperType::ComputeCircumCenter( a, Org, b );
 
  if( barycenter.SquaredEuclideanDistanceTo( Circum ) > 1e-6 )
    {
    std::cout <<"TriangleHelperType::ComputeCircumCenter( a, Org, b ) FAILED"
      <<std::endl;
    return EXIT_FAILURE;
    }

  barycenter = TriangleHelperType::ComputeConstrainedCircumCenter( a, Org, b );

  if( barycenter.SquaredEuclideanDistanceTo( Circum ) > 1e-6 )
    {
    std::cout <<"TriangleHelperType::ComputeConstrainedCircumCenter(a,Org,b)"
      <<" FAILED" <<std::endl;
    return EXIT_FAILURE;
    }

  CoordRepType area = TriangleHelperType::ComputeArea( a, Org, b );
  if( vnl_math_abs( area - 0.25 ) > 1e-6 )
    {
    std::cout <<"TriangleHelperType::ComputeArea( a, Org, b ) FAILED"
      <<std::endl;
    return EXIT_FAILURE;
    }
 
  return EXIT_SUCCESS;
}
