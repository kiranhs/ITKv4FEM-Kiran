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

#ifndef __itkFEMElement3DMembrane_h
#define __itkFEMElement3DMembrane_h

#include "itkFEMElementBase.h"
#include "itkFEMMaterialLinearElasticity.h"

namespace itk
{
namespace fem
{
/**
 * \class Element3DMembrane
 * \brief Class that is used to define a membrane energy problem in 3D space.
 *
 * This class only defines the physics of the problem. Use his class together
 * with element classes that specify the geometry to fully define the element.
 *
 * A membrane element in three dimensional is an isotropic homogeneous
 * element through a small thickness. The elements have three translational
 * degrees of freedom at each node. 
 *
 *
 * You can specify one template parameter:
 *
 *   TBaseClass - Class from which Element3DMembrane is derived. TBaseClass must
 *                be derived from the Element base class. This enables you
 *                to use this class at any level of element definition.
 *                If not specified, it defaults to the Element base class.
 */
template< class TBaseClass = Element >
class Element3DMembrane:public TBaseClass
{
public:
  /** Standard class typedefs. */
  typedef Element3DMembrane                   Self;
  typedef TBaseClass                          Superclass;
  typedef SmartPointer< Self >                Pointer;
  typedef SmartPointer< const Self >          ConstPointer;
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(Element3DMembrane, TBaseClass);
  
  // Repeat the required typedefs and enums from parent class
  typedef typename Superclass::Float      Float;
  typedef typename Superclass::MatrixType MatrixType;
  typedef typename Superclass::VectorType VectorType;

  /**
   * Default constructor only clears the internal storage
   */
  Element3DMembrane();

  //////////////////////////////////////////////////////////////////////////
  /**
   * Methods related to the physics of the problem.
   */

  /**
   * Compute the B matrix.
   */
  virtual void GetStrainDisplacementMatrix(MatrixType & B, const MatrixType & shapeDgl) const;

  /**
   * Compute the D matrix.
   */
  virtual void GetMaterialMatrix(MatrixType & D) const;

  /**
   * Compute the mass matrix specific for 3D membrane problems.
   */
  void GetMassMatrix(MatrixType & Me) const;

  /**
   * 3D membrane elements have 3 DOFs per node.
   */
  virtual unsigned int GetNumberOfDegreesOfFreedomPerNode(void) const
  { return 3; }
  
  /**
   * Get/Set the material properties for the element
   */
  virtual Material::ConstPointer GetMaterial(void) const { return dynamic_cast<const Material *> (&*m_mat); }

  virtual void SetMaterial(Material::ConstPointer mat_) { m_mat =
                                                            dynamic_cast< const MaterialLinearElasticity * >( &*mat_ ); }
 
protected:

  virtual void PrintSelf(std::ostream& os, Indent indent) const;  
  
  /**
   * Pointer to material properties of the element
   */
  MaterialLinearElasticity::ConstPointer m_mat;

  
};  // class Element3DMembrane

}
}  // end namespace itk::fem

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkFEMElement3DMembrane.txx"
#endif

#endif  // #ifndef __itkFEMElement3DMembrane_h
