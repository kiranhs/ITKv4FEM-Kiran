/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkTransformFactoryBase.h
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
#ifndef __itkFEMFactoryBase_h
#define __itkFEMFactoryBase_h

#include "itkObjectFactoryBase.h"

namespace itk
{
/** \class FEMFactoryBase
 * \brief Create instances of FEM Objects.
 * This includes Elements, Loads, and Materials
 */

class ITKCommon_EXPORT FEMFactoryBase : public ObjectFactoryBase
{
public:  
  /** Standard class typedefs. */
  typedef FEMFactoryBase              Self;
  typedef ObjectFactoryBase           Superclass;
  typedef SmartPointer< Self >        Pointer;
  typedef SmartPointer< const Self >  ConstPointer;
  
  /** Class methods used to interface with the registered factories. */
  virtual const char* GetITKSourceVersion(void) const;
  virtual const char* GetDescription(void) const;

  /** Run-time type information (and related methods). */
  itkTypeMacro(FEMFactoryBase, ObjectFactoryBase);

  /** Method for class instantiation. */
  itkFactorylessNewMacro(Self);

  /** Register all builtin transforms */
  static void RegisterDefaultTypes();
  

  /** Register this transform */
  static FEMFactoryBase* GetFactory () 
    {
    if ( m_Factory == 0 )
      {
      // Make and register the factory
      FEMFactoryBase::Pointer p = FEMFactoryBase::New();
      m_Factory = p.GetPointer();
      ObjectFactoryBase::RegisterFactory ( p );
      p->RegisterDefaultTypes ();
      }
    return m_Factory;
    }

  void RegisterType(const char* classOverride,
      const char* overrideClassName,
      const char* description,
      bool enableFlag,
      CreateObjectFunctionBase* createFunction)
    {
    this->RegisterOverride ( classOverride, overrideClassName, description, enableFlag, createFunction );
    }
  
     
protected:
  FEMFactoryBase();
  virtual ~FEMFactoryBase();

private:
  FEMFactoryBase(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  static FEMFactoryBase* m_Factory;
};
} // end namespace itk

#endif
