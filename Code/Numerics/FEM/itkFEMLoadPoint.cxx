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

#include "itkFEMLoadPoint.h"

namespace itk
{
namespace fem
{
void LoadPoint::SetPoint(const vnl_vector< Float > p)
{
  this->point = p;
}

vnl_vector< itk::fem::Element::Float > LoadPoint::GetPoint()
{
  return this->point;
}

void LoadPoint::SetForce(const vnl_vector< Float > f)
{
  this->Fp = f;
}

vnl_vector< itk::fem::Element::Float > LoadPoint::GetForce()
{
  return this->Fp;
}

FEM_CLASS_REGISTER(LoadPoint)
}
}  // end namespace itk::fem
