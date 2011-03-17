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

#include "itkFEMLightObject.h"
#include "itkNumericTraits.h"

namespace itk
{
namespace fem
{
/**
 * Here we just read the global number from the stream.
 * This should be the first function called when reading object data.
 */
//VAM-NOIO
#if 0
void FEMLightObject::Read(std::istream & f, void *)
{
  int n;

  /** Read and set the global object number */
  this->SkipWhiteSpace(f); f >> n; if ( !f ) { goto out; }
  this->GN = n;

out:

  if ( !f )
    {
    throw FEMExceptionIO(__FILE__, __LINE__, "FEMLightObject::Read", "Error reading FEM object!");
    }
}

void FEMLightObject::Write(std::ostream & f) const
{
  // first write the class name
  f << '<' << FEMObjectFactory< Self >::ID2ClassName( this->ClassID() ) << ">\n";

  // then the global object number
  f << "\t" << GN << "\t% Global object number\n";

  // check for errors
  if ( !f )
    {
    throw FEMExceptionIO(__FILE__, __LINE__, "FEMLightObject::Write", "Error writing FEM object!");
    }
}

/**
 * Read and create object of any derived class from stream
 */
FEMLightObject::Pointer
FEMLightObject::CreateFromStream(std::istream & f, void *info)
{
// local variables
  std::streampos          l(0);
  char                    buf[256];
  std::string             s;
  std::string::size_type  b, e;
  int                     clID;
  FEMLightObject::Pointer a = 0;
  std::string             errorMessage;

start:
  l = f.tellg();     // remember the stream position
  SkipWhiteSpace(f); // skip comments and whitespaces
  if ( f.eof() )
    {
    return 0;              // end of stream. all was good
    }
  char c;
  if ( ( c = f.get() ) != '<' )
    {
    std::string rest;
    std::getline(f, rest);
    errorMessage = "Expected < token not found. Instead found '";
    errorMessage += c;
    errorMessage += "'.\nRest of line is '";
    errorMessage += rest;
    errorMessage += "'.\n";
    goto out; // we expect a token
    }
  f.getline(buf, 256, '>');  // read up to 256 characters until '>' is reached.
                             // we read and discard the '>'
  s = std::string(buf);

  // get rid of the whitespaces in front of and the back of token
  b = s.find_first_not_of(whitespaces);                               // end of
                                                                      // whitespaces
                                                                      // in the
                                                                      // beginning
  if ( ( e = s.find_first_of(whitespaces, b) ) == std::string::npos ) //
                                                                      // beginning
                                                                      // of
                                                                      // whitespaces
                                                                      // at the
                                                                      // end
    {
    e = s.size();
    }
  s = s.substr(b, e - b);

  if ( s == "END" )
    {
    /*
     * We can ignore this token. Start again by reading the next object.
     */
    goto start;
    }
  clID = FEMOF::ClassName2ID(s);  // obtain the class ID from FEMObjectFactory
  if ( clID < 0 )
    {
    errorMessage = "Could not obtain class ID from FEMObjectFactory for '";
    errorMessage += s;
    errorMessage += "'.";
    goto out;  // class not found
    }
  // create a new object of the correct class
  a = FEMOF::Create(clID);
  if ( !a )
    {
    errorMessage = "Error creating new object of the derived class";
    goto out;    // error creating new object of the derived class
    }
  /*
   * Now we have to read additional data, which is
   * specific to the class of object we just created
   */
  try
    {
    a->Read(f, info);
    }
  /**
   * Catch possible exceptions while
   * reading object's data from stream
   */
  catch ( ... )
    {
#ifndef FEM_USE_SMART_POINTERS
    delete a;  // if something went wrong, we need to destroy the already
               // created object
#endif
    a = 0;
    throw;     // rethrow the same exception
    }

  /**
   * Return a pointer to a newly created object if all was OK
   * Technically everithing should be fine here (a!=0), but we
   * check again, just in case.
   */
  if ( a ) { return a; }

out:

  /**
   * Something went wrong.
   * Reset the stream position to where it was before reading the object.
   */
  f.seekg(l);

  /*
   * Throw an IO exception
   */
  throw FEMExceptionIO(__FILE__, __LINE__, "FEMLightObject::ReadAnyObjectFromStream()", errorMessage);
}



// Helper function to skip all whitespaces and comments in input stream
void
FEMLightObject::SkipWhiteSpace(std::istream & f)
{
  std::string skip;

  while ( f && !f.eof() && ( std::ws(f).peek() ) == '%' )
    {
    std::getline(f, skip);
    }
}
#endif
void FEMLightObject::SetGlobalNumber(int gn)
{
  this->GN = gn;
}

int FEMLightObject::GetGlobalNumber() const
{
  return this->GN;
}
//VAM-NOIO
#if 0
// string containing all whitespace characters
const std::string
FEMLightObject
:: whitespaces = " \t\n\r";
#endif
}
}  // end namespace itk::fem
