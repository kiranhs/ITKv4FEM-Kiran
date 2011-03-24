/*=========================================================================

Program:   Insight Segmentation & Registration Toolkit
Module:    itkFEMFactoryBase.cxx
Language:  C++
Date:      $Date$
Version:   $Revision$

Copyright (c) Insight Software Consortium. All rights reserved.
See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkFEMFactoryBase.h"
#include "itkFEMFactory.h"
#include "itkVersion.h"

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

#include "itkFEMLoadBC.h"
#include "itkFEMLoadBCMFC.h"
#include "itkFEMLoadEdge.h"
#include "itkFEMLoadElementBase.h"
#include "itkFEMLoadGrav.h"
#include "itkFEMLoadLandmark.h"
#include "itkFEMLoadNode.h"
#include "itkFEMLoadPoint.h"

#include "itkFEMMaterialLinearElasticity.h"

namespace itk
{
FEMFactoryBase* FEMFactoryBase::m_Factory = 0;

FEMFactoryBase::FEMFactoryBase()
{
}

FEMFactoryBase::~FEMFactoryBase()
{
}

void FEMFactoryBase::RegisterDefaultTypes()
{
  if ( !m_Factory )
    {
    FEMFactory< fem::Element::Node >::RegisterElement ();
    FEMFactory< fem::Element2DC0LinearLineStress >::RegisterElement ();
    FEMFactory< fem::Element2DC0LinearQuadrilateralMembrane >::RegisterElement ();
    FEMFactory< fem::Element2DC0LinearQuadrilateralStrain >::RegisterElement ();
    FEMFactory< fem::Element2DC0LinearQuadrilateralStress >::RegisterElement ();
    FEMFactory< fem::Element2DC0LinearTriangularMembrane >::RegisterElement ();
    FEMFactory< fem::Element2DC0LinearTriangularStrain >::RegisterElement ();
    FEMFactory< fem::Element2DC0LinearTriangularStress >::RegisterElement ();
    FEMFactory< fem::Element2DC0QuadraticTriangularStrain >::RegisterElement ();
    FEMFactory< fem::Element2DC0QuadraticTriangularStress >::RegisterElement ();
    FEMFactory< fem::Element2DC1Beam >::RegisterElement ();
    FEMFactory< fem::Element3DC0LinearHexahedronMembrane >::RegisterElement ();
    FEMFactory< fem::Element3DC0LinearHexahedronStrain >::RegisterElement ();
    FEMFactory< fem::Element3DC0LinearTetrahedronMembrane >::RegisterElement ();
    FEMFactory< fem::Element3DC0LinearTetrahedronStrain >::RegisterElement ();
    FEMFactory< fem::Element3DC0LinearTriangularLaplaceBeltrami >::RegisterElement ();
    FEMFactory< fem::Element3DC0LinearTriangularMembrane >::RegisterElement ();
    FEMFactory< itk::fem::LoadBC >::RegisterLoad ();
    FEMFactory< itk::fem::LoadEdge >::RegisterLoad ();
    FEMFactory< itk::fem::LoadLandmark >::RegisterLoad ();
    FEMFactory< itk::fem::LoadNode >::RegisterLoad ();
    FEMFactory< itk::fem::LoadPoint >::RegisterLoad ();
    FEMFactory< itk::fem::LoadGravConst >::RegisterLoad ();
    FEMFactory< itk::fem::LoadElement >::RegisterLoad ();
    FEMFactory< itk::fem::MaterialLinearElasticity >::RegisterMaterial ();
    }
}

const char*
FEMFactoryBase::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char*
FEMFactoryBase::GetDescription() const
{
  return "Transform FactoryBase";
}

} // end namespace itk
