/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkIterativeSupervisedTrainingFunction.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkIterativeSupervisedTrainingFunction_txx
#define __itkIterativeSupervisedTrainingFunction_txx

#include "itkIterativeSupervisedTrainingFunction.h"
#include <fstream>

namespace itk
{
namespace Statistics
{

template<class TSample, class TTargetVector, class ScalarType>
IterativeSupervisedTrainingFunction<TSample,TTargetVector,ScalarType>
::IterativeSupervisedTrainingFunction()
{
  this->m_LearningRate = 0.5;
  m_Threshold = 0;
  m_Stop = false;
}

template<class TSample, class TTargetVector, class ScalarType>
void IterativeSupervisedTrainingFunction<TSample,TTargetVector,ScalarType>
::SetNumOfIterations(long i)
{
  this->SetIterations(i);
  this->Modified();
}

template<class TSample, class TTargetVector, class ScalarType>
void IterativeSupervisedTrainingFunction<TSample,TTargetVector,ScalarType>
::Train(typename IterativeSupervisedTrainingFunction<TSample, TTargetVector, ScalarType>::NetworkType* Net,
        TSample* samples, TTargetVector* targets)
{
  this->SetTrainingSamples(samples);
  this->SetTargetValues(targets);

  InternalVectorType outputvector;
  InternalVectorType errorvector;
  outputvector.SetSize(targets->GetMeasurementVectorSize());
  errorvector.SetSize(targets->GetMeasurementVectorSize());

  //typename Superclass::OutputVectorType outputvector;
  typename Superclass::VectorType inputvector;
  typename Superclass::OutputVectorType targetvector;
  //typename Superclass::OutputVectorType errorvector;

#ifdef __OLD_CODE__
  std::ofstream outfile;
  outfile.open("output.txt");
#endif

  const long num_iterations = this->GetIterations();
  m_Stop = false;
  long i = 0;
  while (!m_Stop)
    {
    int temp = rand() % (this->m_InputSamples.size());
    inputvector = this->m_InputSamples[temp];
    targetvector = this->m_Targets[temp];
    outputvector = Net->GenerateOutput(inputvector);
    for(unsigned int k=0; k<targetvector.Size(); k++)
      {
      errorvector[k] = targetvector[k] - outputvector[k];
      }
#ifdef __OLD_CODE__
    outfile <<errorvector[0] << std::endl;
#endif
    Net->BackwardPropagate(this->m_PerformanceFunction->EvaluateDerivative(errorvector));
    Net->UpdateWeights(this->m_LearningRate);
    i++;
    if (i > num_iterations)
      {
      m_Stop = true;
      }
    }
#ifdef __OLD_CODE__
  if (this->m_PerformanceFunction->Evaluate(errorvector) < m_Threshold
   && i < num_iterations)
    {
    std::cout << "Goal Met " << std::endl;
    }
  else
    {
    std::cout << "Goal Not Met Max Iterations Reached " << std::endl;
    }
#endif
}

/** Print the object */
template<class TSample, class TTargetVector, class ScalarType>
void
IterativeSupervisedTrainingFunction<TSample,TTargetVector,ScalarType>
::PrintSelf( std::ostream& os, Indent indent ) const
{
  os << indent << "IterativeSupervisedTrainingFunction(" << this << ")" << std::endl;
  os << indent << "m_Threshold = " << m_Threshold << std::endl;
  os << indent << "m_Stop = " << m_Stop << std::endl;
  Superclass::PrintSelf( os, indent );
}

} // end namespace Statistics
} // end namespace itk

#endif
