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

#ifndef __itkFEMElement2DC0LinearQuadrilateral_h
#define __itkFEMElement2DC0LinearQuadrilateral_h

#include "itkFEMElementStd.h"

namespace itk
{
namespace fem
{
/**
 * \class Element2DC0LinearQuadrilateral
 * \brief 4-noded, linear, C0 continuous finite element in 2D space.
 *
 * The ordering of the nodes is counter clockwise. That is the nodes
 * should be defined in the following order:
 *
 *  3 (0,1)                  2 (1,1)
 *  *------------------------*
 *  |                        |
 *  |                        |
 *  |                        |
 *  |                        |
 *  |                        |
 *  |                        |
 *  *------------------------*
 *  0 (0,0)                  1 (0,1)
 *
 * This is an abstract class. Specific concrete implemenations of this 
 * It must be combined with the physics component of the problem. 
 * This has already been done in the following classes: 
 *
 * \sa Element2DC0LinearQuadrilateralMembrane 
 * \sa Element2DC0LinearQuadrilateralStrain
 * \sa Element2DC0LinearQuadrilateralStress
 */
 
class Element2DC0LinearQuadrilateral:public ElementStd< 4, 2 >
{
public:
  /** Standard class typedefs. */
  typedef Element2DC0LinearQuadrilateral      Self;
  typedef ElementStd< 4, 2 >                  TemplatedParentClass;
  typedef TemplatedParentClass                Superclass;
  typedef SmartPointer< Self >                Pointer;
  typedef SmartPointer< const Self >          ConstPointer;
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(Element2DC0LinearQuadrilateral, TemplatedParentClass);
  
  
  //////////////////////////////////////////////////////////////////////////
  /**
   * Methods related to numeric integration
   */

  enum { DefaultIntegrationOrder = 2 };

  virtual void GetIntegrationPointAndWeight(unsigned int i, VectorType & pt, Float & w, unsigned int order) const;

  virtual unsigned int GetNumberOfIntegrationPoints(unsigned int order) const;

  //////////////////////////////////////////////////////////////////////////
  /**
   * Methods related to the geometry of an element
   */

  virtual VectorType ShapeFunctions(const VectorType & pt) const;

  virtual void ShapeFunctionDerivatives(const VectorType & pt, MatrixType & shapeD) const;

  virtual bool GetLocalFromGlobalCoordinates(const VectorType & globalPt, VectorType & localPt) const;
  void PopulateEdgeIds();

  void InterpolationFunctions( const VectorType &pcoords, VectorType & sf) const;

  void InterpolationDerivs(const VectorType & pcoords, VectorType & derivs) const;

  Float Determinant2x2(const VectorType &c1, const VectorType &c2) const;

private:

};
}
}  // end namespace itk::fem

#endif  // #ifndef __itkFEMElement2DC0LinearQuadrilateral_h
