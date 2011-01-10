/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    wrap_itkMinimumMaximumImageCalculator.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkMinimumMaximumImageCalculator.h"
#include "itkImage.h"

#ifdef CABLE_CONFIGURATION
#include "itkCSwigMacros.h"
#include "itkCSwigImages.h"

namespace _cable_
{
  const char* const group = ITK_WRAP_GROUP(itkMinimumMaximumImageCalculator);
  namespace wrappers
  {
    ITK_WRAP_OBJECT1(MinimumMaximumImageCalculator, image::F2, itkMinimumMaximumImageCalculatorF2);
    ITK_WRAP_OBJECT1(MinimumMaximumImageCalculator, image::F3, itkMinimumMaximumImageCalculatorF3);
    ITK_WRAP_OBJECT1(MinimumMaximumImageCalculator, image::US2, itkMinimumMaximumImageCalculatorUS2);
    ITK_WRAP_OBJECT1(MinimumMaximumImageCalculator, image::US3, itkMinimumMaximumImageCalculatorUS3);
  }
}

#endif
