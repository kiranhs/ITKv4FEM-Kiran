/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    wrap_itkOtsuThresholdImageCalculator.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkOtsuThresholdImageCalculator.h"
#include "itkImage.h"

#ifdef CABLE_CONFIGURATION
#include "itkCSwigMacros.h"
#include "itkCSwigImages.h"

namespace _cable_
{
  const char* const group = ITK_WRAP_GROUP(itkOtsuThresholdImageCalculator);
  namespace wrappers
  {
    ITK_WRAP_OBJECT1(OtsuThresholdImageCalculator, image::F2,
                     itkOtsuThresholdImageCalculatorF2);
    ITK_WRAP_OBJECT1(OtsuThresholdImageCalculator, image::F3,
                     itkOtsuThresholdImageCalculatorF3);
    ITK_WRAP_OBJECT1(OtsuThresholdImageCalculator, image::US2,
                     itkOtsuThresholdImageCalculatorUS2);
    ITK_WRAP_OBJECT1(OtsuThresholdImageCalculator, image::US3,
                     itkOtsuThresholdImageCalculatorUS3);
  }
}

#endif
