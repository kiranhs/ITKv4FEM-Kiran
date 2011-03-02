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

#ifndef __itkFEMElement2DC0LinearLineStress_h
#define __itkFEMElement2DC0LinearLineStress_h

#include "itkFEMElement2DC0LinearLine.h"
#include "itkFEMElement1DStress.h"

namespace itk
{
namespace fem
{
/**
 * \class Element2DC0LinearLineStress
 * \brief 2-noded finite element class in 2D space for linear elasticity problem.
 */
class Element2DC0LinearLineStress:public Element1DStress< Element2DC0LinearLine >
{
  //FEM_CLASS(Element2DC0LinearLineStress, Element1DStress< Element2DC0LinearLine > )
public:
  typedef Element2DC0LinearLineStress                Self;
  typedef Element1DStress< Element2DC0LinearLine >   Superclass;
  typedef SmartPointer< Self >           Pointer;
  typedef SmartPointer< const Self >     ConstPointer;
  typedef Superclass::Baseclass          Baseclass;
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(Element2DC0LinearLineStress, Element1DStress< Element2DC0LinearLine >);
  
  /***VAM***/
  static int CLID(void);
  virtual int ClassID() const  { return CLID(); }
  
  virtual Baseclass::Pointer Clone() const
  { 
    Pointer o = New();
    o->SetReferenceCount(1);
    o->m_node[0] = this->m_node[0]; 
    o->m_node[1] = this->m_node[1];
    o->m_mat  = this->m_mat; 
    o->GN  = this->GN; 
    return o.GetPointer();
  }
  
  /** Same as New() but returns pointer to base class */
  static Baseclass::Pointer NewB()
  {
    return New().GetPointer();
  }
  
  HANDLE_ELEMENT_LOADS();

  /**
   * Default constructor only clears the internal storage
   */
  Element2DC0LinearLineStress();

  /**
   * Construct an element by specifying pointers to
   * 4 points and a material.
   */
  Element2DC0LinearLineStress(
    NodeIDType n1_,
    NodeIDType n2_,
    Material::ConstPointer p_);

  /**
   * Consistent mass matrix for a line element. 
   * See any finite element book for Consistent mass matrix definition.
   */
  virtual void GetMassMatrix(MatrixType & Me) const;
  
  
};  // class Element2DC0LinearLineStress

FEM_CLASS_INIT(Element2DC0LinearLineStress)
}
}  // end namespace itk::fem

#endif  // #ifndef __itkFEMElement2DC0LinearLineStress_h
