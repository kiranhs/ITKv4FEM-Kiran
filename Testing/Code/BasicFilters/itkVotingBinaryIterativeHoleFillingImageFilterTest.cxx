/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkVotingBinaryIterativeHoleFillingImageFilterTest.cxx
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
#include "itkRandomImageSource.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkVotingBinaryIterativeHoleFillingImageFilter.h"
#include "itkTextOutput.h"
#include "itkImageRegionIterator.h"


int itkVotingBinaryIterativeHoleFillingImageFilterTest(int, char* [] )
{
  // Comment the following if you want to use the itk text output window
  itk::OutputWindow::SetInstance(itk::TextOutput::New());


  typedef itk::Image<unsigned short,2> ImageType;

  itk::RandomImageSource<ImageType>::Pointer random;
  random = itk::RandomImageSource<ImageType>::New();
  random->SetMin(   0 );
  random->SetMax( 100 );

  ImageType::SizeValueType randomSize[2];
  randomSize[0] = randomSize[1] = 8;
  random->SetSize(randomSize);
  
  ImageType::SpacingValueType spacing[2] = {0.7, 2.1};
  random->SetSpacing( spacing );

  ImageType::PointValueType origin[2] = {15, 400};
  random->SetOrigin( origin );
    
  ImageType::PixelType foreground =  97; // prime numbers are good testers
  ImageType::PixelType background =  29;

  itk::BinaryThresholdImageFilter<ImageType,ImageType>::Pointer thresholder;
  thresholder =  itk::BinaryThresholdImageFilter<ImageType,ImageType>::New();
  thresholder->SetInput( random->GetOutput() );
  thresholder->SetLowerThreshold(  30 );
  thresholder->SetUpperThreshold( 100 );
  thresholder->SetInsideValue( foreground );
  thresholder->SetOutsideValue( background );

  // Create a voting image
  itk::VotingBinaryIterativeHoleFillingImageFilter<ImageType>::Pointer voting;
  voting = itk::VotingBinaryIterativeHoleFillingImageFilter<ImageType>::New();
  voting->SetInput( thresholder->GetOutput());
  voting->SetForegroundValue( foreground );
  voting->SetBackgroundValue( background );

  // define the neighborhood size used for the voting filter (5x5)
  ImageType::SizeType neighRadius;
  neighRadius[0] = 1;
  neighRadius[1] = 1;
  voting->SetRadius(neighRadius);

  // Set the maximum number of times the filter should perform passes filling
  // the border of holes and cavities.
  voting->SetMaximumNumberOfIterations( 10 );


  // Set the number of pixels over 50% that will tip the decision about
  // switching a pixel.
  voting->SetMajorityThreshold( 1 );
  
  // run the algorithm
  voting->Update();

  itk::ImageRegionIterator<ImageType> it;
  it = itk::ImageRegionIterator<ImageType>(random->GetOutput(),
                               random->GetOutput()->GetBufferedRegion());
  std::cout << "Input image" << std::endl;
  unsigned int i;
  for (i=1; !it.IsAtEnd(); ++i, ++it)
    {
    std::cout << "\t" << it.Get();
    if ((i % 8) == 0)
      {
      std::cout << std::endl;
      }
    }

  it = itk::ImageRegionIterator<ImageType>(thresholder->GetOutput(),
                               thresholder->GetOutput()->GetBufferedRegion());
  std::cout << "Binary image" << std::endl;
  
  for (i=1; !it.IsAtEnd(); ++i, ++it)
    {
    std::cout << "\t" << it.Get();
    if ((i % 8) == 0)
      {
      std::cout << std::endl;
      }
    }


  std::cout << "Output image" << std::endl;
  it = itk::ImageRegionIterator<ImageType>(voting->GetOutput(),
                               voting->GetOutput()->GetBufferedRegion());
  for (i=1; !it.IsAtEnd(); ++i, ++it)
    {
    std::cout << "\t" << it.Get();
    if ((i % 8) == 0)
      {
      std::cout << std::endl;
      }
    }
  
  std::cout << "Number Of Pixels Changed = " << voting->GetNumberOfPixelsChanged() << std::endl;

  voting->Print( std::cout );
  return EXIT_SUCCESS;
}



