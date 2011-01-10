/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFEMLoadNode.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

// disable debug warnings in MS compiler
#ifdef _MSC_VER
#pragma warning(disable: 4786)
#endif

#include "itkFEMLoadNode.h"

namespace itk
{
namespace fem
{
/**
 * Read the LoadNode object from input stream
 */
void LoadNode::Read(std::istream & f, void *info)
{
  int n;
  /**
   * Convert the info pointer to a usable objects
   */
  ReadInfoType::ElementArrayPointer elements = static_cast< ReadInfoType * >( info )->m_el;

  /** first call the parent's read function */
  Superclass::Read(f, info);

  /** read and set pointer to node that we're applying the load to */
  this->SkipWhiteSpace(f); f >> n; if ( !f ) { goto out; }
  try
    {
    this->m_element = dynamic_cast< const Element * >( &*elements->Find(n) );
    }
  catch ( FEMExceptionObjectNotFound e )
    {
    throw FEMExceptionObjectNotFound(__FILE__, __LINE__, "LoadNode::Read()", e.m_baseClassName, e.m_GN);
    }

  /* read and set the point number */
  this->SkipWhiteSpace(f); f >> this->m_pt; if ( !f ) { goto out; }

  /** read and set the number of elements inside a force vector */
  this->SkipWhiteSpace(f); f >> n; if ( !f ) { goto out; }
  this->F.set_size(n);

  /** read the force vector itself */
  this->SkipWhiteSpace(f); f >> this->F; if ( !f ) { goto out; }

out:

  if ( !f )
    {
    throw FEMExceptionIO(__FILE__, __LINE__, "LoadNode::Read()", "Error reading FEM load!");
    }
}

/**
 * Write the LoadNode to the output stream
 */
void LoadNode::Write(std::ostream & f) const
{
  /** first call the parent's write function */
  Superclass::Write(f);

  /** write the actual Load data */
  // changes made - kiran
//  f<<"\t"<<this->m_element->GN<<"\t% GN of element on which the load
// acts"<<"\n";
  f << "\t" << this->m_element->GetGlobalNumber() << "\t% GN of element on which the load acts" << "\n";
  //changes made - kiran

  f << "\t" << this->m_pt << " " << "\t% Point number within the element\n";
  f << "\t" << this->F.size() << " " << this->F << "\t% Force vector (first number is the size of a vector)\n";

  /** check for errors */
  if ( !f )
    {
    throw FEMExceptionIO(__FILE__, __LINE__, "LoadNode::Write()", "Error writing FEM load!");
    }
}

void LoadNode::SetNode(int num)
{
  this->m_pt = num;
}

int LoadNode::GetNode()
{
  return this->m_pt;
}

void LoadNode::SetForce(const vnl_vector< Float > force)
{
  this->F = force;
}

vnl_vector< itk::fem::Element::Float > LoadNode::GetForce()
{
  return this->F;
}

Element::ConstPointer LoadNode::GetElement()
{
  return this->m_element;
}

void LoadNode::SetElement(Element::ConstPointer el)
{
  this->m_element = el;
}

FEM_CLASS_REGISTER(LoadNode)
}
}  // end namespace itk::fem
