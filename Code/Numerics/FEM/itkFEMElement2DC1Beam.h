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

#ifndef __itkFEMElement2DC1Beam_h
#define __itkFEMElement2DC1Beam_h

#include "itkFEMElementStd.h"
#include "itkFEMLoadElementBase.h"
#include "itkFEMMaterialLinearElasticity.h"

namespace itk
{
namespace fem
{
/**
 * \class Element2DC1Beam
 * \brief 1D Beam (spring that also bends) finite element in 2D space.
 *
 * The Displacements at each node are modeled with two translational, 
 * and one rotational, degree of freedom. 
 */
class Element2DC1Beam:public ElementStd< 2, 2 >
{
public:
  /** Standard class typedefs. */
  typedef Element2DC1Beam                             Self;
  typedef ElementStd< 2, 2 >                           TemplatedParentClass;
  typedef TemplatedParentClass                         Superclass;
  typedef SmartPointer< Self >                         Pointer;
  typedef SmartPointer< const Self >                   ConstPointer;
  
  /** Method for creation through the object factory. */
	itkNewMacro(Self);
	
  /** Run-time type information (and related methods). */
  itkTypeMacro(Element2DC1Beam, TemplatedParentClass);
  
  /**
   * Clone the current object. To be replaced by CreateAnother()
   */
  virtual Baseclass::Pointer Clone() const
  { 
    Pointer o = new Self(*this);
    return o.GetPointer(); 
  }
  
  // FIXME: Write this class in the same way as the others -
  //        properly define all virtual functions.

  /**
   * Default constructor only clears the internal storage
   */
  Element2DC1Beam();

  /**
   * Construct an element by specifying two nodes and material
   */
  Element2DC1Beam(Node::ConstPointer n1_,
                  Node::ConstPointer n2_,
                  Material::ConstPointer mat_);

//////////////////////////////////////////////////////////////////////////
/*
 * Methods related to the physics of the problem.
 */
  virtual void GetStiffnessMatrix(MatrixType & Ke) const;

  virtual void GetMassMatrix(MatrixType & Me) const;


  virtual void GetStrainDisplacementMatrix(MatrixType &, const MatrixType &) const {}
  virtual void GetMaterialMatrix(MatrixType &) const {}

  //////////////////////////////////////////////////////////////////////////
  /**
   * Methods related to numeric integration
   */

  enum { DefaultIntegrationOrder = 1 };
  virtual void GetIntegrationPointAndWeight(unsigned int i, VectorType & pt, Float & w, unsigned int order = 0) const;

  virtual unsigned int GetNumberOfIntegrationPoints(unsigned int order) const;

  //////////////////////////////////////////////////////////////////////////
  /**
   * Methods related to the geometry of an element
   */

  virtual VectorType ShapeFunctions(const VectorType & pt) const;

  virtual void ShapeFunctionDerivatives(const VectorType & pt, MatrixType & shapeD) const;

  virtual bool GetLocalFromGlobalCoordinates(const VectorType &, VectorType &) const
  {
    return false;
  }

  virtual Float JacobianDeterminant(const VectorType & pt, const MatrixType *pJ) const;

  virtual unsigned int GetNumberOfDegreesOfFreedomPerNode(void) const
  { return 3; }

  /**
   * Get/Set the material properties for the element
   */
  virtual Material::ConstPointer GetMaterial(void) const { return dynamic_cast<const Material *> (&*m_mat); }

  virtual void SetMaterial(Material::ConstPointer mat_) { m_mat =
                                                            dynamic_cast< const MaterialLinearElasticity * >( &*mat_ ); }
                                                            
private:

  /**
   * Pointer to material properties of the element
   */
  MaterialLinearElasticity::ConstPointer m_mat;

  
};

}
}  // end namespace itk::fem

#endif // #ifndef __itkFEMElement2DC1Beam_h
