/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkTransformFactory.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkFEMFactory_h
#define __itkFEMFactory_h

#include "itkFEMFactoryBase.h"

namespace itk
{
/** \class FEMFactory
 * \brief Create instances of FEM Objects
 * This includes Elements, Loads, and Materials
 */

template <class T>
class FEMFactory : public FEMFactoryBase
{
public:  
  static void RegisterElement ( )
    {
    typename T::Pointer t = T::New();

    FEMFactoryBase::Pointer f = FEMFactoryBase::GetFactory();

    f->RegisterFEMObject ( t->GetClassName().c_str(),
                           t->GetClassName().c_str(),
                           t->GetClassName().c_str(),
                           1,
                           CreateObjectFunction<T>::New() );
    }
};
} // end namespace itk

#endif
