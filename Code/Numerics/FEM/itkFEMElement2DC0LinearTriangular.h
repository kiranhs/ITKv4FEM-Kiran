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

#ifndef __itkFEMElement2DC0LinearTriangular_h
#define __itkFEMElement2DC0LinearTriangular_h

#include "itkFEMElementStd.h"

namespace itk
{
namespace fem
{
/**
 * \class Element2DC0LinearTriangular
 * \brief 3-noded, linear, C0 continuous finite element in 2D space.
 *
 * The ordering of the nodes is counter clockwise. That is the nodes
 * should be defined in the following order:
 *
 *  (0,1) 
 *  2 
 *  *
 *  |\ 
 *  | \ 
 *  |  \     
 *  |   \ 
 *  |    \ 
 *  |     \
 *  *------*
 *  0      1 
 *  (0,0)  (0,1)
 *
 * This is an abstract class. Specific concrete implemenations of this 
 * It must be combined with the physics component of the problem. 
 * This has already been done in the following classes: 
 *
 * \sa Element2DC0LinearTriangular
 * \sa Element2DC0LinearTriangularStrain
 * \sa Element2DC0LinearTriangularStress
 */
class Element2DC0LinearTriangular:public ElementStd< 3, 2 >
{
public:
  /** Standard class typedefs. */
  typedef Element2DC0LinearTriangular         Self;
  typedef ElementStd< 3, 2 >                  TemplatedParentClass;
  typedef TemplatedParentClass                Superclass;
  typedef SmartPointer< Self >                Pointer;
  typedef SmartPointer< const Self >          ConstPointer;
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(Element2DC0LinearTriangular, TemplatedParentClass);
  
  
  //////////////////////////////////////////////////////////////////////////
  /**
   * Methods related to numeric integration
   */

  enum { DefaultIntegrationOrder = 1 };

  virtual void GetIntegrationPointAndWeight(unsigned int i, VectorType & pt, Float & w, unsigned int order) const;

  virtual unsigned int GetNumberOfIntegrationPoints(unsigned int order) const;

  //////////////////////////////////////////////////////////////////////////
  /**
   * Methods related to the geometry of an element
   */

  virtual VectorType ShapeFunctions(const VectorType & pt) const;

  virtual void ShapeFunctionDerivatives(const VectorType & pt, MatrixType & shapeD) const;

  // FIXME: Write a proper implementation
  virtual bool GetLocalFromGlobalCoordinates(const VectorType & globalPt, VectorType & localPt) const;

  // Since the Jacobian is not quadratic, we need to provide our
  // own implementation of calculating the determinant and inverse.
  virtual Float JacobianDeterminant(const VectorType & pt, const MatrixType *pJ = 0) const;

  virtual void JacobianInverse(const VectorType & pt, MatrixType & invJ, const MatrixType *pJ = 0) const;

  virtual void PopulateEdgeIds();


  /**
   * Constants for integration rules.
   */
  static const Float trigGaussRuleInfo[6][7][4];

  /**
   * Array that holds number of integration point for each order
   * of numerical integration.
   */
  static const unsigned int Nip[6];
};
}
}  // end namespace itk::fem

#endif  // #ifndef __itkFEMElement2DC0LinearTriangular_h
