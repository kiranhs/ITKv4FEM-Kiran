/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    wrap_itkBinaryThresholdImageFilter.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkBinaryThresholdImageFilter.h"
#include "itkImage.h"

#ifdef CABLE_CONFIGURATION
#include "itkCSwigMacros.h"
#include "itkCSwigImages.h"

namespace _cable_
{
  const char* const group = ITK_WRAP_GROUP(itkBinaryThresholdImageFilter);
  namespace wrappers
  {
    ITK_WRAP_OBJECT2_WITH_SUPERCLASS(BinaryThresholdImageFilter, image::F2, image::US2, 
                                     itkBinaryThresholdImageFilterF2US2);
    ITK_WRAP_OBJECT2_WITH_SUPERCLASS(BinaryThresholdImageFilter, image::US2, image::US2, 
                                     itkBinaryThresholdImageFilterUS2US2);
    ITK_WRAP_OBJECT2_WITH_SUPERCLASS(BinaryThresholdImageFilter, image::F3, image::US3, 
                                     itkBinaryThresholdImageFilterF3US3);
    ITK_WRAP_OBJECT2_WITH_SUPERCLASS(BinaryThresholdImageFilter, image::US3, image::US3, 
                                     itkBinaryThresholdImageFilterUS3US3);
  }
}

#endif
