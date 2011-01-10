/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkDecoratorTest.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <iostream>
#include "itkVector.h"
#include "itkAffineTransform.h"
#include "itkSimpleDataObjectDecorator.h"
#include "itkDataObjectDecorator.h"
#include "itkAutoPointerDataObjectDecorator.h"

// Problems with gcc 2.95. Temporary workaround
#if !(defined(__GNUC__) && __GNUC__ <= 2)
#define OK
#endif

#ifdef OK
namespace {
template <class charT, class traits, class T, class A>
std::basic_ostream<charT, traits>&
operator<<(std::basic_ostream<charT, traits>&os, const std::vector<T,A> &p) 
{
  os << "vector<" << typeid(T).name() << "> with " << p.size() << " elements " << std::endl;
  return os;
}
}
#endif

int itkDecoratorTest(int, char* [] )
{
  int status = 0;

  std::cout << "----------------------------------------------------"
            << std::endl;
  
  typedef itk::SimpleDataObjectDecorator<float> FloatObjectType;

  FloatObjectType::Pointer f = FloatObjectType::New();
  f->Set(5.0);

  std::cout << "Value of f: " << f->Get() << std::endl;
  std::cout << "FloatDataObject: " << f << std::endl;

  std::cout << "----------------------------------------------------"
            << std::endl;

  typedef itk::AffineTransform<double, 3> TransformType;
  typedef itk::DataObjectDecorator<TransformType> TransformObjectType;

  TransformObjectType::Pointer decoratedTransform = TransformObjectType::New();
  TransformType::Pointer transformObject = TransformType::New();
  const TransformType * constTransformObject = transformObject;

  transformObject->Scale( 5.0 );

  decoratedTransform->Set( constTransformObject );
  
  std::cout << "Value of decoratedTransform: ";
  decoratedTransform->Get()->Print(std::cout);
  std::cout << "TransformDataObject: " << decoratedTransform;

  std::cout << "----------------------------------------------------"
            << std::endl;

  typedef std::vector<float> VectorType;
  typedef VectorType* VectorPointer;
  typedef itk::SimpleDataObjectDecorator<VectorType> VectorObjectType;
  typedef itk::AutoPointerDataObjectDecorator<VectorType> VectorPointerObjectType;

  VectorType v;
  v.resize(5);
#ifdef OK
  std::cout << v << std::endl;
#endif  
  VectorObjectType::Pointer vo = VectorObjectType::New();
  vo->Set(v);
#ifdef OK
  std::cout << vo;
#endif
  std::cout << "----------------------------------------------------"
            << std::endl;

  // The following code block will NOT cause a memory leak because the
  // ownership of the dynamically allocated memory is passed to the
  // AutoPointerDataObjectDecorator 
  {
  VectorPointer vp;
  vp = new VectorType;
  vp->resize(3);
#ifdef OK
  std::cout << *vp << std::endl;
#endif

  VectorPointerObjectType::Pointer vop = VectorPointerObjectType::New();
  vop->Set(vp);

#ifdef OK
  std::cout << vop;
#endif
  }

  std::cout << "----------------------------------------------------"
            << std::endl;

  // The following code block will cause a memory leak because the
  // decorator does not deallocate the memory that was passed in on a
  // pointer. The AutoPointerDataObjectDecorator does delete the memory.
  //typedef itk::SimpleDataObjectDecorator<VectorPointer> VectorPointerObjectType2;
  //{
  //VectorPointer vp2;
  //vp2 = new VectorType;
  //vp2->resize(4);
  //std::cout << *vp2 << std::endl;
  
  //VectorPointerObjectType2::Pointer vop2 = VectorPointerObjectType2::New();
  //vop2->Set(vp2);

  //std::cout << vop2;
  //}

  return status;
}
