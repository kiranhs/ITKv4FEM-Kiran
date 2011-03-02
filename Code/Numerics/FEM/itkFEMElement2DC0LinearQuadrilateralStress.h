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

#ifndef __itkFEMElement2DC0LinearQuadrilateralStress_h
#define __itkFEMElement2DC0LinearQuadrilateralStress_h

#include "itkFEMElement2DC0LinearQuadrilateral.h"
#include "itkFEMElement2DStress.h"

namespace itk
{
namespace fem
{
/**
 * \class Element2DC0LinearQuadrilateralStress
 * \brief 4-noded finite element class in 2D space for linear elasticity problem
 * The constitutive equation used is from the principles of linear elasticity.
 */
class Element2DC0LinearQuadrilateralStress:public Element2DStress< Element2DC0LinearQuadrilateral >
{
  //FEM_CLASS(Element2DC0LinearQuadrilateralStress, Element2DStress< Element2DC0LinearQuadrilateral > )
public:
  
  typedef Element2DC0LinearQuadrilateralStress                Self;
  typedef Element2DStress< Element2DC0LinearQuadrilateral >   Superclass;
  typedef SmartPointer< Self >                                Pointer;
  typedef SmartPointer< const Self >                          ConstPointer;
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(Element2DC0LinearQuadrilateralStress, Element2DStress< Element2DC0LinearQuadrilateral >);
   
  /***VAM***/
  static int CLID(void);
  virtual int ClassID() const  { return CLID(); }
  
  virtual Baseclass::Pointer Clone() const
    { 
          Pointer o = New();
          o->SetReferenceCount(1);
          o->m_node[0] = this->m_node[0]; 
          o->m_node[1] = this->m_node[1]; 
          o->m_node[2] = this->m_node[2]; 
          o->m_node[3] = this->m_node[3]; 
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
  Element2DC0LinearQuadrilateralStress();

  /**
   * Construct an element by specifying pointers to
   * 4 points and a material.
   */
  Element2DC0LinearQuadrilateralStress(
    NodeIDType n1_,
    NodeIDType n2_,
    NodeIDType n3_,
    NodeIDType n4_,
    Material::ConstPointer p_);
  
};  // class Element2DC0LinearQuadrilateralStress

FEM_CLASS_INIT(Element2DC0LinearQuadrilateralStress)
}
}  // end namespace itk::fem

#endif  // #ifndef __itkFEMElement2DC0LinearQuadrilateralStress_h
