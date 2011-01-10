/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    DeformableRegistration9.cxx
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

#include "itkConfigure.h"


#ifndef USE_FFTWD
#error "This program needs single precision FFTWD to work."
#endif


#include "itkImageFileReader.h" 
#include "itkImageFileWriter.h" 

#include "itkCurvatureRegistrationFilter.h"
#include "itkFastSymmetricForcesDemonsRegistrationFunction.h"

#include "itkHistogramMatchingImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkWarpImageFilter.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkExceptionObject.h"

const unsigned int Dimension = 2;

//  The following section of code implements a Command observer
//  that will monitor the evolution of the registration process.
//
  class CommandIterationUpdate : public itk::Command 
  {
  public:
    typedef  CommandIterationUpdate   Self;
    typedef  itk::Command             Superclass;
    typedef  itk::SmartPointer<CommandIterationUpdate>  Pointer;
    itkNewMacro( CommandIterationUpdate );
  protected:
    CommandIterationUpdate() {};

    typedef itk::Image< float, Dimension > InternalImageType;
    typedef itk::Vector< float, Dimension >    VectorPixelType;
    typedef itk::Image<  VectorPixelType, Dimension > DeformationFieldType;

    typedef itk::CurvatureRegistrationFilter<
                                InternalImageType,
                                InternalImageType,
                                DeformationFieldType,
                                itk::FastSymmetricForcesDemonsRegistrationFunction<InternalImageType,InternalImageType,DeformationFieldType> >   RegistrationFilterType;

  public:

    void Execute(itk::Object *caller, const itk::EventObject & event)
      {
        Execute( (const itk::Object *)caller, event);
      }

    void Execute(const itk::Object * object, const itk::EventObject & event)
      {
         const RegistrationFilterType * filter = 
          dynamic_cast< const RegistrationFilterType * >( object );
        if( !(itk::IterationEvent().CheckEvent( &event )) )
          {
          return;
          }
        std::cout << filter->GetMetric() << std::endl;
      }
  };


int main( int argc, char *argv[] )
{
  if( argc < 4 )
    {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " fixedImageFile movingImageFile ";
    std::cerr << " outputImageFile " << std::endl;
    return EXIT_FAILURE;
    }

  typedef short PixelType;

  typedef itk::Image< PixelType, Dimension >  FixedImageType;
  typedef itk::Image< PixelType, Dimension >  MovingImageType;
  typedef itk::Image< float, Dimension > JacobianImageType;

  typedef itk::ImageFileReader< FixedImageType  > FixedImageReaderType;
  typedef itk::ImageFileReader< MovingImageType > MovingImageReaderType;

  FixedImageReaderType::Pointer fixedImageReader   = 
    FixedImageReaderType::New();
  MovingImageReaderType::Pointer movingImageReader = 
    MovingImageReaderType::New();

  fixedImageReader->SetFileName( argv[1] );
  movingImageReader->SetFileName( argv[2] );

  typedef float InternalPixelType;
  typedef itk::Image< InternalPixelType, Dimension > InternalImageType;
  typedef itk::CastImageFilter< FixedImageType, 
                                InternalImageType > FixedImageCasterType;
  typedef itk::CastImageFilter< MovingImageType, 
                                InternalImageType > MovingImageCasterType;

  FixedImageCasterType::Pointer fixedImageCaster   = 
    FixedImageCasterType::New();
  MovingImageCasterType::Pointer movingImageCaster = 
    MovingImageCasterType::New();

  fixedImageCaster->SetInput( fixedImageReader->GetOutput() );
  movingImageCaster->SetInput( movingImageReader->GetOutput() ); 

  typedef itk::HistogramMatchingImageFilter<
                                    InternalImageType,
                                    InternalImageType >   MatchingFilterType;
  MatchingFilterType::Pointer matcher = MatchingFilterType::New();

  matcher->SetInput( movingImageCaster->GetOutput() );
  matcher->SetReferenceImage( fixedImageCaster->GetOutput() );
  matcher->SetNumberOfHistogramLevels( 1024 );
  matcher->SetNumberOfMatchPoints( 7 );
  matcher->ThresholdAtMeanIntensityOn();

  typedef itk::Vector< float, Dimension >    VectorPixelType;
  typedef itk::Image<  VectorPixelType, Dimension > DeformationFieldType;
  typedef itk::CurvatureRegistrationFilter<
                                InternalImageType,
                                InternalImageType,
                                DeformationFieldType,
                                itk::FastSymmetricForcesDemonsRegistrationFunction<InternalImageType,InternalImageType,DeformationFieldType> >   RegistrationFilterType;
  RegistrationFilterType::Pointer filter = RegistrationFilterType::New();

  CommandIterationUpdate::Pointer observer = CommandIterationUpdate::New();
  filter->AddObserver( itk::IterationEvent(), observer );

  filter->SetFixedImage( fixedImageCaster->GetOutput() );
  filter->SetMovingImage( matcher->GetOutput() );
  filter->SetNumberOfIterations( 150 );
  filter->SetTimeStep( 1 );
  filter->SetConstraintWeight( 1 );
  filter->Update();

  typedef itk::WarpImageFilter<
                          MovingImageType, 
                          MovingImageType,
                          DeformationFieldType  >     WarperType;
  typedef itk::LinearInterpolateImageFunction<
                                   MovingImageType,
                                   double          >  InterpolatorType;
  WarperType::Pointer warper = WarperType::New();
  InterpolatorType::Pointer interpolator = InterpolatorType::New();
  FixedImageType::Pointer fixedImage = fixedImageReader->GetOutput();

  warper->SetInput( movingImageReader->GetOutput() );
  warper->SetInterpolator( interpolator );
  warper->SetOutputSpacing( fixedImage->GetSpacing() );
  warper->SetOutputOrigin( fixedImage->GetOrigin() );
  warper->SetOutputDirection( fixedImage->GetDirection() );
  warper->SetDeformationField( filter->GetOutput() );

  // Write warped image out to file
  typedef unsigned short  OutputPixelType;
  typedef itk::Image< OutputPixelType, Dimension > OutputImageType;
  typedef itk::CastImageFilter< 
                        MovingImageType,
                        OutputImageType > CastFilterType;
  typedef itk::ImageFileWriter< OutputImageType >  WriterType;

  WriterType::Pointer      writer =  WriterType::New();
  CastFilterType::Pointer  caster =  CastFilterType::New();

  writer->SetFileName( argv[3] );
  
  caster->SetInput( warper->GetOutput() );
  writer->SetInput( caster->GetOutput() );
  writer->Update();

  if( argc > 4 ) // if a fourth line argument has been provided...
    {

    typedef itk::ImageFileWriter< DeformationFieldType > FieldWriterType;

    FieldWriterType::Pointer fieldWriter = FieldWriterType::New();
    fieldWriter->SetFileName( argv[4] );
    fieldWriter->SetInput( filter->GetOutput() );

    try
      {
      fieldWriter->Update();
      }
    catch ( itk::ExceptionObject e )
      {
      e.Print( std::cerr );
      }
    }

  return EXIT_SUCCESS;
}

