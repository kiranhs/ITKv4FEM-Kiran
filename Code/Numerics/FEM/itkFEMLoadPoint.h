/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFEMLoadPoint.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkFEMLoadPoint_h
#define __itkFEMLoadPoint_h

#include "itkFEMLoadElementBase.h"
#include "vnl/vnl_vector.h"

namespace itk
{
namespace fem
{
/**
 * \class LoadPoint
 * \brief This load is applied on a point in an element.
 *
 * FIXME: To be implemented. Nothing works yet
 */
class LoadPoint:public LoadElement
{
  FEM_CLASS(LoadPoint, LoadElement)
public:
  /**
   * Default constructor
   */
  LoadPoint():
    point(2), Fp(2) {}    /**  we initialize 2D point and force vector */

/**
   * Set the point where the load acts
   */
  void SetPoint(const vnl_vector< Float > p);

  /**
   * Get the point where the load acts
   */
  vnl_vector< Float > GetPoint();

/**
   * Set the force vector
   */
  void SetForce(const vnl_vector< Float > f);

  /**
   * Get the force vector
   */
  vnl_vector< Float > GetForce();

protected:
  /**
  * Point of which the load acts in global coord. sys.
  */
  vnl_vector< Float > point;

  /**
   * the actual load vector
   */
  vnl_vector< Float > Fp;
};

FEM_CLASS_INIT(LoadPoint)
}
}  // end namespace itk::fem

#endif // #ifndef __itkFEMLoadPoint_h
