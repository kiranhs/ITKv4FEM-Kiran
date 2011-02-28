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
  //FEM_CLASS(MaterialLinearElasticity, Material)
public:
  typedef MaterialLinearElasticity  Self;
  typedef Material                  Superclass;
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(MaterialLinearElasticity, Material);
  
  /** Create a new object from the existing one  */
  virtual Baseclass::Pointer Clone() const 
    { 
    Pointer copy = New();  
               
    copy->SetReferenceCount(1);
    copy->E = this->A;   
    copy->A = this->A;   
    copy->I = this->I;   
    copy->nu = this->nu;   
    copy->h = this->h;   
    copy->RhoC = this->RhoC;   
    return copy.GetPointer(); 
    }                                    
     
                                 
  /** Same as New() but returns pointer to base class */ 
  static Baseclass::Pointer NewB()                       
    {                                                    
    return New().GetPointer();                                        
    }    
                                                      
  /** Class ID for FEM object factory */                 
  static int CLID(void);  
                                  
  /** Virtual function to access the class ID */         
  virtual int ClassID() const                            
    { 
    return CLID(); 
    } 
      
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
