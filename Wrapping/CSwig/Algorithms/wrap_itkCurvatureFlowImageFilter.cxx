/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    wrap_itkCurvatureFlowImageFilter.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkImage.h"
#include "itkCurvatureFlowImageFilter.h"

#ifdef CABLE_CONFIGURATION
#include "itkCSwigMacros.h"
#include "itkCSwigImages.h"

namespace _cable_
{
  const char* const group = ITK_WRAP_GROUP(itkCurvatureFlowImageFilter);
  namespace wrappers
  {
    ITK_WRAP_OBJECT2(CurvatureFlowImageFilter, image::F2, image::F2,
                     itkCurvatureFlowImageFilterF2F2);
    ITK_WRAP_OBJECT2(CurvatureFlowImageFilter, image::F3, image::F3,
                     itkCurvatureFlowImageFilterF3F3);

    ITK_WRAP_OBJECT2(CurvatureFlowImageFilter, image::D2, image::D2,
                     itkCurvatureFlowImageFilterD2D2);
    ITK_WRAP_OBJECT2(CurvatureFlowImageFilter, image::D3, image::D3,
                     itkCurvatureFlowImageFilterD3D3);
  }
}

#endif
