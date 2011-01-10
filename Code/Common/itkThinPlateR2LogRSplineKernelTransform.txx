/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkThinPlateR2LogRSplineKernelTransform.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkThinPlateR2LogRSplineKernelTransform_txx
#define __itkThinPlateR2LogRSplineKernelTransform_txx
#include "itkThinPlateR2LogRSplineKernelTransform.h"

namespace itk
{

template <class TScalarType, unsigned int NDimensions>
void
ThinPlateR2LogRSplineKernelTransform<TScalarType, NDimensions>::
ComputeG(const InputVectorType & x, GMatrixType & gmatrix) const
{

  const TScalarType r = x.GetNorm();
  gmatrix.fill( NumericTraits< TScalarType >::Zero );
  const TScalarType R2logR = 
    ( r > 1e-8 )? r * r * vcl_log(r ) : NumericTraits<TScalarType>::Zero;

  gmatrix.fill_diagonal( R2logR );
}


template <class TScalarType, unsigned int NDimensions>
void
ThinPlateR2LogRSplineKernelTransform<TScalarType, NDimensions>::
ComputeDeformationContribution( const InputPointType  & thisPoint,
                                OutputPointType & result     ) const
{

  unsigned long numberOfLandmarks = this->m_SourceLandmarks->GetNumberOfPoints();

  PointsIterator sp  = this->m_SourceLandmarks->GetPoints()->Begin();

  for(unsigned int lnd=0; lnd < numberOfLandmarks; lnd++ )
    {
    InputVectorType position = thisPoint - sp->Value();
    const TScalarType r = position.GetNorm();
    const TScalarType R2logR = 
      ( r > 1e-8 )? r * r * vcl_log(r ) : NumericTraits<TScalarType>::Zero;
    for(unsigned int odim=0; odim < NDimensions; odim++ )
      {
      result[ odim ] += R2logR * this->m_DMatrix(odim,lnd);
      }
    ++sp;
    }

}


} // namespace itk
#endif
