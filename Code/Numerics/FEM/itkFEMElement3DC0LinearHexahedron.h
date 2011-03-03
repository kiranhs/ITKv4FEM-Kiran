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

#ifndef __itkFEMElement3DC0LinearHexahedron_h
#define __itkFEMElement3DC0LinearHexahedron_h

#include "itkFEMElementStd.h"

namespace itk
{
namespace fem
{
/**
 * \class Element3DC0LinearHexahedron
 * \brief 8-noded, linear, C0 continuous finite element in 3D space.
 */
class Element3DC0LinearHexahedron:public ElementStd< 8, 3 >
{
  typedef ElementStd< 8, 3 > TemplatedParentClass;
  //FEM_ABSTRACT_CLASS(Element3DC0LinearHexahedron, TemplatedParentClass)
public:

  typedef Element3DC0LinearHexahedron      Self;
  typedef TemplatedParentClass             Superclass;
  typedef SmartPointer< Self >             Pointer;
  typedef SmartPointer< const Self >       ConstPointer;
  
  /** Method for creation through the object factory. */
  /***Cannot have NewMacro because of pure virtual methods ***/
  //itkNewMacro(Self);
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(Element3DC0LinearHexahedron, TemplatedParentClass);
   
  /***VAM***/
  virtual Baseclass::Pointer Clone() const { return NULL; } 
  //static int CLID(void);
  //virtual int ClassID() const  { return CLID(); }
  
  //////////////////////////////////////////////////////////////////////////
  /**
   * Methods related to numeric integration
   */

  virtual void GetIntegrationPointAndWeight(unsigned int i, VectorType & pt, Float & w, unsigned int order) const;

  virtual unsigned int GetNumberOfIntegrationPoints(unsigned int order) const;

  //////////////////////////////////////////////////////////////////////////
  /**
   * Methods related to the geometry of an element
   */

  virtual VectorType ShapeFunctions(const VectorType & pt) const;

  virtual void ShapeFunctionDerivatives(const VectorType & pt, MatrixType & shapeD) const;

  virtual bool GetLocalFromGlobalCoordinates(const VectorType & globalPt, VectorType & localPt) const;

  /**
   * Methods used in computing parametric/local coordinates given global coordinates.
   */
  void InterpolationFunctions( const VectorType &pcoords, VectorType & sf) const;

  void InterpolationDerivs(const VectorType & pcoords, VectorType & derivs) const;

  Float Determinant3x3(const VectorType &c1, const VectorType &c2, const VectorType &c3) const;
};
}
}  // end namespace itk::fem

#endif  // #ifndef __itkFEMElement3DC0LinearHexahedron_h
