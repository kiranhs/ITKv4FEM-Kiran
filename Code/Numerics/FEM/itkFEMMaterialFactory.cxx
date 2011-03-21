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

#include "itkFEMMaterialFactory.h"




#define OVERRIDE_FACTORY( classname ) \
this->RegisterOverride("itkfem#classname", \
"itkfem#classname", \
"Create a FEM#classname", \
1, \
CreateObjectFunction<fem::classname>::New());

namespace itk
{

FEMMaterialFactory::FEMMaterialFactory()
{
  OVERRIDE_FACTORY( Element2DC0LinearLineStress );
  OVERRIDE_FACTORY( Element2DC0LinearQuadrilateralMembrane );
  OVERRIDE_FACTORY( Element2DC0LinearQuadrilateralStrain );
  OVERRIDE_FACTORY( FEMElement2DC0LinearQuadrilateralStress );
  OVERRIDE_FACTORY( Element2DC0LinearTriangularMembrane );
  OVERRIDE_FACTORY( Element2DC0LinearTriangularStrain );
  OVERRIDE_FACTORY( Element2DC0LinearTriangularStress );
  OVERRIDE_FACTORY( Element2DC0QuadraticTriangularStrain );
  OVERRIDE_FACTORY( Element2DC0QuadraticTriangularStress );
  OVERRIDE_FACTORY( Element2DC1Beam );
  OVERRIDE_FACTORY( Element3DC0LinearHexahedronMembrane );
  OVERRIDE_FACTORY( Element3DC0LinearHexahedronStrain );
  OVERRIDE_FACTORY( Element3DC0LinearTetrahedronMembrane );
  OVERRIDE_FACTORY( Element3DC0LinearTetrahedronStrain );
  OVERRIDE_FACTORY( Element3DC0LinearTriangularLaplaceBeltrami );
  OVERRIDE_FACTORY( Element3DC0LinearTriangularMembrane );
}
  
FEMMaterialFactory::~FEMMaterialFactory()
{

}

const char*
FEMMaterialFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char*
FEMMaterialFactory::GetDescription() const
{
  return "FEM Element Factory";
}
  
} // end namespace itk


#undef OVERRIDE_FACTORY
