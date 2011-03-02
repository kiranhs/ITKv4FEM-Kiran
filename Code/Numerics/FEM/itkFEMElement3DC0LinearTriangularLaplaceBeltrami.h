/*=========================================================================
*
* Copyright Insight Software Consortium
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0.txt
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
*=========================================================================*/

#ifndef __itkFEMElement3DC0LinearTriangularLaplaceBeltrami_h
#define __itkFEMElement3DC0LinearTriangularLaplaceBeltrami_h

#include "itkFEMElement3DC0LinearTriangular.h"
#include "itkFEMElement3DMembrane.h"
#include "itkFEMElement3DMembrane1DOF.h"

namespace itk
{
namespace fem
{
/**
 * \class Element3DC0LinearTriangularLaplaceBeltrami
 * \brief 3-noded finite element class in 3D space for surface LaplaceBeltrami problem.
 *
 * This element is combined from Element3DC0LinearTriangular and Element3DLaplaceBeltrami.
 */
class Element3DC0LinearTriangularLaplaceBeltrami:public Element3DMembrane1DOF< Element3DC0LinearTriangular >
{
  //FEM_CLASS(Element3DC0LinearTriangularLaplaceBeltrami, Element3DMembrane1DOF< Element3DC0LinearTriangular > )
public:

  typedef Element3DC0LinearTriangularLaplaceBeltrami                Self;
  typedef Element3DMembrane1DOF< Element3DC0LinearTriangular >      Superclass;
  typedef SmartPointer< Self >                                      Pointer;
  typedef SmartPointer< const Self >                                ConstPointer;
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(Element3DC0LinearTriangularLaplaceBeltrami, Element3DMembrane1DOF< Element3DC0LinearTriangular >);
   
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
  Element3DC0LinearTriangularLaplaceBeltrami();

  /**
   * Construct an element by specifying pointers to
   * 3 points and a material.
   */
  Element3DC0LinearTriangularLaplaceBeltrami(
    NodeIDType n1_,
    NodeIDType n2_,
    NodeIDType n3_,
    Material::ConstPointer p_);

  virtual unsigned int GetNumberOfDegreesOfFreedomPerNode(void) const
  { return 1; }

  virtual void GetStiffnessMatrix(MatrixType & Ke) const;

  
};  // class Element3DC0LinearTriangularLaplaceBeltrami

FEM_CLASS_INIT(Element3DC0LinearTriangularLaplaceBeltrami)
}
}  // end namespace itk::fem

#endif  // #ifndef __itkFEMElement3DC0LinearTriangularLaplaceBeltrami_h
