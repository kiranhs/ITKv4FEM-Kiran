/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkVolumeSplineKernelTransform.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkVolumeSplineKernelTransform_txx
#define __itkVolumeSplineKernelTransform_txx
#include "itkVolumeSplineKernelTransform.h"

namespace itk
{

template <class TScalarType, unsigned int NDimensions>
void
VolumeSplineKernelTransform<TScalarType, NDimensions>::
ComputeG(const InputVectorType & x, GMatrixType & gmatrix) const
{

  const TScalarType r = x.GetNorm();
  gmatrix.fill( NumericTraits< TScalarType >::Zero );
  const TScalarType r3 = r * r * r;
  for(unsigned int i=0; i<NDimensions; i++)
    {
    gmatrix[i][i] = r3;
    }
}


template <class TScalarType, unsigned int NDimensions>
void
VolumeSplineKernelTransform<TScalarType, NDimensions>::
ComputeDeformationContribution( const InputPointType  & thisPoint,
                                OutputPointType & result     ) const
{

  unsigned long numberOfLandmarks = 
                              this->m_SourceLandmarks->GetNumberOfPoints();

  PointsIterator sp  = this->m_SourceLandmarks->GetPoints()->Begin();

  for(unsigned int lnd=0; lnd < numberOfLandmarks; lnd++ )
    {
    InputVectorType position = thisPoint - sp->Value();
    const TScalarType r = position.GetNorm();
    const TScalarType r3 = r * r * r;

    for(unsigned int odim=0; odim < NDimensions; odim++ )
      {
      result[ odim ] += r3 * this->m_DMatrix(odim,lnd);
      }
    ++sp;
    }

}


} // namespace itk
#endif
