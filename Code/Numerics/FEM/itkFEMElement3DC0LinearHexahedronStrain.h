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

#ifndef __itkFEMElement3DC0LinearHexahedronStrain_h
#define __itkFEMElement3DC0LinearHexahedronStrain_h

#include "itkFEMElement3DC0LinearHexahedron.h"
#include "itkFEMElement3DStrain.h"

namespace itk
{
namespace fem
{
/**
 * \class Element3DC0LinearHexahedronStrain
 * \brief 8-noded finite element class in 3D space for linear elasticity problem
 * The constitutive equation used is from linear elasticity theory.
 */
class Element3DC0LinearHexahedronStrain:public Element3DStrain< Element3DC0LinearHexahedron >
{
  //FEM_CLASS(Element3DC0LinearHexahedronStrain, Element3DStrain< Element3DC0LinearHexahedron > )
public:
  
  typedef Element3DC0LinearHexahedronStrain                Self;
  typedef Element3DStrain< Element3DC0LinearHexahedron >   Superclass;
  typedef SmartPointer< Self >                             Pointer;
  typedef SmartPointer< const Self >                       ConstPointer;
  typedef Superclass::Baseclass                            Baseclass;
  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(Element3DC0LinearHexahedronStrain, Element3DStrain< Element3DC0LinearHexahedron >);
   
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
          o->m_node[4] = this->m_node[4]; 
          o->m_node[5] = this->m_node[5]; 
          o->m_node[6] = this->m_node[6]; 
          o->m_node[7] = this->m_node[7]; 
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
  Element3DC0LinearHexahedronStrain();

  /**
   * Construct an element by specifying pointers to
   * an array of 8 points and a material.
   */
  Element3DC0LinearHexahedronStrain(
    NodeIDType ns_[],
    Material::ConstPointer p_);
  
};  // class Element3DC0LinearHexahedronStrain

FEM_CLASS_INIT(Element3DC0LinearHexahedronStrain)
}
}  // end namespace itk::fem

#endif  // #ifndef __itkFEMElement3DC0LinearHexahedronStrain_h
