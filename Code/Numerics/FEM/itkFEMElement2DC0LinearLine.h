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

#ifndef __itkFEMElement2DC0LinearLine_h
#define __itkFEMElement2DC0LinearLine_h

#include "itkFEMElementStd.h"

namespace itk
{
namespace fem
{
/**
 * \class Element2DC0LinearLine
 * \brief 2-noded, linear, C0 continuous line element in 2D space.
 * \Takes loads only along the length of the axis 
 */
class Element2DC0LinearLine:public ElementStd< 2, 2 >
{
  typedef ElementStd< 2, 2 > TemplatedParentClass;
  FEM_ABSTRACT_CLASS(Element2DC0LinearLine, TemplatedParentClass)
public:

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

  /**
   * Get parametric/local coordinates given global coordinates. The function returns true if the 
   * global coordinate is within the element else returns false. 
   * For a line, line length*1e-4 is used as the tolerance 
   */
  virtual bool GetLocalFromGlobalCoordinates(const VectorType & globalPt, VectorType & localPt) const;

  /**
   * We need to provide our own implementation of calculating Jacobian,
   * because the element lives in 2D space and has only one dimension.
   * The default implementation of Jacobian in the Element base class
   * is not correct since it assumes that the number of element dimensions
   * is equal to the number of spatial dimensions.
   *
   * Jacobian is a scalar for this element.
   */
  virtual void Jacobian(const VectorType & pt, MatrixType & J, const MatrixType *pshapeD = 0) const;

  /**
   * Distance of a point to a line.(Used in GetLocalFromGlobalCoordinates ).
   */
  Float DistanceToLine(const VectorType &x, const VectorType &p1, 
	  const VectorType &p2, Float &t, VectorType &closestPoint) const;
};
}
}  // end namespace itk::fem

#endif  // #ifndef __itkFEMElement2DC0LinearLine_h
