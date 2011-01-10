/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    wrap_itkExpNegativeImageFilter.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkExpNegativeImageFilter.h"
#include "itkImage.h"

#ifdef CABLE_CONFIGURATION
#include "itkCSwigMacros.h"
#include "itkCSwigImages.h"

namespace _cable_
{
  const char* const group = ITK_WRAP_GROUP(itkExpNegativeImageFilter);
  namespace wrappers
  {
    ITK_WRAP_OBJECT2_WITH_SUPERCLASS(ExpNegativeImageFilter, image::F2, image::F2,
                     itkExpNegativeImageFilterF2F2);
    ITK_WRAP_OBJECT2_WITH_SUPERCLASS(ExpNegativeImageFilter, image::F3, image::F3,
                     itkExpNegativeImageFilterF3F3);
    ITK_WRAP_OBJECT2_WITH_SUPERCLASS(ExpNegativeImageFilter, image::US2, image::US2,
                     itkExpNegativeImageFilterUS2US2);
    ITK_WRAP_OBJECT2_WITH_SUPERCLASS(ExpNegativeImageFilter, image::US3, image::US3,
                     itkExpNegativeImageFilterUS3US3);
  }
}

#endif
