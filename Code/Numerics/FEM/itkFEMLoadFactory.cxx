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

#include "itkFEMLoadFactory.h"
#include "itkFEMElement2DC0LinearLineStress.h"
#include "itkFEMElement2DC0LinearQuadrilateral.h"
#include "itkFEMElement2DC0LinearQuadrilateralMembrane.h"
#include "itkFEMElement2DC0LinearQuadrilateralStrain.h"
#include "itkFEMElement2DC0LinearQuadrilateralStress.h"
#include "itkFEMElement2DC0LinearTriangular.h"
#include "itkFEMElement2DC0LinearTriangularMembrane.h"
#include "itkFEMElement2DC0LinearTriangularStrain.h"
#include "itkFEMElement2DC0LinearTriangularStress.h"


#define OVERRIDE_FACTORY( classname ) \
this->RegisterOverride("itkFEM#classname", \
"itkFEM#classname", \
"Create a FEM#classname", \
1, \
CreateObjectFunction<fem::classname>::New());

namespace itk
{

FEMLoadFactory::FEMLoadFactory()
{
  OVERRIDE_FACTORY( Element2DC0LinearLineStress );
  OVERRIDE_FACTORY( Element2DC0LinearQuadrilateralMembrane );
  OVERRIDE_FACTORY( Element2DC0LinearQuadrilateralStrain );
  OVERRIDE_FACTORY( Element2DC0LinearQuadrilateralStress );
}
  
FEMLoadFactory::~FEMLoadFactory()
{

}

const char*
FEMLoadFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char*
FEMLoadFactory::GetDescription() const
{
  return "FEM Load Factory";
}
  
} // end namespace itk


#undef OVERRIDE_FACTORY
