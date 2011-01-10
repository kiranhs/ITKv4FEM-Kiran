/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ImageRegistration1o.cxx
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
// This example illustrates the use of the image registration framework in
// Insight.  It should be read as a "Hello World" for ITK
// registration. Which means that for now you don't ask "why?". Instead,
// use the example as an introduction to the typical elements involved in
// solving an image registration problem.
//
// \index{itk::Image!Instantiation}
// \index{itk::Image!Header}
//
// A registration method requires the following set of components: two input
// images, a transform, a metric, an interpolator and an optimizer. Some of
// these components are parametrized by the image type for which the
// registration is intended.  The following header files provide declarations
// for common types of these components.
//
// Software Guide : EndLatex 


// Software Guide : BeginCodeSnippet
#include "itkImageRegistrationMethod.h"
#include "itkTranslationTransform.h"
#include "itkMeanSquaresImageToImageMetric.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkRegularStepGradientDescentOptimizer.h"
#include "itkImage.h"
// Software Guide : EndCodeSnippet


#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include "itkResampleImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkSquaredDifferenceImageFilter.h"


int main( int argc, char *argv[] )
{
  if( argc < 4 )
    {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " fixedImageFile  movingImageFile ";
    std::cerr << "outputImagefile [differenceImage]" << std::endl;
    return EXIT_FAILURE;
    }
  
  // Software Guide : BeginLatex
  // 
  // The types of each one of the components in the registration methods should
  // be instantiated. First, we select the image dimension and the type for
  // representing image pixels.
  //
  // Software Guide : EndLatex 
  //
  // Software Guide : BeginCodeSnippet 
  const    unsigned int    Dimension = 2;
  typedef  float           PixelType;
  // Software Guide : EndCodeSnippet

  
  //  Software Guide : BeginLatex
  //  
  //  The types of the input images are instantiated by the following lines.
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  typedef itk::Image< PixelType, Dimension >  FixedImageType;
  typedef itk::Image< PixelType, Dimension >  MovingImageType;
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //  
  //  The transform that will map one image space into the other is defined
  //  below.
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  typedef itk::TranslationTransform< double, Dimension > TransformType;
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //  
  //  An optimizer is required to explore the parameter space of the transform
  //  in search of optimal values of the metric.
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  typedef itk::RegularStepGradientDescentOptimizer       OptimizerType;
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //  
  //  The metric will compare how well the two images match each other. Metric
  //  types are usually parametrized by the image types as can be seen in the
  //  following type declaration.
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  typedef itk::MeanSquaresImageToImageMetric< 
                                    FixedImageType, 
                                    MovingImageType >    MetricType;
  // Software Guide : EndCodeSnippet



  //  Software Guide : BeginLatex
  //  
  //  Finally, the type of the interpolator is declared. The interpolator will
  //  evaluate the moving image at non-grid positions.
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  typedef itk:: LinearInterpolateImageFunction< 
                                    MovingImageType,
                                    double          >    InterpolatorType;
  // Software Guide : EndCodeSnippet 


  //  Software Guide : BeginLatex
  //
  //  The registration method type is instantiated using the types of the
  //  fixed and moving images. This class is responsible for interconnecting
  //  all the components we have described so far.
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  typedef itk::ImageRegistrationMethod< 
                                    FixedImageType, 
                                    MovingImageType >    RegistrationType;
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //
  //  Each one of the registration components is created using its
  //  \code{New()} method and is assigned to its respective 
  //  \doxygen{SmartPointer}.
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  MetricType::Pointer         metric        = MetricType::New();
  TransformType::Pointer      transform     = TransformType::New();
  OptimizerType::Pointer      optimizer     = OptimizerType::New();
  InterpolatorType::Pointer   interpolator  = InterpolatorType::New();
  RegistrationType::Pointer   registration  = RegistrationType::New();
  // Software Guide : EndCodeSnippet
  

  //  Software Guide : BeginLatex
  //
  //  Each component is now connected to the instance of the registration method.
  //  \index{itk::RegistrationMethod!SetMetric()}
  //  \index{itk::RegistrationMethod!SetOptimizer()}
  //  \index{itk::RegistrationMethod!SetTransform()}
  //  \index{itk::RegistrationMethod!SetFixedImage()}
  //  \index{itk::RegistrationMethod!SetMovingImage()}
  //  \index{itk::RegistrationMethod!SetInterpolator()}
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  registration->SetMetric(        metric        );
  registration->SetOptimizer(     optimizer     );
  registration->SetTransform(     transform     );
  registration->SetInterpolator(  interpolator  );
  // Software Guide : EndCodeSnippet


  typedef itk::ImageFileReader< FixedImageType  > FixedImageReaderType;
  typedef itk::ImageFileReader< MovingImageType > MovingImageReaderType;
  FixedImageReaderType::Pointer  fixedImageReader  = FixedImageReaderType::New();
  MovingImageReaderType::Pointer movingImageReader = MovingImageReaderType::New();

  fixedImageReader->SetFileName(  argv[1] );
  movingImageReader->SetFileName( argv[2] );


  //  Software Guide : BeginLatex
  //  
  //  In this example, the fixed and moving images are read from files. This
  //  requires the \doxygen{ImageRegistrationMethod} to acquire its inputs to
  //  the output of the readers.
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  registration->SetFixedImage(    fixedImageReader->GetOutput()    );
  registration->SetMovingImage(   movingImageReader->GetOutput()   );
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //  
  //  The registration can be restricted to consider only a particular region
  //  of the fixed image as input to the metric computation. This region is
  //  defined by the \code{SetFixedImageRegion()} method.  You could use this
  //  feature to reduce the computational time of the registration or to avoid
  //  unwanted objects present in the image affecting the registration outcome.
  //  In this example we use the full available content of the image. This
  //  region is identified by the \code{BufferedRegion} of the fixed image.
  //  Note that for this region to be valid the reader must first invoke its
  //  \code{Update()} method.
  //
  //  \index{itk::ImageRegistrationMethod!SetFixedImageRegion()}
  //  \index{itk::Image!GetBufferedRegion()}
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  fixedImageReader->Update();
  registration->SetFixedImageRegion( 
     fixedImageReader->GetOutput()->GetBufferedRegion() );
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //
  //  The parameters of the transform are initialized by passing them in an
  //  array. This can be used to setup an initial known correction of the
  //  misalignment. In this particular case, a translation transform is
  //  being used for the registration. The array of parameters for this
  //  transform is simply composed of the translation values along each
  //  dimension. Setting the values of the parameters to zero 
  //  initializes the transform as an \emph{identity} transform. Note that the
  //  array constructor requires the number of elements as an argument.
  //
  //  \index{itk::TranslationTransform!GetNumberOfParameters()}
  //  \index{itk::RegistrationMethod!SetInitialTransformParameters()}
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  typedef RegistrationType::ParametersType ParametersType;
  ParametersType initialParameters( transform->GetNumberOfParameters() );

  initialParameters[0] = 0.0;  // Initial offset in mm along X
  initialParameters[1] = 0.0;  // Initial offset in mm along Y
  
  registration->SetInitialTransformParameters( initialParameters );
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //
  //  At this point the registration method is ready for execution. The
  //  optimizer is the component that drives the execution of the
  //  registration.  However, the ImageRegistrationMethod class
  //  orchestrates the ensemble to make sure that everything is in place
  //  before control is passed to the optimizer.
  //
  //  It is usually desirable to fine tune the parameters of the optimizer.
  //  Each optimizer has particular parameters that must be interpreted in the
  //  context of the optimization strategy it implements. The optimizer used in
  //  this example is a variant of gradient descent that attempts to prevent it
  //  from taking steps which are too large.  At each iteration, this optimizer
  //  will take a step along the direction of the \doxygen{ImageToImageMetric}
  //  derivative. The initial length of the step is defined by the user. Each
  //  time the direction of the derivative abruptly changes, the optimizer
  //  assumes that a local extrema has been passed and reacts by reducing the
  //  step length by a half. After several reductions of the step length, the
  //  optimizer may be moving in a very restricted area of the transform
  //  parameter space. The user can define how small the step length should be
  //  to consider convergence to have been reached. This is equivalent to defining
  //  the precision with which the final transform should be known.
  //
  //  The initial step length is defined with the method
  //  \code{SetMaximumStepLength()}, while the tolerance for convergence is
  //  defined with the method \code{SetMinimumStepLength()}.
  //
  //  \index{itk::Regular\-Setp\-Gradient\-Descent\-Optimizer!SetMaximumStepLength()}
  //  \index{itk::Regular\-Step\-Gradient\-Descent\-Optimizer!SetMinimumStepLength()}
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  optimizer->SetMaximumStepLength( 4.00 );  
  optimizer->SetMinimumStepLength( 0.01 );
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //  
  //  In the case where the optimizer never succeeds in reaching the desired
  //  precision tolerance, it is prudent to establish a limit on the number of
  //  iterations to be performed. This maximum number is defined with the
  //  method \code{SetNumberOfIterations()}.
  //
  //  \index{itk::Regular\-Setp\-Gradient\-Descent\-Optimizer!SetNumberOfIterations()}
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  optimizer->SetNumberOfIterations( 200 );
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //  
  //  The registration process is triggered by an invocation of the
  //  \code{Update()} method. If something goes wrong during the
  //  initialization or execution of the registration an exception will be
  //  thrown. We should therefore place the \code{Update()} method
  //  in a \code{try/catch} block as illustrated in the following lines.
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  try 
    { 
    registration->Update(); 
    } 
  catch( itk::ExceptionObject & err ) 
    { 
    std::cout << "ExceptionObject caught !" << std::endl; 
    std::cout << err << std::endl; 
    return EXIT_FAILURE;
    } 
  // Software Guide : EndCodeSnippet

  
  //  Software Guide : BeginLatex
  //  
  // In a real application, you may attempt to recover from the error in the
  // catch block. Here we are simply printing out a message and then
  // terminating the execution of the program.
  //
  //  Software Guide : EndLatex 

  //  Software Guide : BeginLatex
  //  
  //  The result of the registration process is an array of parameters that
  //  defines the spatial transformation in an unique way. This final result is
  //  obtained using the \code{GetLastTransformParameters()} method.
  //
  //  \index{itk::RegistrationMethod!GetLastTransformParameters()}
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  ParametersType finalParameters = registration->GetLastTransformParameters();
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //  
  //  In the case of the \doxygen{TranslationTransform}, there is a
  //  straightforward interpretation of the parameters.  Each element of the
  //  array corresponds to a translation along one spatial dimension.
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  const double TranslationAlongX = finalParameters[0];
  const double TranslationAlongY = finalParameters[1];
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //  
  //  The optimizer can be queried for the actual number of iterations
  //  performed to reach convergence.  The \code{GetCurrentIteration()}
  //  method returns this value. A large number of iterations may be an
  //  indication that the maximum step length has been set too small, which
  //  is undesirable since it results in long computational times.
  //
  //  \index{itk::Regular\-Setp\-Gradient\-Descent\-Optimizer!GetCurrentIteration()}
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  const unsigned int numberOfIterations = optimizer->GetCurrentIteration();
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //  
  //  The value of the image metric corresponding to the last set of parameters
  //  can be obtained with the \code{GetValue()} method of the optimizer.
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  const double bestValue = optimizer->GetValue();
  // Software Guide : EndCodeSnippet


  // Print out results
  //
  std::cout << "Result = " << std::endl;
  std::cout << " Translation X = " << TranslationAlongX  << std::endl;
  std::cout << " Translation Y = " << TranslationAlongY  << std::endl;
  std::cout << " Iterations    = " << numberOfIterations << std::endl;
  std::cout << " Metric value  = " << bestValue          << std::endl;


  //  Software Guide : BeginLatex
  //  
  //  Let's execute this example over two of the images provided in
  //  \code{Examples/Data}:
  //  
  //  \begin{itemize}
  //  \item \code{BrainProtonDensitySliceBorder20.png} 
  //  \item \code{BrainProtonDensitySliceShifted13x17y.png}
  //  \end{itemize}
  //
  //  The second image is the result of intentionally translating the first
  //  image by $(13,17)$ millimeters. Both images have unit-spacing and
  //  are shown in Figure \ref{fig:FixedMovingImageRegistration1}. The
  //  registration takes 18 iterations and the resulting transform parameters are:
  //
  //  \begin{verbatim}
  //  Translation X = 12.9903
  //  Translation Y = 17.0001
  //  \end{verbatim}
  // 
  //  As expected, these values match the misalignment
  //  intentionally introduced in the moving image quite well.
  //
  // \begin{figure}
  // \center
  // \includegraphics[width=0.44\textwidth]{BrainProtonDensitySliceBorder20.eps}
  // \includegraphics[width=0.44\textwidth]{BrainProtonDensitySliceShifted13x17y.eps}
  // \itkcaption[Fixed and Moving images in registration framework]{Fixed and
  // Moving image provided as input to the registration method.}
  // \label{fig:FixedMovingImageRegistration1}
  // \end{figure}
  //
  //
  //  Software Guide : EndLatex 


  //  Software Guide : BeginLatex
  //  
  //  It is common, as the last step of a registration task, to use the
  //  resulting transform to map the moving image into the fixed image space.
  //  This is easily done with the \doxygen{ResampleImageFilter}. Please
  //  refer to Section~\ref{sec:ResampleImageFilter} for details on the use
  //  of this filter.  First, a ResampleImageFilter type is instantiated
  //  using the image types. It is convenient to use the fixed image type as
  //  the output type since it is likely that the transformed moving image
  //  will be compared with the fixed image.
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  typedef itk::ResampleImageFilter< 
                            MovingImageType, 
                            FixedImageType >    ResampleFilterType;
  // Software Guide : EndCodeSnippet



  //  Software Guide : BeginLatex
  //  
  //  A resampling filter is created and the corresponding transform and
  //  moving image connected as inputs.
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  ResampleFilterType::Pointer resample = ResampleFilterType::New();
  resample->SetInput( movingImageReader->GetOutput() );
  // Software Guide : EndCodeSnippet



  //  Software Guide : BeginLatex
  //  
  //  The Transform that is produced as output of the Registration method is
  //  now passed as input to the resampling filter.
  //
  //  \index{itk::ImageRegistrationMethod!Resampling image}
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  resample->SetTransform( registration->GetOutput()->Get() );
  // Software Guide : EndCodeSnippet



  //  Software Guide : BeginLatex
  //  
  //  As described in Section \ref{sec:ResampleImageFilter}, the
  //  ResampleImageFilter requires additional parameters to be
  //  specified, in particular, the spacing, origin and size of the output
  //  image. The default pixel value is also set to a distinct gray level in
  //  order to make the regions that are outside of the mapped image visible.  
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  FixedImageType::Pointer fixedImage = fixedImageReader->GetOutput();
  resample->SetSize( fixedImage->GetLargestPossibleRegion().GetSize() );
  resample->SetOutputOrigin(  fixedImage->GetOrigin() );
  resample->SetOutputSpacing( fixedImage->GetSpacing() );
  resample->SetOutputDirection( fixedImage->GetDirection() );
  resample->SetDefaultPixelValue( 100 );
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //  
  // \begin{figure}
  // \center
  // \includegraphics[width=0.32\textwidth]{ImageRegistration1Output.eps}
  // \includegraphics[width=0.32\textwidth]{ImageRegistration1DifferenceBefore.eps}
  // \includegraphics[width=0.32\textwidth]{ImageRegistration1DifferenceAfter.eps}
  // \itkcaption[HelloWorld registration output images]{Mapped moving image and its
  // difference with the fixed image before and after registration}
  // \label{fig:ImageRegistration1Output}
  // \end{figure}
  //
  //  Software Guide : EndLatex 


  //  Software Guide : BeginLatex
  //  
  //  The output of the filter is passed to a writer that will store the
  //  image in a file. An \doxygen{CastImageFilter} is used to convert the
  //  pixel type of the resampled image to the final type used by the
  //  writer. The cast and writer filters are instantiated below.
  //
  //  Software Guide : EndLatex 
  
  // Software Guide : BeginCodeSnippet
  typedef unsigned char  OutputPixelType;
  typedef itk::Image< OutputPixelType, Dimension > OutputImageType;
  typedef itk::CastImageFilter< 
                        FixedImageType,
                        OutputImageType > CastFilterType;
  typedef itk::ImageFileWriter< OutputImageType >  WriterType;
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //  
  //  The filters are created by invoking their \code{New()}
  //  method.
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  WriterType::Pointer      writer =  WriterType::New();
  CastFilterType::Pointer  caster =  CastFilterType::New();
  // Software Guide : EndCodeSnippet


  writer->SetFileName( argv[3] );


  //  Software Guide : BeginLatex
  //
  //  The \code{Update()} method of the writer is invoked in order to trigger
  //  the execution of the pipeline.
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  caster->SetInput( resample->GetOutput() );
  writer->SetInput( caster->GetOutput()   );
  writer->Update();
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //  
  // \begin{figure}
  // \center
  // \includegraphics[width=\textwidth]{ImageRegistration1Pipeline.eps}
  // \itkcaption[Pipeline structure of the registration example]{Pipeline
  // structure of the registration example.}
  // \label{fig:ImageRegistration1Pipeline}
  // \end{figure}
  //
  //
  //  Software Guide : EndLatex 


  //  Software Guide : BeginLatex
  //  
  //  The fixed image and the transformed moving image can easily be compared
  //  using the \code{SquaredDifferenceImageFilter}. This pixel-wise
  //  filter computes the squared value of the difference between homologous
  //  pixels of its input images.
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  typedef itk::SquaredDifferenceImageFilter< 
                                  FixedImageType, 
                                  FixedImageType, 
                                  OutputImageType > DifferenceFilterType;

  DifferenceFilterType::Pointer difference = DifferenceFilterType::New();
  difference->SetInput1( fixedImageReader->GetOutput() );
  difference->SetInput2( resample->GetOutput() );
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //  
  //  Its output can be passed to another writer.
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  WriterType::Pointer writer2 = WriterType::New();
  writer2->SetInput( difference->GetOutput() );  
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //  
  //  The complete pipeline structure of the current example is presented in
  //  Figure~\ref{fig:ImageRegistration1Pipeline}.  The components of the
  //  registration method are depicted as well.  Figure
  //  \ref{fig:ImageRegistration1Output} (left) shows the result of resampling
  //  the moving image in order to map it onto the fixed image space. The top
  //  and right borders of the image appear in the gray level selected with the
  //  \code{SetDefaultPixelValue()} in the ResampleImageFilter. The
  //  center image shows the squared difference between the fixed image and
  //  the moving image. The right image shows the squared difference between
  //  the fixed image and the transformed moving image.  Both difference images
  //  are displayed negated in order to accentuate those pixels where differences
  //  exist. 
  //
  //  Software Guide : EndLatex 


  if( argc >= 5 )
    {
    writer2->SetFileName( argv[4] );
    writer2->Update();
    }

  //  Software Guide : BeginLatex
  //  
  // \begin{figure}
  // \center
  // \includegraphics[height=0.44\textwidth]{ImageRegistration1TraceTranslations.eps}
  // \includegraphics[height=0.44\textwidth]{ImageRegistration1TraceMetric.eps}
  // \itkcaption[Trace of translations and metrics during registration]{The sequence
  // of translations and metric values at each iteration of the optimizer.}
  // \label{fig:ImageRegistration1Trace}
  // \end{figure}
  //
  //  It is always useful to keep in mind that registration is essentially an
  //  optimization problem. Figure \ref{fig:ImageRegistration1Trace} helps to
  //  reinforce this notion by showing the trace of translations and values
  //  of the image metric at each iteration of the optimizer. It can be seen
  //  from the top figure that the step length is progressively reduced as
  //  the optimizer gets closer to the metric extrema. The bottom plot
  //  clearly shows how the metric value decreases as the optimization
  //  advances. The log plot helps to hightlight the normal oscilations of
  //  the optimizer around the extrema value.
  //
  //  Software Guide : EndLatex 


  return EXIT_SUCCESS;
}

