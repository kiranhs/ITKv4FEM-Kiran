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
  E(100.0), A(1.0), I(1.0), nu(0.2), h(1.0), RhoC(1.0)
{}


void MaterialLinearElasticity::SetCrossSectionalArea(double a)
{
  this->A = a;
}

double MaterialLinearElasticity::GetCrossSectionalArea() const
{
  return this->A;
}

void MaterialLinearElasticity::SetYoungsModulus(double y)
{
  this->E = y;
}

double MaterialLinearElasticity::GetYoungsModulus() const
{
  return this->E;
}

void MaterialLinearElasticity::SetThickness(double t)
{
  this->h = t;
}

double MaterialLinearElasticity::GetThickness() const
{
  return this->h;
}

void MaterialLinearElasticity::SetMomentOfInertia(double i)
{
  this->I = i;
}

double MaterialLinearElasticity::GetMomentOfInertia() const
{
  return this->I;
}

void MaterialLinearElasticity::SetPoissonsRatio(double pr)
{
  this->nu = pr;
}

double MaterialLinearElasticity::GetPoissonsRatio() const
{
  return this->nu;
}

void MaterialLinearElasticity::SetDensityHeatProduct(double dhp)
{
  this->RhoC = dhp;
}

double MaterialLinearElasticity::GetDensityHeatProduct() const
{
  return this->RhoC;
}

FEM_CLASS_REGISTER(MaterialLinearElasticity)
}
}  // end namespace itk::fem
