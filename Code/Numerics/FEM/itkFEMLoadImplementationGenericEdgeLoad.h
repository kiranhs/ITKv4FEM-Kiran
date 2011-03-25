/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFEMLoadImplementationGenericEdgeLoad.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkFEMLoadImplementationGenericEdgeLoad_h
#define __itkFEMLoadImplementationGenericEdgeLoad_h

#include "itkFEMElementBase.h"
#include "itkFEMLoadEdge.h"

namespace itk
{
namespace fem
{
/**
 * \class LoadImplementationGenericEdgeLoad
 * \brief Class that holds a templated generic Edge load implementation.
 *
 * For more info see class LoadImplementationGenericBodyLoad.
 */
class LoadImplementationGenericEdgeLoad
{
public:
  template< class TElementClassConstPointer >
  static void HandleLoad(TElementClassConstPointer e, Element::LoadPointer l, Element::VectorType & Fe)
  {
    // Check if we really got an object of correct class
    LoadEdge::Pointer l0 = dynamic_cast< LoadEdge * >( &*l );

    if ( !l0 )
      {
      // Class of passed load object was not compatible!
      throw FEMException(__FILE__, __LINE__, "FEM error");
      }

    // Statically cast the passed pointer to the element base class and
    // call the real load implementation with the correct pointer types.
    // If cast fails, the passed pointer was of incompatible class.
    Implementation(static_cast< Element::ConstPointer >( e ), l0, Fe);
  }

private:
  /**
   * Handle LoadEdge in element by integrating over the element domain.
   * This implementation requires that the element has the shape functions
   * and integration routines defined.
   *
   * It is also assumed, that element's local DOFs are numbered with respect
   * to node ID. If this is not the case, you should not use this function.
   */
  static void Implementation(Element::ConstPointer element, LoadEdge::Pointer load, Element::VectorType & Fe);

  /**
   * Private constructor prohibits creation of objects of this class
   */
  LoadImplementationGenericEdgeLoad();
};

}
}  // end namespace itk::fem

#endif // #ifndef __itkFEMLoadImplementationGenericEdgeLoad_h
