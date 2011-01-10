/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    wrap_itkVector.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkVector.h"

#ifdef CABLE_CONFIGURATION
#include "itkCSwigMacros.h"
namespace _cable_
{
  const char* const group = ITK_WRAP_GROUP(itkVector);
  namespace wrappers
  {
    typedef itk::Vector<double, 2 >::Vector itkVectorD2;
    typedef itk::Vector<double, 3 >::Vector itkVectorD3;
  }
}
#endif
