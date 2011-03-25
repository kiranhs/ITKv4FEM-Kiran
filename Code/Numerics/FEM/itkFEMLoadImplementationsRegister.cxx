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

// disable debug warnings in MS compiler
#ifdef _MSC_VER
#pragma warning(disable: 4786)
#endif

#include "itkVisitorDispatcher.h"

#include "itkFEMLoadPoint.h"
#include "itkFEMLoadGrav.h"
#include "itkFEMLoadEdge.h"
#include "itkFEMLoadLandmark.h"

#include "itkFEMLoadImplementationGenericBodyLoad.h"
#include "itkFEMLoadImplementationGenericEdgeLoad.h"
#include "itkFEMLoadImplementationGenericLandmarkLoad.h"

#include "itkFEMElement2DC0LinearLineStress.h"
#include "itkFEMElement2DC1Beam.h"
#include "itkFEMElement2DC0LinearTriangularStress.h"
#include "itkFEMElement2DC0LinearTriangularStrain.h"
#include "itkFEMElement2DC0LinearTriangularMembrane.h"
#include "itkFEMElement2DC0LinearQuadrilateralStress.h"
#include "itkFEMElement2DC0LinearQuadrilateralStrain.h"
#include "itkFEMElement2DC0LinearQuadrilateralMembrane.h"
#include "itkFEMElement3DC0LinearTetrahedronStrain.h"
#include "itkFEMElement3DC0LinearHexahedronStrain.h"
#include "itkFEMElement3DC0LinearTetrahedronMembrane.h"
#include "itkFEMElement3DC0LinearHexahedronMembrane.h"

namespace itk
{
namespace fem
{


/**
 * Registers all Load classes in the FEM library with VisitorDispatcher.
 * This function must be called before the FEM library is functional!.
 */
void LoadImplementationsRegister(void)
{
  
  
}
}
}  // end namespace itk::fem
