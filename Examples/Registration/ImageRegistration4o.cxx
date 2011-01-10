/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ImageRegistration4o.cxx
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

// Software Guide : BeginLatex
//
// In this example, we will solve a simple multi-modality problem using
// another implementation of mutual information. One of the main differences
// between \doxygen{MattesMutualInformationImageToImageMetric} and
// \doxygen{MutualInformationImageToImageMetric} is that only one spatial
// sample set is used for the whole registration process instead of using new
// samples every iteration. The use of a single sample set results in a much
// smoother cost function and hence allows the use of more intelligent
// optimizers. In this example, we will use the 
// RegularStepGradientDescentOptimizer.  Another noticeable
// difference is that pre-normalization of the images is not necessary as the
// metric rescales internally when building up the discrete density
// functions.  Other differences between the two mutual information
// implementations are described in detail in Section
// \ref{sec:MutualInformationMetric}. 
//
// First, we include the header files of the components used in this example.
//
// \index{itk::ImageRegistrationMethod!Multi-Modality}
// Software Guide : EndLatex 


// Software Guide : BeginCodeSnippet
#include "itkImageRegistrationMethod.h"
#include "itkTranslationTransform.h"
#include "itkMattesMutualInformationImageToImageMetric.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkRegularStepGradientDescentOptimizer.h"
#include "itkImage.h"
// Software Guide : EndCodeSnippet


#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include "itkResampleImageFilter.h"
#include "itkCastImageFilter.h"


//  The following section of code implements a Command observer
//  used to monitor the evolution of the registration process.
//
#include "itkCommand.h"
class CommandIterationUpdate : public itk::Command 
{
public:
  typedef  CommandIterationUpdate   Self;
  typedef  itk::Command             Superclass;
  typedef itk::SmartPointer<Self>   Pointer;
  itkNewMacro( Self );
protected:
  CommandIterationUpdate() {};
public:
  typedef itk::RegularStepGradientDescentOptimizer OptimizerType;
  typedef   const OptimizerType *                  OptimizerPointer;

  void Execute(itk::Object *caller, const itk::EventObject & event)
    {
    Execute( (const itk::Object *)caller, event);
    }

  void Execute(const itk::Object * object, const itk::EventObject & event)
    {
    OptimizerPointer optimizer = 
      dynamic_cast< OptimizerPointer >( object );
    if( ! itk::IterationEvent().CheckEvent( &event ) )
      {
      return;
      }
    std::cout << optimizer->GetCurrentIteration() << "   ";
    std::cout << optimizer->GetValue() << "   ";
    std::cout << optimizer->GetCurrentPosition() << std::endl;
    }
};


int main( int argc, char *argv[] )
{
  if( argc < 4 )
    {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " fixedImageFile  movingImageFile ";
    std::cerr << "outputImagefile [defaultPixelValue]" << std::endl;
    return EXIT_FAILURE;
    }
  
  const    unsigned int    Dimension = 2;
  typedef  unsigned short  PixelType;
  
  typedef itk::Image< PixelType, Dimension >  FixedImageType;
  typedef itk::Image< PixelType, Dimension >  MovingImageType;

  typedef itk::TranslationTransform< double, Dimension > TransformType;
  typedef itk::RegularStepGradientDescentOptimizer       OptimizerType;
  typedef itk::LinearInterpolateImageFunction< 
                                    MovingImageType,
                                    double             > InterpolatorType;
  typedef itk::ImageRegistrationMethod< 
                                    FixedImageType, 
                                    MovingImageType    > RegistrationType;

  //  Software Guide : BeginLatex
  //  
  //  In this example the image types and all registration components,
  //  except the metric, are declared as in Section 
  //  \ref{sec:IntroductionImageRegistration}.
  //  The Mattes mutual information metric type is 
  //  instantiated using the image types.
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  typedef itk::MattesMutualInformationImageToImageMetric< 
                                          FixedImageType, 
                                          MovingImageType >    MetricType;
  // Software Guide : EndCodeSnippet

  TransformType::Pointer      transform     = TransformType::New();
  OptimizerType::Pointer      optimizer     = OptimizerType::New();
  InterpolatorType::Pointer   interpolator  = InterpolatorType::New();
  RegistrationType::Pointer   registration  = RegistrationType::New();

  registration->SetOptimizer(     optimizer     );
  registration->SetTransform(     transform     );
  registration->SetInterpolator(  interpolator  );
  

  //  Software Guide : BeginLatex
  //  
  //  The metric is created using the \code{New()} method and then
  //  connected to the registration object.
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  MetricType::Pointer metric = MetricType::New();
  registration->SetMetric( metric  );
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //  
  //  The metric requires two parameters to be selected: the number
  //  of bins used to compute the entropy and the number of spatial samples
  //  used to compute the density estimates. In typical application, 50 
  //  histogram bins are sufficient and the metric is relatively insensitive
  //  to changes in the number of bins. The number of spatial samples
  //  to be used depends on the content of the image. If the images are
  //  smooth and do not contain much detail, then using approximately
  //  $1$ percent of the pixels will do. On the other hand, if the images
  //  are detailed, it may be necessary to use a much higher proportion,
  //  such as $20$ percent.
  //
  //  \index{itk::Mattes\-Mutual\-Information\-Image\-To\-Image\-Metric!SetNumberOfHistogramBins()}
  //  \index{itk::Mattes\-Mutual\-Information\-Image\-To\-Image\-Metric!SetNumberOfSpatialSamples()}
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  metric->SetNumberOfHistogramBins( 24 );
  metric->SetNumberOfSpatialSamples( 10000 );
  // Software Guide : EndCodeSnippet

  typedef itk::ImageFileReader< FixedImageType  > FixedImageReaderType;
  typedef itk::ImageFileReader< MovingImageType > MovingImageReaderType;

  FixedImageReaderType::Pointer  fixedImageReader  = FixedImageReaderType::New();
  MovingImageReaderType::Pointer movingImageReader = MovingImageReaderType::New();

  fixedImageReader->SetFileName(  argv[1] );
  movingImageReader->SetFileName( argv[2] );

  registration->SetFixedImage(    fixedImageReader->GetOutput()    );
  registration->SetMovingImage(   movingImageReader->GetOutput()   );

  fixedImageReader->Update();

  registration->SetFixedImageRegion( 
       fixedImageReader->GetOutput()->GetBufferedRegion() );


  typedef RegistrationType::ParametersType ParametersType;
  ParametersType initialParameters( transform->GetNumberOfParameters() );

  initialParameters[0] = 0.0;  // Initial offset in mm along X
  initialParameters[1] = 0.0;  // Initial offset in mm along Y
  
  registration->SetInitialTransformParameters( initialParameters );


  //  Software Guide : BeginLatex
  //  
  //  Another significant difference in the metric is that it
  //  computes the negative mutual information and hence we
  //  need to minimize the cost function in this case. In this
  //  example we will use the same optimization parameters as in
  //  Section \ref{sec:IntroductionImageRegistration}.
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  optimizer->MinimizeOn();
  optimizer->SetMaximumStepLength( 2.00 );  
  optimizer->SetMinimumStepLength( 0.001 );
  optimizer->SetNumberOfIterations( 200 );
  optimizer->SetRelaxationFactor( 0.8 );
  // Software Guide : EndCodeSnippet


  // Create the Command observer and register it with the optimizer.
  //
  CommandIterationUpdate::Pointer observer = CommandIterationUpdate::New();
  optimizer->AddObserver( itk::IterationEvent(), observer );


  try 
    { 
    registration->StartRegistration(); 
    std::cout << "Optimizer stop condition: "
              << registration->GetOptimizer()->GetStopConditionDescription()
              << std::endl;
    } 
  catch( itk::ExceptionObject & err ) 
    { 
    std::cerr << "ExceptionObject caught !" << std::endl; 
    std::cerr << err << std::endl; 
    return EXIT_FAILURE;
    } 

  ParametersType finalParameters = registration->GetLastTransformParameters();
  
  double TranslationAlongX = finalParameters[0];
  double TranslationAlongY = finalParameters[1];
  
  // For stability reasons it may be desirable to round up the values of translation
  // 
  unsigned int numberOfIterations = optimizer->GetCurrentIteration();
  
  double bestValue = optimizer->GetValue();


  // Print out results
  //
  std::cout << "Result = " << std::endl;
  std::cout << " Translation X = " << TranslationAlongX  << std::endl;
  std::cout << " Translation Y = " << TranslationAlongY  << std::endl;
  std::cout << " Iterations    = " << numberOfIterations << std::endl;
  std::cout << " Metric value  = " << bestValue          << std::endl;
  std::cout << " Stop Condition  = " << optimizer->GetStopCondition() << std::endl;


  //  Software Guide : BeginLatex
  //  
  //  This example is executed using the same multi-modality images as
  //  in the previous one.  The registration converges after $24$ iterations and produces
  //  the following results:
  //
  //  \begin{verbatim}
  //  Translation X = 13.1719
  //  Translation Y = 16.9006
  //  \end{verbatim}
  //  These values are a very close match to 
  //  the true misaligment introduced in the moving image.
  //
  //  Software Guide : EndLatex 


  typedef itk::ResampleImageFilter< 
                            MovingImageType, 
                            FixedImageType >    ResampleFilterType;

  TransformType::Pointer finalTransform = TransformType::New();

  finalTransform->SetParameters( finalParameters );
  finalTransform->SetFixedParameters( transform->GetFixedParameters() );

  ResampleFilterType::Pointer resample = ResampleFilterType::New();

  resample->SetTransform( finalTransform );
  resample->SetInput( movingImageReader->GetOutput() );

  FixedImageType::Pointer fixedImage = fixedImageReader->GetOutput();

  PixelType defaultPixelValue = 100;

  if( argc > 4 )
    {
    defaultPixelValue = atoi( argv[4] );
    }
  
  resample->SetSize(    fixedImage->GetLargestPossibleRegion().GetSize() );
  resample->SetOutputOrigin(  fixedImage->GetOrigin() );
  resample->SetOutputSpacing( fixedImage->GetSpacing() );
  resample->SetOutputDirection( fixedImage->GetDirection() );
  resample->SetDefaultPixelValue( defaultPixelValue );


  typedef  unsigned char                                   OutputPixelType;
  typedef itk::Image< OutputPixelType, Dimension > OutputImageType;
  typedef itk::CastImageFilter< 
                        FixedImageType,
                        OutputImageType >                  CastFilterType;
  typedef itk::ImageFileWriter< OutputImageType >          WriterType;

  WriterType::Pointer      writer =  WriterType::New();
  CastFilterType::Pointer  caster =  CastFilterType::New();

  writer->SetFileName( argv[3] );

  caster->SetInput( resample->GetOutput() );
  writer->SetInput( caster->GetOutput()   );
  writer->Update();


  //  Software Guide : BeginLatex
  // 
  // \begin{figure}
  // \center
  // \includegraphics[width=0.32\textwidth]{ImageRegistration4Output.eps}
  // \includegraphics[width=0.32\textwidth]{ImageRegistration4CheckerboardBefore.eps}
  // \includegraphics[width=0.32\textwidth]{ImageRegistration4CheckerboardAfter.eps}
  // \itkcaption[MattesMutualInformationImageToImageMetric output images]{The mapped
  // moving image (left) and the composition of fixed and moving images before
  // (center) and after (right) registration with Mattes mutual information.}
  // \label{fig:ImageRegistration4Output}
  // \end{figure}
  //
  //  The result of resampling the moving image is presented at the top
  //  of Figure \ref{fig:ImageRegistration4Output}. The center and right
  //  parts of the figure present a checkerboard composite of the fixed and
  //  moving images before and after registration.
  //
  //  Software Guide : EndLatex 

  //  Software Guide : BeginLatex
  //  
  // \begin{figure}
  // \center
  // \includegraphics[height=0.44\textwidth]{ImageRegistration4TraceTranslations.eps}
  // \includegraphics[height=0.44\textwidth]{ImageRegistration4TraceMetric.eps}
  // \itkcaption[MattesMutualInformationImageToImageMetric output plots]{Sequence
  // of translations and metric values at each iteration of the optimizer.}
  // \label{fig:ImageRegistration4TraceTranslations}
  // \end{figure}
  //
  //  Figure \ref{fig:ImageRegistration4TraceTranslations} (top) shows the
  //  sequence of translations followed by the optimizer as it searched the
  //  parameter space. The bottom of the same figure shows the sequence
  //  of metric values computed as the optimizer searched the parameter
  //  space.  Comparing these trace plots with Figures
  //  \ref{fig:ImageRegistration2TraceTranslations} and
  //  \ref{fig:ImageRegistration2TraceMetric}, we can see that the measures
  //  produced by MattesMutualInformationImageToImageMetric are
  //  smoother than those of
  //  the MutualInformationImageToImageMetric. This smoothness allows
  //  the use of more sophisticated optimizers such as the
  //  \doxygen{RegularStepGradientDescentOptimizer} which efficiently locks
  //  onto the optimal value.
  //
  //
  //  Software Guide : EndLatex 

  return EXIT_SUCCESS;
}
