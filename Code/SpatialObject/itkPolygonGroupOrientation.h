/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkPolygonGroupOrientation.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkPolygonGroupOrientation_h
#define __itkPolygonGroupOrientation_h
namespace itk {
/**
 * \enum PolygonGroupOrientation
 * \brief enumerates the possible spatial orientations
 */
typedef enum {
  Axial = 0,
  Coronal = 1,
  Sagittal = 2,
  UserPlane = 3,
  Unknown = 4
} PolygonGroupOrientation;
}

#endif
