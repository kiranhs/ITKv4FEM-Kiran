/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    wrap_itkMeanImageFilter.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkMeanImageFilter.h"
#include "itkImage.h"

#ifdef CABLE_CONFIGURATION
#include "itkCSwigMacros.h"
#include "itkCSwigImages.h"

namespace _cable_
{
  const char* const group = ITK_WRAP_GROUP(itkMeanImageFilter);
  namespace wrappers
  {
    ITK_WRAP_OBJECT2(MeanImageFilter, image::F2, image::F2, 
                     itkMeanImageFilterF2F2);
    ITK_WRAP_OBJECT2(MeanImageFilter, image::US2, image::US2, 
                     itkMeanImageFilterUS2US2);
    ITK_WRAP_OBJECT2(MeanImageFilter, image::F3, image::F3, 
                     itkMeanImageFilterF3F3);
    ITK_WRAP_OBJECT2(MeanImageFilter, image::US3, image::US3, 
                     itkMeanImageFilterUS3US3);
  }
}


#endif
