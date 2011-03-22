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

#include <iostream>
#include <locale>
#include <cstdio>
#include <ios>

#include "itkFEMElementFactory.h"
//#include "itkFEMElementBase.h"
#include "itkFEMElement2DC0LinearLineStress.h"
#include "itkFEMElement2DC0LinearQuadrilateralMembrane.h"
#include "itkFEMElement2DC0LinearQuadrilateralStrain.h"
#include "itkFEMElement2DC0LinearQuadrilateralStress.h"
#include "itkFEMElement2DC0LinearTriangularMembrane.h"
#include "itkFEMElement2DC0LinearTriangularStrain.h"
#include "itkFEMElement2DC0LinearTriangularStress.h"
#include "itkFEMElement2DC0QuadraticTriangularStrain.h"
#include "itkFEMElement2DC0QuadraticTriangularStress.h"
#include "itkFEMElement2DC1Beam.h"
#include "itkFEMElement3DC0LinearHexahedronMembrane.h"
#include "itkFEMElement3DC0LinearHexahedronStrain.h"
#include "itkFEMElement3DC0LinearTetrahedronMembrane.h"
#include "itkFEMElement3DC0LinearTetrahedronStrain.h"
#include "itkFEMElement3DC0LinearTriangularLaplaceBeltrami.h"
#include "itkFEMElement3DC0LinearTriangularMembrane.h"


#define OVERRIDE_FACTORY( classname ) 

#if 0
#define OVERRIDE_FACTORY( classname ) \
this->RegisterOverride("itkFEM#classname", \
"itkFEM#classname", \
"Create a FEM#classname", \
1, \
CreateObjectFunction<fem::classname>::New());
#endif

namespace itk
{

FEMElementFactory::FEMElementFactory()
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
  
FEMElementFactory::~FEMElementFactory()
{

}

const char*
FEMElementFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char*
FEMElementFactory::GetDescription() const
{
  return "FEM Element Factory";
}
  
} // end namespace itk


#undef OVERRIDE_FACTORY
