/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    DeformableRegistration14.cxx
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
// This example illustrates the use of the \doxygen{RegularStepGradientDescentOptimizer}
// in the context of a deformable registration problem. The code of this example is almost
// identical to the one in Section~\ref{sec:DeformableRegistration8}.
//
// \index{itk::BSplineDeformableTransform}
// \index{itk::BSplineDeformableTransform!DeformableRegistration}
// \index{itk::RegularStepGradientDescentOptimizer}
//
//
// Software Guide : EndLatex 

#include "itkImageRegistrationMethod.h"
#include "itkMattesMutualInformationImageToImageMetric.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkImage.h"

#include "itkTimeProbesCollectorBase.h"

#ifdef ITK_USE_REVIEW
#include "itkMemoryProbesCollectorBase.h"
#define itkProbesCreate()  \
  itk::TimeProbesCollectorBase chronometer; \
  itk::MemoryProbesCollectorBase memorymeter
#define itkProbesStart( text ) memorymeter.Start( text ); chronometer.Start( text )
#define itkProbesStop( text )  chronometer.Stop( text ); memorymeter.Stop( text  )
#define itkProbesReport( stream )  chronometer.Report( stream ); memorymeter.Report( stream  )
#else
#define itkProbesCreate()  \
  itk::TimeProbesCollectorBase chronometer
#define itkProbesStart( text ) chronometer.Start( text )
#define itkProbesStop( text )  chronometer.Stop( text )
#define itkProbesReport( stream )  chronometer.Report( stream )
#endif


//  Software Guide : BeginLatex
//  
//  The following are the most relevant headers to this example.
//
//  \index{itk::BSplineDeformableTransform!header}
//  \index{itk::RegularStepGradientDescentOptimizer!header}
// 
//  Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet
#include "itkBSplineDeformableTransform.h"
#include "itkRegularStepGradientDescentOptimizer.h"
// Software Guide : EndCodeSnippet


#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include "itkResampleImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkSquaredDifferenceImageFilter.h"


#include "itkTransformFileReader.h"

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
    if( !(itk::IterationEvent().CheckEvent( &event )) )
      {
      return;
      }
    std::cout << optimizer->GetCurrentIteration() << "   ";
    std::cout << optimizer->GetValue() << "   ";
    std::cout << std::endl;
    }
};

int main( int argc, char *argv[] )
{
  if( argc < 4 )
    {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " fixedImageFile  movingImageFile outputImagefile  ";
    std::cerr << " [differenceOutputfile] [differenceBeforeRegistration] ";
    std::cerr << " [deformationField] ";
    std::cerr << " [useExplicitPDFderivatives ] [useCachingBSplineWeights ] ";
    std::cerr << " [filenameForFinalTransformParameters] ";
    std::cerr << " [maximumStepLength] [maximumNumberOfIterations]";
    std::cerr << std::endl;
    return EXIT_FAILURE;
    }
  
  const    unsigned int    ImageDimension = 3;
  typedef  signed short    PixelType;

  typedef itk::Image< PixelType, ImageDimension >  FixedImageType;
  typedef itk::Image< PixelType, ImageDimension >  MovingImageType;


  const unsigned int SpaceDimension = ImageDimension;
  const unsigned int SplineOrder = 3;
  typedef double CoordinateRepType;

  typedef itk::BSplineDeformableTransform<
                            CoordinateRepType,
                            SpaceDimension,
                            SplineOrder >     TransformType;

  typedef itk::RegularStepGradientDescentOptimizer       OptimizerType;


  typedef itk::MattesMutualInformationImageToImageMetric< 
                                    FixedImageType, 
                                    MovingImageType >    MetricType;

  typedef itk:: LinearInterpolateImageFunction< 
                                    MovingImageType,
                                    double          >    InterpolatorType;

  typedef itk::ImageRegistrationMethod< 
                                    FixedImageType, 
                                    MovingImageType >    RegistrationType;

  MetricType::Pointer         metric        = MetricType::New();
  OptimizerType::Pointer      optimizer     = OptimizerType::New();
  InterpolatorType::Pointer   interpolator  = InterpolatorType::New();
  RegistrationType::Pointer   registration  = RegistrationType::New();
  

  registration->SetMetric(        metric        );
  registration->SetOptimizer(     optimizer     );
  registration->SetInterpolator(  interpolator  );


  TransformType::Pointer  transform = TransformType::New();
  registration->SetTransform( transform );

  //
  //   In general, you must first solve an Affine registration between
  //   the images before attempting to solve a deformable registration.
  //   If you have solve an affine transform, it can be loaded into the
  //   BSplineDeformableTransform as a "bulk" transform that will be
  //   pre-composed with the deformation computed by the BSpline.
  //   The following code loads one of such initial transforms if they
  //   are available.
  //   
  typedef itk::TransformFileReader        TransformReaderType;
  typedef itk::AffineTransform<double, 3> AffineTransformType;

  TransformReaderType::Pointer transformReader = TransformReaderType::New();

  if( argc > 11 )
    {
    std::cout << "Loading Transform: " << argv[11] << std::endl;
    transformReader->SetFileName( argv[11] );
    transformReader->Update();
    
    typedef TransformReaderType::TransformListType * TransformListType;
    TransformListType transforms = transformReader->GetTransformList();
    TransformReaderType::TransformListType::const_iterator tit = transforms->begin();
    if( !strcmp((*tit)->GetNameOfClass(),"AffineTransform") )
      {
      AffineTransformType::Pointer affine_read = 
                  static_cast<AffineTransformType*>((*tit).GetPointer());
      AffineTransformType::Pointer affine_transform = 
                  dynamic_cast< AffineTransformType * >( affine_read.GetPointer() );
    
      if( affine_transform )
        {
        transform->SetBulkTransform( affine_transform );
        }
      } 
    else
      {
      std::cerr << "Bulk transform wasn't an affine transform." << std::endl;
      return EXIT_FAILURE;
      }
    }


  typedef itk::ImageFileReader< FixedImageType  > FixedImageReaderType;
  typedef itk::ImageFileReader< MovingImageType > MovingImageReaderType;

  FixedImageReaderType::Pointer  fixedImageReader  = FixedImageReaderType::New();
  MovingImageReaderType::Pointer movingImageReader = MovingImageReaderType::New();

  fixedImageReader->SetFileName(  argv[1] );
  movingImageReader->SetFileName( argv[2] );

  FixedImageType::ConstPointer fixedImage = fixedImageReader->GetOutput();

  registration->SetFixedImage(  fixedImage   );
  registration->SetMovingImage(   movingImageReader->GetOutput()   );

  fixedImageReader->Update();

  FixedImageType::RegionType fixedRegion = fixedImage->GetBufferedRegion();
  
  registration->SetFixedImageRegion( fixedRegion );

  unsigned int numberOfGridNodesInOneDimension = 5;

  if( argc > 10 )
    {
    numberOfGridNodesInOneDimension = atoi( argv[10] );
    }


  typedef TransformType::RegionType RegionType;
  RegionType bsplineRegion;
  RegionType::SizeType   gridSizeOnImage;
  RegionType::SizeType   gridBorderSize;
  RegionType::SizeType   totalGridSize;

  gridSizeOnImage.Fill( numberOfGridNodesInOneDimension );
  gridBorderSize.Fill( SplineOrder );    // Border for spline order = 3 ( 1 lower, 2 upper )
  totalGridSize = gridSizeOnImage + gridBorderSize;

  bsplineRegion.SetSize( totalGridSize );

  typedef TransformType::SpacingType SpacingType;
  SpacingType spacing = fixedImage->GetSpacing();

  typedef TransformType::OriginType OriginType;
  OriginType origin = fixedImage->GetOrigin();

  FixedImageType::SizeType fixedImageSize = fixedRegion.GetSize();

  for(unsigned int r=0; r<ImageDimension; r++)
    {
    spacing[r] *= static_cast<double>(fixedImageSize[r] - 1)  / 
                  static_cast<double>(gridSizeOnImage[r] - 1);
    }

  FixedImageType::DirectionType gridDirection = fixedImage->GetDirection();
  SpacingType gridOriginOffset = gridDirection * spacing;

  OriginType gridOrigin = origin - gridOriginOffset; 

  transform->SetGridSpacing( spacing );
  transform->SetGridOrigin( gridOrigin );
  transform->SetGridRegion( bsplineRegion );
  transform->SetGridDirection( gridDirection );
  

  typedef TransformType::ParametersType     ParametersType;

  const unsigned int numberOfParameters =
               transform->GetNumberOfParameters();
  
  ParametersType parameters( numberOfParameters );

  parameters.Fill( 0.0 );

  transform->SetParameters( parameters );

  registration->SetInitialTransformParameters( transform->GetParameters() );
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //  
  //  Next we set the parameters of the RegularStepGradientDescentOptimizer object. 
  //
  //  Software Guide : EndLatex 


  // Software Guide : BeginCodeSnippet
  optimizer->SetMaximumStepLength( 10.0 );
  optimizer->SetMinimumStepLength(  0.01 );

  optimizer->SetRelaxationFactor( 0.7 );
  optimizer->SetNumberOfIterations( 50 );
  // Software Guide : EndCodeSnippet


  // Optionally, get the step length from the command line arguments
  if( argc > 12 )
    {
    optimizer->SetMaximumStepLength( atof( argv[12] ) );
    }

  // Optionally, get the number of iterations from the command line arguments
  if( argc > 13 )
    {
    optimizer->SetNumberOfIterations( atoi( argv[13] ) );
    }


  // Create the Command observer and register it with the optimizer.
  //
  CommandIterationUpdate::Pointer observer = CommandIterationUpdate::New();
  optimizer->AddObserver( itk::IterationEvent(), observer );


  metric->SetNumberOfHistogramBins( 50 );
  
  const unsigned int numberOfSamples = 
    static_cast<unsigned int>( fixedRegion.GetNumberOfPixels() * 20.0 / 100.0 );

  metric->SetNumberOfSpatialSamples( numberOfSamples );
  metric->ReinitializeSeed( 76926294 );


  if( argc > 7 )
    {
    // Define whether to calculate the metric derivative by explicitly
    // computing the derivatives of the joint PDF with respect to the Transform
    // parameters, or doing it by progressively accumulating contributions from
    // each bin in the joint PDF.
    metric->SetUseExplicitPDFDerivatives( atoi( argv[7] ) );
    }

  if( argc > 8 )
    {
    // Define whether to cache the BSpline weights and indexes corresponding to
    // each one of the samples used to compute the metric. Enabling caching will
    // make the algorithm run faster but it will have a cost on the amount of memory
    // that needs to be allocated. This option is only relevant when using the 
    // BSplineDeformableTransform.
    metric->SetUseCachingOfBSplineWeights( atoi( argv[8] ) );
    }


  // Add time and memory probes
  itkProbesCreate();

  std::cout << std::endl << "Starting Registration" << std::endl;

  try 
    { 
    itkProbesStart( "Registration" );
    registration->StartRegistration(); 
    itkProbesStop( "Registration" );
    std::cout << "Optimizer stop condition = "
              << registration->GetOptimizer()->GetStopConditionDescription()
              << std::endl;
    } 
  catch( itk::ExceptionObject & err ) 
    { 
    std::cerr << "ExceptionObject caught !" << std::endl; 
    std::cerr << err << std::endl; 
    return EXIT_FAILURE;
    } 
  
  OptimizerType::ParametersType finalParameters = 
                    registration->GetLastTransformParameters();


  // Report the time and memory taken by the registration
  itkProbesReport( std::cout );

  transform->SetParameters( finalParameters );


  typedef itk::ResampleImageFilter< 
                            MovingImageType, 
                            FixedImageType >    ResampleFilterType;

  ResampleFilterType::Pointer resample = ResampleFilterType::New();

  resample->SetTransform( transform );
  resample->SetInput( movingImageReader->GetOutput() );

  resample->SetSize(    fixedImage->GetLargestPossibleRegion().GetSize() );
  resample->SetOutputOrigin(  fixedImage->GetOrigin() );
  resample->SetOutputSpacing( fixedImage->GetSpacing() );
  resample->SetOutputDirection( fixedImage->GetDirection() );

  // This value is set to zero in order to make easier to perform
  // regression testing in this example. However, for didactic 
  // exercise it will be better to set it to a medium gray value
  // such as 100 or 128.
  resample->SetDefaultPixelValue( 0 );
  
  typedef  signed short  OutputPixelType;

  typedef itk::Image< OutputPixelType, ImageDimension > OutputImageType;
  
  typedef itk::CastImageFilter< 
                        FixedImageType,
                        OutputImageType > CastFilterType;
                    
  typedef itk::ImageFileWriter< OutputImageType >  WriterType;


  WriterType::Pointer      writer =  WriterType::New();
  CastFilterType::Pointer  caster =  CastFilterType::New();


  writer->SetFileName( argv[3] );


  caster->SetInput( resample->GetOutput() );
  writer->SetInput( caster->GetOutput()   );


  try
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & err ) 
    { 
    std::cerr << "ExceptionObject caught !" << std::endl; 
    std::cerr << err << std::endl; 
    return EXIT_FAILURE;
    } 
 


  typedef itk::SquaredDifferenceImageFilter< 
                                  FixedImageType, 
                                  FixedImageType, 
                                  OutputImageType > DifferenceFilterType;

  DifferenceFilterType::Pointer difference = DifferenceFilterType::New();

  WriterType::Pointer writer2 = WriterType::New();
  writer2->SetInput( difference->GetOutput() );  
  

  // Compute the difference image between the 
  // fixed and resampled moving image.
  if( argc > 4 )
    {
    difference->SetInput1( fixedImageReader->GetOutput() );
    difference->SetInput2( resample->GetOutput() );
    writer2->SetFileName( argv[4] );
    try
      {
      writer2->Update();
      }
    catch( itk::ExceptionObject & err ) 
      { 
      std::cerr << "ExceptionObject caught !" << std::endl; 
      std::cerr << err << std::endl; 
      return EXIT_FAILURE;
      } 
    }


  // Compute the difference image between the 
  // fixed and moving image before registration.
  if( argc > 5 )
    {
    writer2->SetFileName( argv[5] );
    difference->SetInput1( fixedImageReader->GetOutput() );
    difference->SetInput2( movingImageReader->GetOutput() );
    try
      {
      writer2->Update();
      }
    catch( itk::ExceptionObject & err ) 
      { 
      std::cerr << "ExceptionObject caught !" << std::endl; 
      std::cerr << err << std::endl; 
      return EXIT_FAILURE;
      } 
    }

  // Generate the explicit deformation field resulting from 
  // the registration.
  if( argc > 6 )
    {

    typedef itk::Vector< float, ImageDimension >      VectorType;
    typedef itk::Image< VectorType, ImageDimension >  DeformationFieldType;

    DeformationFieldType::Pointer field = DeformationFieldType::New();
    field->SetRegions( fixedRegion );
    field->SetOrigin( fixedImage->GetOrigin() );
    field->SetSpacing( fixedImage->GetSpacing() );
    field->SetDirection( fixedImage->GetDirection() );
    field->Allocate();

    typedef itk::ImageRegionIterator< DeformationFieldType > FieldIterator;
    FieldIterator fi( field, fixedRegion );

    fi.GoToBegin();

    TransformType::InputPointType  fixedPoint;
    TransformType::OutputPointType movingPoint;
    DeformationFieldType::IndexType index;

    VectorType displacement;

    while( ! fi.IsAtEnd() )
      {
      index = fi.GetIndex();
      field->TransformIndexToPhysicalPoint( index, fixedPoint );
      movingPoint = transform->TransformPoint( fixedPoint );
      displacement = movingPoint - fixedPoint;
      fi.Set( displacement );
      ++fi;
      }

    typedef itk::ImageFileWriter< DeformationFieldType >  FieldWriterType;
    FieldWriterType::Pointer fieldWriter = FieldWriterType::New();

    fieldWriter->SetInput( field );

    fieldWriter->SetFileName( argv[6] );
    try
      {
      fieldWriter->Update();
      }
    catch( itk::ExceptionObject & excp )
      {
      std::cerr << "Exception thrown " << std::endl;
      std::cerr << excp << std::endl;
      return EXIT_FAILURE;
      }
    }

  // Optionally, save the transform parameters in a file
  if( argc > 9 )
    {
    std::ofstream parametersFile;
    parametersFile.open( argv[9] );
    parametersFile << finalParameters << std::endl;
    parametersFile.close();
    }

  return EXIT_SUCCESS;
}

#undef itkProbesCreate
#undef itkProbesStart
#undef itkProbesStop
#undef itkProbesReport
