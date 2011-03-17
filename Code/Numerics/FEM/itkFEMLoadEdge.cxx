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

#include "itkFEMLoadEdge.h"

namespace itk
{
namespace fem
{
/**
 * Read the Load object from input stream
 */
//VAM-NOIO
#if 0
void LoadEdge::Read(std::istream & f, void *info)
{
  int n, m;

  /** first call the parent's read function */
  Superclass::Read(f, info);

  /** ... edge number */
  this->SkipWhiteSpace(f); f >> n; if ( !f ) { goto out; }
  m_Edge = n;
  /** ... # of rows */
  this->SkipWhiteSpace(f); f >> n; if ( !f ) { goto out; }
  /** ... # of cols */
  this->SkipWhiteSpace(f); f >> m; if ( !f ) { goto out; }
  m_Force.set_size(n, m);
  for ( int i = 0; i < n; i++ )
    {
    this->SkipWhiteSpace(f);
    for ( int j = 0; j < m; j++ )
      {
      f >> m_Force[i][j];
      }
    this->SkipWhiteSpace(f);
    }

out:

  if ( !f )
    {
    throw FEMExceptionIO(__FILE__, __LINE__, "LoadEdge::Read()", "Error reading FEM load!");
    }
}

/**
 * Write the Load object to the output stream
 */
void LoadEdge::Write(std::ostream & f) const
{
  /** first call the parent's write function */
  Superclass::Write(f);

  /** Write the actual Load data */

  /** ... edge number */
  f << "\t" << m_Edge << "\t% Edge number" << "\n";

  /** ... force matrix */
  f << "\t" << m_Force.rows() << "\t% # rows in force matrix" << "\n";
  f << "\t" << m_Force.cols() << "\t% # cols in force matrix" << "\n";
  f << "\t% force matrix\n";
  for ( int i = 0; i < (int)m_Force.rows(); i++ )
    {
    f << "\t";
    for ( int j = 0; j < (int)m_Force.cols(); j++ )
      {
      f << m_Force[i][j] << " ";
      }
    f << "\n";
    }

  /** check for errors */
  if ( !f )
    {
    throw FEMExceptionIO(__FILE__, __LINE__, "LoadBCMFC::Write()", "Error writing FEM load!");
    }
}
#endif
void LoadEdge::SetEdge(int edge)
{
  this->m_Edge = edge;
}

int LoadEdge::GetEdge()
{
  return this->m_Edge;
}

void LoadEdge::SetForce(const vnl_matrix< itk::fem::Element::Float > force)
{
  this->m_Force = force;
}

vnl_matrix< itk::fem::Element::Float >& LoadEdge::GetForce()
{
  return this->m_Force;
}

  
// Explicit New() method, used here because we need to split the itkNewMacro()
// in order to overload the CreateAnother() method.
LoadEdge::Pointer LoadEdge::New(void)
{
  Pointer smartPtr = ::itk::ObjectFactory< Self >::Create();
  if(smartPtr.IsNull())
  {
    smartPtr = static_cast<Pointer>(new Self);
  }
  smartPtr->UnRegister();
  return smartPtr;
}

// Explicit New() method, used here because we need to split the itkNewMacro()
// in order to overload the CreateAnother() method.  
::itk::LightObject::Pointer LoadEdge::CreateAnother(void) const
{
  ::itk::LightObject::Pointer smartPtr;
  Pointer copyPtr = Self::New().GetPointer();
  
  copyPtr->m_Edge = this->m_Edge;
  copyPtr->m_Force = this->m_Force;
  copyPtr->el = this->el;
  copyPtr->GN = this->GN;
  
  smartPtr = static_cast<Pointer>(copyPtr);
  
  return smartPtr;
}
  
FEM_CLASS_REGISTER(LoadEdge)
}
}  // end namespace itk::fem
