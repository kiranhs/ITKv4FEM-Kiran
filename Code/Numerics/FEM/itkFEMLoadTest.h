/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#ifndef __itkFEMLoadTest_h
#define __itkFEMLoadTest_h

#include "itkFEMLoadElementBase.h"

namespace itk
{
namespace fem
{
/**
 * \class LoadTest
 * \brief Example to show how to define templated load classes.
 *
 * \note The class must be instantiated, before the object factory can
 *       produce more objects of this class. Instantiate the specific
 *       derived classes with: "template class LoadTest<...>;" where required.
 */
template< class TClass >
class LoadTest:public LoadElement
{
  //FEM_CLASS(LoadTest, LoadElement)
public:
  typedef LoadTest                Self;
  typedef LoadElement                    Superclass;
  typedef SmartPointer< Self >                               Pointer;
  typedef SmartPointer< const Self >                         ConstPointer;
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(LoadTest, LoadElement);
   
  /***VAM***/
  static int CLID(void);
  virtual int ClassID() const  { return CLID(); }
  
  virtual Baseclass::Pointer Clone() const
    { 
          Pointer o = New();
          o->SetReferenceCount(1);
          o->data = this->data;
  
          o->GN  = this->GN; 
          return o.GetPointer();
    }
    
  /** Same as New() but returns pointer to base class */
  static Baseclass::Pointer NewB()
    {
    return New().GetPointer();
    }
    
    
  /**
   * Default constructor
   */
  LoadTest() {}

  /**
   * Some data that this load defines.
   */
  TClass data;

  virtual void Read(std::istream & f, void *info)
  {
    Superclass::Read(f, info);
  }

  void Write(std::ostream & f) const
  {
    // call the parent's write function
    Superclass::Write(f);
  }

private:
  /** Dummy static int that enables automatic registration
      with FEMObjectFactory. */
  static const int DummyCLID;
};

// Provide the templated code for CLID function, that is
// otherwise generated automaticly with FEM_CLASS_REGISTER
// macro.
template< class TClass >
int LoadTest< TClass >::CLID(void)
{
  static const int CLID_ = FEMOF::Register( LoadTest::NewB, ( std::string("LoadTest(")
                                                              + typeid( TClass ).name() + ")" ).c_str() );

  return CLID_;
}

// Make sure that the class is registered with FEMObjectFactory
// by calling CLID() static member function each time the class
// is instantiated for a specific template parameter TClass.
template< class TClass >
const int LoadTest< TClass >::DummyCLID = LoadTest< TClass >::CLID();
}
}  // end namespace itk::fem

#endif // #ifndef __itkFEMLoadTest_h
