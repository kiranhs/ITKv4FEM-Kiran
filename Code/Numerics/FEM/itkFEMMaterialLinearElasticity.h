/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFEMMaterialLinearElasticity.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkFEMMaterialLinearElasticity_h
#define __itkFEMMaterialLinearElasticity_h

#include "itkFEMMaterialBase.h"

namespace itk
{
namespace fem
{
/**
 * \class MaterialLinearElasticity
 * \brief Linear elasticity material class
 *
 * This class includes material and other kind of properties required to
 * define material properties of finite elements applied to linear
 * elasticity problems in FEM toolkit.
 */
class MaterialLinearElasticity:public Material
{
  FEM_CLASS(MaterialLinearElasticity, Material)
public:
  virtual void Read(std::istream & f, void *info);

  virtual void Write(std::ostream & f) const;

  /**
   * Default constructor only initializes the members.
   */
  MaterialLinearElasticity();

  /**
  * Set cross-sectional area
  */
  void SetCrossSectionalArea(double area);

  /**
    * Get cross-sectional area
    */
  double GetCrossSectionalArea() const;

  /**
  * Set youngs/elastic modulus
  */
  void SetYoungsModulus(double modulus);

  /**
   * Get youngs/elastic modulus
   */
  double GetYoungsModulus() const;

  /**
  * Set thickness - for 2D plane stress/strain problems
  */
  void SetThickness(double t);

  /**
   * Get thickness - for 2D plane stress/strain problems
   */
  double GetThickness() const;

  /**
  * Set Moment of intertia - for beam elements
  */
  void SetMomentOfInertia(double iner);

  /**
 * Get Moment of intertia - for beam elements
 */
  double GetMomentOfInertia() const;

  /**
 * Set poisson's ratio
 */
  void SetPoissonsRatio(double poi);

  /**
* Get poisson's ratio
*/
  double GetPoissonsRatio() const;

  /**
* Set density heat product
*/
  void SetDensityHeatProduct(double dhp);

  /**
  * Get density heat product
  */
  double GetDensityHeatProduct() const;

protected:

  /* Data members of MaterialLinearElasticity class */

  /**
   * Young modulus
   */
  double E;

  /**
   * Cross section area of a line element
   */
  double A;  //

  /**
   * Moment of inertia
   */
  double I;

  /**
   * Poisson's ratio
   */
  double nu;

  /**
   * Thickness
   */
  double h;

  /*
   * ... we can add properties here as required without the influence on the already defined elements
   */

  /**
   * Density times Heat Capacity
   */
  double RhoC;
};

FEM_CLASS_INIT(MaterialLinearElasticity)
}
}  // end namespace itk::fem

#endif // #ifndef __itkFEMMaterialLinearElasticity_h
