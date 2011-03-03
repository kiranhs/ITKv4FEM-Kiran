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

#ifndef __itkFEMLoadNode_h
#define __itkFEMLoadNode_h

#include "itkFEMLoadBase.h"
#include "vnl/vnl_vector.h"

namespace itk
{
namespace fem
{
/**
 * \class LoadNode
 * \brief This load is applied on a specific point within the system.
 *
 * The point is defined as a point within an element object.
 *
 * You must provide a pointer to an element object and a number
 * of point on which on which the load acts. Force vector F should have
 * element->GetNumberOfDegreesOfFreedomPerNode() dimensions.
 */
class LoadNode:public Load
{
  //FEM_CLASS(LoadNode, Load)
public:

  typedef LoadNode                Self;
  typedef Load                    Superclass;
  typedef SmartPointer< Self >                               Pointer;
  typedef SmartPointer< const Self >                         ConstPointer;
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(LoadNode, Load);
   
  /***VAM***/
  static int CLID(void);
  virtual int ClassID() const  { return CLID(); }
  
  virtual Baseclass::Pointer Clone() const
    { 
          Pointer o = New();
          o->SetReferenceCount(1);
          o->m_element = this->m_element;
          o->m_pt = this->m_pt;
          o->F = this->F;
  
          o->GN  = this->GN; 
          return o.GetPointer();
    }
    
  /** Same as New() but returns pointer to base class */
  static Baseclass::Pointer NewB()
    {
    return New().GetPointer();
    }
    
    
  typedef Node::Float Float;

  /**
   * Read a LoadNode object from input stream.
   */
  virtual void Read(std::istream & f, void *info);

  /**
   * Write a Load object to the output stream
   */
  virtual void Write(std::ostream & f) const;

  /**
    * Set the force acting at the node
    */
  void SetForce(const vnl_vector< Float > force);

  /**
  * Get the force acting at the node
  */
  vnl_vector< Float > GetForce();

  //void SetElement(Element::ConstPointer pointer);

  /**
  * Get the element containing the degree of freedom
  * on which the force is being applied.
  */
  Element::ConstPointer GetElement();

  /**
   * Get the element containing the degree of freedom
   * on which the force is being applied.
   */
  void SetElement(Element::ConstPointer el);

  /**
   * Set the node number on which the load is being applied.
   */
  void SetNode(int num);

  /**
 * Get the node number on which the load is being applied.
 */
  int GetNode();

  LoadNode():m_element(0) {}    // default constructor
  LoadNode(Element::ConstPointer element_, unsigned int pt_, vnl_vector< Float > F_):
    m_element(element_), m_pt(pt_), F(F_) {}
	
protected:
  /**
 * Pointer to an element in a system that contains the DOF
 * on which the external force is applied.
 */
  Element::ConstPointer m_element;

  /**
   * Point within the element on which the force acts.
   */
  unsigned int m_pt;

  /**
   * Force applied on the node. Dimension of F should equal
   * element->GetNumberOfDegreesOfFreedomPerNode().
   */
  vnl_vector< Float > F;
};

FEM_CLASS_INIT(LoadNode)
}
}  // end namespace itk::fem

#endif // #ifndef __itkFEMLoadDOF_h
