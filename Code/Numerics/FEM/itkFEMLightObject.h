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

#ifndef __itkFEMLightObject_h
#define __itkFEMLightObject_h

#include "itkFEMMacro.h"
#include "itkFEMException.h"
#include <iostream>

namespace itk
{
namespace fem
{
/**
 * \class FEMLightObject
 * \brief Base class for all classes that define the FEM system.
 *
 * Base class for all classes that define FEM system (Elements, Nodes...).
 * Every FEM object requires a global number that can be used to find
 * that object. It is also required that the object can be written to and
 * read from a file (stream). This functionality is implemented inside
 * FEMLightObject class.
 */
class FEMLightObject:public itk::LightObject
{
public:
  /** Standard class typedefs. */
  typedef FEMLightObject                      Self;
  typedef itk::LightObject                    Superclass;
  typedef Self                                Baseclass;
  typedef SmartPointer< Self >                Pointer;
  typedef SmartPointer< const Self >          ConstPointer;
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(FEMLightObject, itk::LightObject);
  
 
  /**
   * Duplicates the currect object. This function must be implemented
   * by every derived class to create an exact copy of an object. The
   * function returns a pointer to a base class.
   */
//VAM - Remove ????
  virtual Baseclass::Pointer Clone() const = 0;


  /**
   * Set the global number of the object
   */
  void SetGlobalNumber(int);

  /**
  * Get the global number of the object
  */
  int GetGlobalNumber() const;

protected:  // If we're using smart pointers, constructors and destructors
            // should be protected.
            
  /**
   * Default constructor
   */
  FEMLightObject():GN(-1) {}

  /**
   * Virtual destructor
   */
  virtual ~FEMLightObject() {}

  /**
   * Copy constructor must be available for the FEM objects...
   */
  FEMLightObject(const FEMLightObject & o) { GN = o.GN; }
protected:

  /**
   * Global number of an object (ID of an object)
   * In general the ID's are required to be unique only within
   * a specific type of derived classes (Elements, Nodes, ...)
   * If the GN is not required, it can be ignored. (normally you
   * need the GN when writing or reading objects to/from stream.
   */
//VAM - Change to m_GlobalNumber - Use macros to get and set this value
  int GN;
};

}
}  // end namespace itk::fem

#endif // #ifndef __itkFEMLightObject_h
