/*=========================================================================
*
* Copyright Insight Software Consortium
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0.txt
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
*=========================================================================*/

#ifndef __itkFEMElement3DMembrane1DOF_txx
#define __itkFEMElement3DMembrane1DOF_txx

#include "itkFEMElement3DMembrane1DOF.h"

namespace itk
{
namespace fem
{
template< class TBaseClass >
Element3DMembrane1DOF< TBaseClass >
::Element3DMembrane1DOF():Superclass(), m_mat(0) {}

//////////////////////////////////////////////////////////////////////////
/*
 * Methods related to the physics of the problem.
 */

template< class TBaseClass >
void
Element3DMembrane1DOF< TBaseClass >
::GetStrainDisplacementMatrix(MatrixType & B, const MatrixType & shapeDgl) const
{}

template< class TBaseClass >
void
Element3DMembrane1DOF< TBaseClass >
::GetMassMatrix(MatrixType & Me) const
{
  // Call the parent's get matrix function
  Superclass::GetMassMatrix(Me);

  // Since parent class doesn't have the material properties,
  // we need to adjust Me matrix here for the density of the element.
  Me = Me * m_mat->GetDensityHeatProduct();
}

template< class TBaseClass >
void
Element3DMembrane1DOF< TBaseClass >
::GetMaterialMatrix(MatrixType & D) const
{
  unsigned int d = 3;

  D.set_size(d, d);

  D.fill(0.0);

  // This is the main difference from the linear elasticity problem.
  /* Material properties matrix.  Simpler than linear elasticity. */
  Float disot = m_mat->GetYoungsModulus();

  for ( unsigned int i = 0; i < d; i++ )
    {
    D[i][i] = disot;
    }
}

template< class TBaseClass >
void Element3DMembrane1DOF< TBaseClass >::GetStiffnessMatrix(MatrixType & Ke) const
{
  Superclass::GetStiffnessMatrix(Ke);
}

template< class TBaseClass >
void
Element3DMembrane1DOF< TBaseClass >
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Materials: " << this->m_mat << std::endl;
}

}
}  // end namespace itk::fem

#endif // #ifndef __itkFEMElement3DMembrane1DOF_txx
