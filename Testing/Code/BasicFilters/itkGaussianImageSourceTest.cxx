/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkGaussianImageSourceTest.cxx
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

#include "itkImage.h"
#include "itkFilterWatcher.h"
#include "itkGaussianImageSource.h"

int itkGaussianImageSourceTest(int, char* [] )
{
  // This can be changed!
  const unsigned int dim = 3;

  // Image typedef
  typedef itk::Image< unsigned char, dim > ImageType;

  // Create a gaussian image source
  typedef itk::GaussianImageSource< ImageType > GaussianSourceType;
  GaussianSourceType::Pointer pSource = GaussianSourceType::New();
  FilterWatcher watcher(pSource, "pSource");

  ImageType::SpacingValueType   spacing[] = { 1.2f, 1.3f, 1.4f };
  ImageType::PointValueType     origin[] = { 1.0f, 4.0f, 2.0f };
  ImageType::SizeValueType      size[] = {  130,  150,  120 };

  GaussianSourceType::ArrayType mean;
  mean[0] = size[0]/2.0f + origin[0];
  mean[1] = size[1]/2.0f + origin[1];
  mean[2] = size[2]/2.0f + origin[2];
  
  GaussianSourceType::ArrayType sigma;
  sigma[0] = 25.0f;
  sigma[1] = 35.0f;
  sigma[2] = 55.0f;
  
  pSource->SetSize( size );
  pSource->SetOrigin( origin );
  pSource->SetSpacing( spacing );
  pSource->SetMean( mean );
  pSource->SetSigma( sigma );
  
  // Test the get macros as well (booorrring...)
  pSource->GetSize();
  pSource->GetSpacing();
  pSource->GetOrigin();
  pSource->GetDirection();
  pSource->GetScale();
  pSource->GetNormalized();
  pSource->GetSigma();
  pSource->GetMean();

  // Get the output of the source
  ImageType::Pointer pImage = pSource->GetOutput();
  
  // Run the pipeline
  pSource->Update();

  std::cout << pImage << std::endl;

  return EXIT_SUCCESS;
}


