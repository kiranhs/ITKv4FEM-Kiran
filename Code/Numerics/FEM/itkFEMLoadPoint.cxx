/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFEMLoadPoint.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

// disable debug warnings in MS compiler
#ifdef _MSC_VER
#pragma warning(disable: 4786)
#endif

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
