/*=========================================================================
*
* Copyright Insight Software Consortium
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0.txt
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
*=========================================================================*/

#ifndef __itkFEMElement3DC0LinearTriangular_h
#define __itkFEMElement3DC0LinearTriangular_h

#include "itkFEMElementStd.h"

namespace itk
{
namespace fem
{
/**
 * \class Element3DC0LinearTriangular
 * \brief 3-noded, linear, C0 continuous finite element in 2D space.
 */
class Element3DC0LinearTriangular:public ElementStd< 3, 3 >
{
public:
  /** Standard class typedefs. */
  typedef Element3DC0LinearTriangular         Self;
  typedef ElementStd< 3, 3 >                  TemplatedParentClass;
  typedef TemplatedParentClass                Superclass;
  typedef SmartPointer< Self >                Pointer;
  typedef SmartPointer< const Self >          ConstPointer;
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(Element3DC0LinearTriangular, TemplatedParentClass);
  
//////////////////////////////////////////////////////////////////////////
/*
 * Methods related to numeric integration
 */

  enum { DefaultIntegrationOrder = 1 };

  virtual void GetIntegrationPointAndWeight(unsigned int i, VectorType & pt, Float & w, unsigned int order) const;

  virtual unsigned int GetNumberOfIntegrationPoints(unsigned int order) const;

//////////////////////////////////////////////////////////////////////////
/*
 * Methods related to the geometry of an element
 */

  virtual VectorType ShapeFunctions(const VectorType & pt) const;

  virtual void ShapeFunctionDerivatives(const VectorType & pt, MatrixType & shapeD) const;

  virtual bool GetLocalFromGlobalCoordinates(const VectorType & globalPt, VectorType & localPt) const;

  virtual Float JacobianDeterminant(const VectorType & pt, const MatrixType *pJ = 0) const;

  virtual void JacobianInverse(const VectorType & pt, MatrixType & invJ, const MatrixType *pJ = 0) const;

 /**
 * Normal of the triangle element
 */
  void ComputeNormalDirection(const VectorType & v1, const VectorType & v2,
	  const VectorType & v3, VectorType & n)const;

 /**
 * Project the point x onto the plane containing the triangle element
 */
  void GeneralizedProjectPoint(const VectorType & x, const VectorType & origin,
	  const VectorType & normal, VectorType & xproj) const;

  itk::fem::Element::Float Determinant2x2(const VectorType & c1, const VectorType & c2) const;
  /**
   * Constants for integration rules.
   */
  static const Float trigGaussRuleInfo[6][7][4];

  /**
   * Array that holds number of integration point for each order
   * of numerical integration.
   */
  static const unsigned int Nip[6];

  virtual void PopulateEdgeIds();

};
}
}  // end namespace itk::fem

#endif  // #ifndef __itkFEMElement3DC0LinearTriangular_h
