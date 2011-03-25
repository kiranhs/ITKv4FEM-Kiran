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

#ifndef __itkFEMElement2DC0QuadraticTriangularStress_h
#define __itkFEMElement2DC0QuadraticTriangularStress_h

#include "itkFEMElement2DC0QuadraticTriangular.h"
#include "itkFEMElement2DStress.h"

namespace itk
{
namespace fem
{
/**
 * \class Element2DC0QuadraticTriangularStress
 * \brief 6-noded finite element class in 2D space for linear elasticity problem.
 * The constitutive equation used is for plane stress.
 *
 * This class combines the geometry of the FE problem defined in
 * \link Element2DC0QuadraticTriangular
 * and the physics of the problem defined in
 * \link Element2DStress
 * 
 * \sa Element2DC0LinearTriangularStrain
 */
class Element2DC0QuadraticTriangularStress:public Element2DStress< Element2DC0QuadraticTriangular >
{
public:
  /** Standard class typedefs. */
  typedef Element2DC0QuadraticTriangularStress                  Self;
  typedef Element2DStress< Element2DC0QuadraticTriangular >     Superclass;
  typedef SmartPointer< Self >                                  Pointer;
  typedef SmartPointer< const Self >                            ConstPointer;
  
  /** Method for creation through the object factory. */
	itkNewMacro(Self);
	
  /** Run-time type information (and related methods). */
  itkTypeMacro(Element2DC0QuadraticTriangularStress, Element2DStress< Element2DC0QuadraticTriangular >);
  
  /**
   * Clone the current object. To be replaced by CreateAnother()
   */
  virtual Baseclass::Pointer Clone() const
  { 
    Pointer o = new Self(*this);
    return o.GetPointer(); 
  }

  /**
   * Default constructor only clears the internal storage
   */
  Element2DC0QuadraticTriangularStress();

  /**
   * Construct an element by specifying pointers to
   * 3 points and a material.
   */
  Element2DC0QuadraticTriangularStress(
    NodeIDType n1_,
    NodeIDType n2_,
    NodeIDType n3_,
    NodeIDType n4_,
    NodeIDType n5_,
    NodeIDType n6_,
    Material::ConstPointer p_);



};  // class Element2DC0QuadraticTriangularStress

}
}  // end namespace itk::fem

#endif  // #ifndef __itkFEMElement2DC0QuadraticTriangularStress_h
