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
#include "itkTransformFactory.h"
#include "itkVersion.h"

#include "itkAffineTransform.h"
#include "itkBSplineDeformableTransform.h"
#include "itkCenteredAffineTransform.h"
#include "itkCenteredEuler3DTransform.h"
#include "itkCenteredEuler3DTransform.h"
#include "itkCenteredRigid2DTransform.h"
#include "itkCenteredSimilarity2DTransform.h"
#include "itkSimilarity2DTransform.h"
#include "itkEuler2DTransform.h"
#include "itkEuler3DTransform.h"
#include "itkFixedCenterOfRotationAffineTransform.h"
#include "itkIdentityTransform.h"
#include "itkQuaternionRigidTransform.h"
#include "itkRigid2DTransform.h"
#include "itkRigid3DPerspectiveTransform.h"
#include "itkRigid3DTransform.h"
#include "itkScalableAffineTransform.h"
#include "itkScaleLogarithmicTransform.h"
#include "itkScaleVersor3DTransform.h"
#include "itkScaleSkewVersor3DTransform.h"
#include "itkScaleTransform.h"
#include "itkTranslationTransform.h"
#include "itkVersorRigid3DTransform.h"
#include "itkVersorTransform.h"

namespace itk
{
FEMFactoryBase* FEMFactoryBase::m_Factory = 0;

FEMFactoryBase::FEMFactoryBase()
{
}

FEMFactoryBase::~FEMFactoryBase()
{
}

void FEMFactoryBase::RegisterDefaultElements()
{
  if ( !m_Factory )
    {
    FEMFactory< itk::fem::Element2DC0LinearLineStress >::RegisterElement ();
    FEMFactory< Element2DC0LinearQuadrilateralMembrane >::RegisterElement ();
    FEMFactory< Element2DC0LinearQuadrilateralStrain >::RegisterElement ();
    FEMFactory< FEMElement2DC0LinearQuadrilateralStress >::RegisterElement ();
    FEMFactory< Element2DC0LinearTriangularMembrane >::RegisterElement ();
    FEMFactory< Element2DC0LinearTriangularStrain >::RegisterElement ();
    FEMFactory< Element2DC0LinearTriangularStress >::RegisterElement ();
    FEMFactory< Element2DC0QuadraticTriangularStrain >::RegisterElement ();
    FEMFactory< Element2DC0QuadraticTriangularStress >::RegisterElement ();
    FEMFactory< Element2DC1Beam >::RegisterElement ();
    FEMFactory< Element3DC0LinearHexahedronMembrane >::RegisterElement ();
    FEMFactory< Element3DC0LinearHexahedronStrain >::RegisterElement ();
    FEMFactory< Element3DC0LinearTetrahedronMembrane >::RegisterElement ();
    FEMFactory< Element3DC0LinearTetrahedronStrain >::RegisterElement ();
    FEMFactory< Element3DC0LinearTriangularLaplaceBeltrami >::RegisterElement ();
    FEMFactory< Element3DC0LinearTriangularMembrane >::RegisterElement ();
    }
}

void FEMFactoryBase::RegisterDefaultLoads()
{
  if ( !m_Factory )
    {
    FEMFactory< itk::fem::LoadBC >::RegisterLoad ();
    FEMFactory< itk::fem::LoadEdge >::RegisterLoad ();
    FEMFactory< itk::fem::LoadLandmark >::RegisterLoad ();
    FEMFactory< itk::fem::LoadNode >::RegisterLoad ();
    FEMFactory< itk::fem::LoadPoint >::RegisterLoad ();
    FEMFactory< itk::fem::LoadGravConst >::RegisterLoad ();
    FEMFactory< itk::fem::LoadElement >::RegisterLoad ();
    }
}

void FEMFactoryBase::RegisterDefaultMaterials()
{
  if ( !m_Factory )
    {
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
