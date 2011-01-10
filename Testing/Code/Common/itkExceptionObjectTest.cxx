/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkExceptionObjectTest.cxx
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


#include "itkExceptionObject.h"
#include <iostream>

class mammal
 {
 public:  
   virtual int GetType()=0;  
   virtual bool operator== (mammal &); 
   mammal() {};
   virtual ~mammal() {};
 };

class human : public mammal 
{  
  public: 
    int GetType() 
    { 
      return 32; 
    }
};

class naked_mole_rat : public mammal
{ 
  public:  
    int GetType() 
    { 
      return 2; 
    }
};

bool mammal::operator== (mammal &o)
{
  if ( this->GetType() != o.GetType() )
    {
    itk::IncompatibleOperandsError e(__FILE__, __LINE__);
    e.SetLocation("bool mammal::operator==(mammal&, mammal&)");
    e.SetDescription("Cannot compare mammals of unequal type");
    throw e;
    }
  else
    {
    if ( /* blah blah blah */ 1) 
      {
      return true;
      }
    else 
      {
      return false;
      }
    }
}

int lookup(const int& i)
{
  static int table[5] = { 23,42,42,32,12 };
  if ( ! ( 0 <= i && i < 5 ) )
    {
    itk::RangeError e(__FILE__, __LINE__);
    e.SetLocation("int lookup(const int& )");
    e.SetDescription("Attempted to access out-of-bounds array element");
    throw e;
    }
  return table[i];
}


int itkExceptionObjectTest(int, char* [] )
{
  // SOME BASIC TESTS OF THE itk::ExceptionObject 's
  
  itk::RangeError E;
  E.SetLocation("itkExceptionObjectTest(int, char**)");
  E.SetDescription("E");
  std::cout << E << std::endl;
  itk::RangeError F(E);
  std::cout << F << std::endl;
  itk::RangeError G;
  G.SetLocation("itkExceptionObjectTest(int, char**)");
  G.SetDescription("G");
  std::cout << "F==G? " << (F==G) << std::endl;
  E = F = G;
  std::cout << F << std::endl;
  std::cout << "F==G? " << (F==G) << std::endl;

  itk::RangeError *Ep  = new itk::RangeError;
  Ep->SetLocation("itkExceptionObjectTest(int, char**)");
  Ep->SetDescription("Ep");
  itk::RangeError *Fp = new itk::RangeError;
  *Fp = *Ep;
  delete Ep;
  std::cout << *Fp << std::endl;
  
  // ** BE SURE TO CATCH BY REFERENCE TO AVOID SLICING **
  bool raised = false;
  try {
    lookup(4);  // OK
    lookup(12); // ERROR
    }
  catch (itk::ExceptionObject &e) 
    { 
    std::cout << e << std::endl; 
    raised = true;
    }
  if( !raised )
    {
    return EXIT_FAILURE;
    }

  raised = false;
  try
    {
    human john, jane;
    naked_mole_rat hal;
    john == john;  // OK
    jane == john;  // OK
    hal == john;   // ERROR
    }
  catch (itk::IncompatibleOperandsError &e) 
    { 
    std::cout << e << std::endl; 
    raised = true;
    }
  if( !raised )
    {
    return EXIT_FAILURE;
    }

  /*
  // SAMPLE ERROR STUFF
  itk::SampleError Se;
  Se.SetLocation("SE LOCATION");
  Se.SetDescription("SE DESCRIPTION");
  itk::SampleError Sf(Se);
  itk::SampleError Sg;
  Sg = Sf;
  std::cout << Sg << std::endl;
  

  try
  {
    itk::SampleError E;
    E.SetLocation("djibouti");
    E.SetDescription("sample error");
    throw E;
  }
  catch (itk::ExceptionObject &e) { std::cout << e << std::endl; }
  */

  delete Fp;

  return EXIT_SUCCESS;
  
}
