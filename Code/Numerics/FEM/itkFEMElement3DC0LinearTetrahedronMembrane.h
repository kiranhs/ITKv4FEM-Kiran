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

#ifndef __itkFEMElement3DC0LinearTetrahedronMembrane_h
#define __itkFEMElement3DC0LinearTetrahedronMembrane_h

#include "itkFEMElement3DC0LinearTetrahedron.h"
#include "itkFEMElement3DMembrane.h"

namespace itk
{
namespace fem
{
/**
 * \class Element3DC0LinearTetrahedronMembrane
 * \brief 4-noded finite element class in 3D space for linear elasticity problem
 * The constitutive equation used is from membrane bending energy.
 */
class Element3DC0LinearTetrahedronMembrane:public Element3DMembrane< Element3DC0LinearTetrahedron >
{
  //FEM_CLASS(Element3DC0LinearTetrahedronMembrane, Element3DMembrane< Element3DC0LinearTetrahedron > )
public:

  typedef Element3DC0LinearTetrahedronMembrane                Self;
  typedef Element3DMembrane< Element3DC0LinearTetrahedron >   Superclass;
  typedef SmartPointer< Self >                                Pointer;
  typedef SmartPointer< const Self >                          ConstPointer;
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(Element3DC0LinearTetrahedronMembrane, Element3DMembrane< Element3DC0LinearTetrahedron >);
   
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
  Element3DC0LinearTetrahedronMembrane();

  /**
   * Construct an element by specifying pointers to
   * an array of 4 points and a material.
   */
  Element3DC0LinearTetrahedronMembrane(
    NodeIDType ns_[],
    Material::ConstPointer p_);
  
};  // class Element3DC0LinearTetrahedronMembrane

FEM_CLASS_INIT(Element3DC0LinearTetrahedronMembrane)
}
}  // end namespace itk::fem

#endif  // #ifndef __itkFEMElement3DC0LinearTetrahedronMembrane_h
