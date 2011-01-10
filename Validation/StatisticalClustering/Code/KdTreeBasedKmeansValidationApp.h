/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    KdTreeBasedKmeansValidationApp.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __KdTreeBasedKmeansValidationApp_h
#define __KdTreeBasedKmeansValidationApp_h

#include "StatisticalClusteringValidationAppBase.h"
#include "KdTreeBasedKmeansClusteringMethod.h"

template< class TPixel, unsigned int VMeasurementVectorSize >
class KdTreeBasedKmeansValidationApp : 
  public StatisticalClusteringValidationAppBase< TPixel, VMeasurementVectorSize >
{
public:
  KdTreeBasedKmeansValidationApp() ;
  ~KdTreeBasedKmeansValidationApp() ;

  typedef StatisticalClusteringValidationAppBase< TPixel, VMeasurementVectorSize > Superclass ;
  typedef typename Superclass::ParametersType ParametersType ;
  typedef typename Superclass::SampleGeneratorType SampleGeneratorType ;
  
  typedef KdTreeBasedKmeansClusteringMethod< typename SampleGeneratorType::KdTreeType >
    ClusteringMethodType ;

protected:
  void GenerateSample() ;
  void PrepareInputParameterFilter() ;
  void PrepareOutputParameterTableHeader() ;
  void StartClustering(ParametersType& params) ;
  void MapClusterToClass() ;
  void PutResult(unsigned int caseNo) ;

  ClusteringMethodType m_ClusteringMethod ;
} ; // end of class

#ifndef ITK_MANUAL_INSTANTIATION
#include "KdTreeBasedKmeansValidationApp.txx"
#endif

#endif
