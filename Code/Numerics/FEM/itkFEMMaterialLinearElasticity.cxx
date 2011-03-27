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

#include "itkFEMMaterialLinearElasticity.h"

namespace itk
{
namespace fem
{
/**
 * Default constructor
 */
MaterialLinearElasticity::MaterialLinearElasticity():
  m_YoungModulus(100.0), 
  m_CrossSectionalArea(1.0), 
  m_MomentOfInertia(1.0), 
  m_PoissonRatio(0.2), 
  m_Thickness(1.0), 
  m_DensityHeatCapacity(1.0)
{}


void MaterialLinearElasticity::SetCrossSectionalArea(double a)
{
  this->m_CrossSectionalArea = a;
}

double MaterialLinearElasticity::GetCrossSectionalArea() const
{
  return this->m_CrossSectionalArea;
}

void MaterialLinearElasticity::SetYoungsModulus(double y)
{
  this->m_YoungModulus = y;
}

double MaterialLinearElasticity::GetYoungsModulus() const
{
  return this->m_YoungModulus;
}

void MaterialLinearElasticity::SetThickness(double t)
{
  this->m_Thickness = t;
}

double MaterialLinearElasticity::GetThickness() const
{
  return this->m_Thickness;
}

void MaterialLinearElasticity::SetMomentOfInertia(double i)
{
  this->m_MomentOfInertia = i;
}

double MaterialLinearElasticity::GetMomentOfInertia() const
{
  return this->m_MomentOfInertia;
}

void MaterialLinearElasticity::SetPoissonsRatio(double pr)
{
  this->m_PoissonRatio = pr;
}

double MaterialLinearElasticity::GetPoissonsRatio() const
{
  return this->m_PoissonRatio;
}

void MaterialLinearElasticity::SetDensityHeatProduct(double dhp)
{
  this->m_DensityHeatCapacity = dhp;
}

double MaterialLinearElasticity::GetDensityHeatProduct() const
{
  return this->m_DensityHeatCapacity;
}

}
}  // end namespace itk::fem
