/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFEMRegistrationFilter.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkFEMRegistrationFilter_txx
#define __itkFEMRegistrationFilter_txx

// disable debug warnings in MS compiler
#ifdef _MSC_VER
#pragma warning(disable: 4786)
#endif

#include "itkFEMRegistrationFilter.h"

#include "itkFEMElements.h"
#include "itkFEMLoadBC.h"


#include "itkImageFileWriter.h"
#include "itkImageFileReader.h"
#include "itkCastImageFilter.h"
#include "itkDiscreteGaussianImageFilter.h"
#include "itkDerivativeImageFilter.h"
#include "itkGroupSpatialObject.h"
#include "itkVectorNeighborhoodOperatorImageFilter.h"
#include "itkNearestNeighborInterpolateImageFunction.h"
#include "itkBSplineInterpolateImageFunction.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkMinimumMaximumImageFilter.h"
#include "itkShiftScaleImageFilter.h"
#include "itkSpatialObject.h"
#include "itkSpatialObjectReader.h"
#include "itkFEMObjectSpatialObject.h"

#include "vnl/algo/vnl_determinant.h"

namespace itk {
namespace fem {


template<class TMovingImage,class TFixedImage,class TFemObject>
FEMRegistrationFilter<TMovingImage,TFixedImage,TFemObject>::~FEMRegistrationFilter( )
{
}


template<class TMovingImage,class TFixedImage,class TFemObject>
FEMRegistrationFilter<TMovingImage,TFixedImage,TFemObject>::FEMRegistrationFilter( )
{
  
  this->SetNumberOfRequiredInputs(2);

  m_FileCount=0;

  m_MinE=0;
  m_MinE=vnl_huge_val(m_MinE);

  m_CurrentLevel=0;
  m_DescentDirection=positive;
  m_E.set_size(1);
  m_Gamma.set_size(1);
  m_Gamma[m_CurrentLevel]=1;
  m_Rho.set_size(1);
  m_E[0]=1.;
  m_Rho[0]=1.;
  m_Maxiters.set_size(1);
  m_Maxiters[m_CurrentLevel]=1;
  m_TimeStep=1;
  m_Alpha=1.0;
  m_Temp=0.0;
  m_MeshPixelsPerElementAtEachResolution.set_size(1);
  m_NumberOfIntegrationPoints.set_size(1);
  m_NumberOfIntegrationPoints[m_CurrentLevel]=4;
  m_MetricWidth.set_size(1);
  m_MetricWidth[m_CurrentLevel]=3;
  m_DoLineSearchOnImageEnergy=1;
  m_LineSearchMaximumIterations=100;
  m_UseMassMatrix=true;

  m_NumLevels=1;
  m_MaxLevel=1;
  m_MeshStep=2;
  m_MeshLevels=1;
  m_UseMultiResolution=false;
  m_UseLandmarks=false;
  m_MinJacobian=1.0;

  m_TotalIterations=0;


  for (unsigned int i=0; i < ImageDimension; i++)
    {
    m_ImageScaling[i]=1;
    m_CurrentImageScaling[i]=1;
    m_FullImageSize[i]=0;
    m_ImageOrigin[i]=0;
    }
  m_FloatImage=NULL;
  m_Field=NULL;
  m_TotalField=NULL;
  m_WarpedImage=NULL;

  // Setup the default interpolator
  typename DefaultInterpolatorType::Pointer interp =
    DefaultInterpolatorType::New();

  m_Interpolator =
    static_cast<InterpolatorType*>( interp.GetPointer() );
  m_Interpolator->SetInputImage(m_Field);

  m_Load = 0;
  
  m_EmployRegridding = 1;

}

template<class TMovingImage,class TFixedImage,class TFemObject>
void FEMRegistrationFilter<TMovingImage,TFixedImage,TFemObject>::RunRegistration(void)
{
  // Solve the system in time
  if (!m_UseMultiResolution && m_Maxiters[m_CurrentLevel] > 0)
    {
    typedef typename itk::fem::FEMObject< 3 >    TestFEMObjectType;
    TestFEMObjectType::Pointer femObject = TestFEMObjectType::New();
    
    typedef SolverCrankNicolson<3>   TestSolverType;
        
    TestSolverType::Pointer mySolver = TestSolverType::New();
    mySolver->SetDeltaT(m_TimeStep);
    mySolver->SetRho(m_Rho[m_CurrentLevel]);
    mySolver->SetAlpha(m_Alpha);
    CreateMesh(static_cast<double>(m_MeshPixelsPerElementAtEachResolution[m_CurrentLevel]),
               mySolver,m_FullImageSize);
    ApplyLoads(m_FullImageSize);

    const unsigned int ndofpernode=(m_Element)->GetNumberOfDegreesOfFreedomPerNode();
    const unsigned int numnodesperelt=(m_Element)->GetNumberOfNodes()+1;
    const unsigned int ndof=femObject->GetNumberOfDegreesOfFreedom();
    unsigned int nzelts;
    
    nzelts=numnodesperelt*ndofpernode*ndof;
    //Used if reading a mesh
    //nzelts=((2*numnodesperelt*ndofpernode*ndof > 25*ndof) ? 2*numnodesperelt*ndofpernode*ndof : 25*ndof);
    
    LinearSystemWrapperItpack itpackWrapper;
    itpackWrapper.SetMaximumNonZeroValuesInMatrix(nzelts);
    itpackWrapper.SetMaximumNumberIterations(2*mySolver->GetOutput()->GetNumberOfDegreesOfFreedom());
    itpackWrapper.SetTolerance(1.e-1);
    //    itpackWrapper.JacobianSemiIterative();
    itpackWrapper.JacobianConjugateGradient();
    mySolver->SetLinearSystemWrapper(&itpackWrapper);

//VAM - Moved to Solver
#if 0
    if( m_UseMassMatrix )
      {
      mySolver->AssembleKandM();
      }
    else
      {
      mySolver->InitializeForSolution();
      mySolver->AssembleK();
      }
#endif
    IterativeSolve(mySolver);
    //    InterpolateVectorField(&mySolver);
    }
  else //if (m_Maxiters[m_CurrentLevel] > 0)
    {
    MultiResSolve();
    }

  if (m_Field)
    {
    if (m_TotalField) m_Field=m_TotalField;
    this->ComputeJacobian(1.,m_Field,2.5);
    WarpImage(m_OriginalMovingImage);
    }
  return;
}

template<class TMovingImage,class TFixedImage,class TFemObject>
void FEMRegistrationFilter<TMovingImage,TFixedImage,TFemObject>::SetMovingImage(MovingImageType* R)
{
  m_MovingImage=R;
  if (m_TotalIterations == 0)
    {
    m_OriginalMovingImage=R;
    this->ProcessObject::SetNthInput( 0, const_cast< MovingImageType * >( R ) );
    }
}

template<class TMovingImage,class TFixedImage,class TFemObject>
void FEMRegistrationFilter<TMovingImage,TFixedImage,TFemObject>::SetFixedImage(FixedImageType* T)
{
  m_FixedImage=T;
  m_FullImageSize = m_FixedImage->GetLargestPossibleRegion().GetSize();

  if (m_TotalIterations == 0)
    {
    this->ProcessObject::SetNthInput( 1, const_cast< FixedImageType * >( T ) );
    }
  VectorType disp;
  for (unsigned int i=0; i < ImageDimension; i++)
    {
    disp[i]=0.0;
    m_ImageOrigin[i]=0;
    }

  m_CurrentLevelImageSize=m_FullImageSize;
}

template<class TMovingImage,class TFixedImage,class TFemObject>
void FEMRegistrationFilter<TMovingImage,TFixedImage,TFemObject>::SetInputFEMObject(FEMObjectType* F, unsigned int level)
{
  this->ProcessObject::SetNthInput( 2+level, const_cast< FEMObjectType * >( F ) );
}

template<class TMovingImage,class TFixedImage,class TFemObject>
typename FEMRegistrationFilter<TMovingImage,TFixedImage,TFemObject>::FEMObjectType* 
FEMRegistrationFilter<TMovingImage,TFixedImage,TFemObject>::GetInputFEMObject(unsigned int level)
{
  return const_cast<FEMObjectType *>(this->GetNthInput(2+level));
}


template<class TMovingImage,class TFixedImage,class TFemObject>
void FEMRegistrationFilter<TMovingImage,TFixedImage,TFemObject>::ChooseMetric(unsigned int which)
{
  // Choose the similarity metric


// for using the imagetoimagemetricloads
#ifdef  USEIMAGEMETRIC
  typedef itk::MeanSquaresImageToImageMetric<FixedImageType,MovingImageType> MetricType0;
  typedef itk::NormalizedCorrelationImageToImageMetric<FixedImageType,MovingImageType> MetricType1;
  typedef itk::PatternIntensityImageToImageMetric<FixedImageType,MovingImageType> MetricType2;
  typedef itk::MutualInformationImageToImageMetric<FixedImageType,MovingImageType> MetricType3;
  typedef itk::MattesMutualInformationImageToImageMetric<FixedImageType,MovingImageType> MetricType4;
//  typedef itk::DemonsImageToImageMetric<FixedImageType,MovingImageType> MetricType5;

  typedef itk::MeanSquaresImageToImageMetric<FixedImageType,MovingImageType> MetricType5;

  float m_Temp=1.0;

  MetricType3::Pointer m=MetricType3::New();
  MetricType4::Pointer ma=MetricType4::New();

  unsigned int whichmetric=(unsigned int) which;

  m_WhichMetric=which;
  unsigned int nsp=1;
  for (int i=0; i<ImageDimension; i++)
    {
    nsp *= m_MetricWidth[m_CurrentLevel];
    }
  if (which == 3 )
    {
    m->SetNumberOfSpatialSamples(nsp/2);
    m->SetFixedImageStandardDeviation(0.4);
    m->SetMovingImageStandardDeviation(0.4);
    }
  else if (which == 4 )
    {
    ma->SetNumberOfHistogramBins( 10 );
    ma->SetNumberOfSpatialSamples( nsp/2 );
    }

  switch (which)
    {
    case 0:
      m_Metric=MetricType0::New();
      m_Metric->SetScaleGradient(m_Temp); // this is the default(?)
      //p=MetricType0::New();
      //m_Function->SetInverseMetric(p);
      break;
    case 1:
      m_Metric=MetricType1::New();
      m_Metric->SetScaleGradient(m_Temp);
      break;
    case 2:
      m_Metric=MetricType2::New();
      m_Metric->SetScaleGradient(m_Temp);
      break;
    case 3:
      m_Metric=m;
      m_Metric->SetScaleGradient(m_Temp);
      break;
    case 4:
      m_Metric=ma;
      m_Metric->SetScaleGradient(m_Temp);
      break;
    case 5:
      m_Metric=MetricType5::New();
      m_Metric->SetScaleGradient(m_Temp);
      break;
    default:
      m_Metric=MetricType0::New();
      m_Metric->SetScaleGradient(m_Temp);

    }
#else

  typedef itk::MeanSquareRegistrationFunction<FixedImageType,MovingImageType,FieldType> MetricType0;
  typedef itk::NCCRegistrationFunction<FixedImageType,MovingImageType,FieldType> MetricType1;
  typedef itk::NCCRegistrationFunction<FixedImageType,MovingImageType,FieldType> MetricType2;
  typedef itk::MIRegistrationFunction<FixedImageType,MovingImageType,FieldType> MetricType3;
  typedef itk::MIRegistrationFunction<FixedImageType,MovingImageType,FieldType> MetricType4;
  typedef itk::DemonsRegistrationFunction<FixedImageType,MovingImageType,FieldType> MetricType5;

  typename MetricType3::Pointer m=MetricType3::New();
  typename MetricType4::Pointer ma=MetricType4::New();

  m_WhichMetric=(unsigned int)which;

  switch (which)
    {
    case 0:
      m_Metric=MetricType0::New();
      break;
    case 1:
      m_Metric=MetricType1::New();
      break;
    case 2:
      m_Metric=MetricType2::New();
      break;
    case 3:
      m_Metric=m;
      break;
    case 4:
      m_Metric=ma;
      break;
    case 5:
      m_Metric=MetricType5::New();
      break;
    default:
      m_Metric=MetricType0::New();
    }


  m_Metric->SetGradientStep( m_Gamma[m_CurrentLevel] );
  if ( m_Temp == 1.0 ) m_Metric->SetNormalizeGradient(true);
  else m_Metric->SetNormalizeGradient(false);
#endif
}

#if 0
template<class TMovingImage,class TFixedImage,class TFemObject>
bool FEMRegistrationFilter<TMovingImage,TFixedImage,TFemObject>::ReadConfigFile(const char* fname)
// Reads the parameters necessary to configure the example & returns
// false if no configuration file is found
{
  std::ifstream f;
  char buffer[4096] = {'\0'};
  Float fbuf = 0.0;
  unsigned int ibuf = 0;
  unsigned int jj;

  std::cout << "Reading config file..." << fname << std::endl;
  f.open(fname);
  if (f)
    {

    this->DoMultiRes(true);

    FEMLightObject::SkipWhiteSpace(f);
    f >> ibuf;
    this->m_NumLevels = (unsigned int) ibuf;

    FEMLightObject::SkipWhiteSpace(f);
    f >> ibuf;
    this->m_MaxLevel = ibuf;

    // get the initial scales for the pyramid
    FEMLightObject::SkipWhiteSpace(f);
    for (jj=0; jj<ImageDimension; jj++)
      {
      f >> ibuf;
      m_ImageScaling[jj] = ibuf;
      }

    this->m_MeshPixelsPerElementAtEachResolution.set_size(m_NumLevels);
    FEMLightObject::SkipWhiteSpace(f);
    for (jj=0; jj<this->m_NumLevels; jj++)
      {
      f >> ibuf;
      this->m_MeshPixelsPerElementAtEachResolution(jj) = ibuf;
      }

    FEMLightObject::SkipWhiteSpace(f);
    this->m_E.set_size(m_NumLevels);
    for (jj=0; jj<this->m_NumLevels; jj++)
      {
      f >> fbuf;
      this->SetElasticity(fbuf,jj);
      }

    FEMLightObject::SkipWhiteSpace(f);
    this->m_Rho.set_size(m_NumLevels);
    for (jj=0; jj<this->m_NumLevels; jj++)
      {
      f >> fbuf;
      this->SetRho(fbuf,jj);
      }

    FEMLightObject::SkipWhiteSpace(f);
    this->m_Gamma.set_size(m_NumLevels);
    for (jj=0; jj<this->m_NumLevels; jj++)
      {
      f >> fbuf;
      this->SetGamma(fbuf,jj);
      }

    FEMLightObject::SkipWhiteSpace(f);
    this->m_NumberOfIntegrationPoints.set_size(m_NumLevels);
    for(jj=0; jj< m_NumLevels; jj++)
      {
      f >> ibuf;
      this->SetNumberOfIntegrationPoints(ibuf,jj);
      }

    FEMLightObject::SkipWhiteSpace(f);
    this->m_MetricWidth.set_size(m_NumLevels);
    for(jj=0; jj< m_NumLevels; jj++)
      {
      f >> ibuf;
      this->SetWidthOfMetricRegion(ibuf,jj);
      }

    FEMLightObject::SkipWhiteSpace(f);
    this->m_Maxiters.set_size(m_NumLevels);
    for (jj=0; jj<this->m_NumLevels; jj++)
      {
      f >> ibuf;
      this->SetMaximumIterations(ibuf,jj);
      }

    FEMLightObject::SkipWhiteSpace(f);
    float fbuf2=1.0;
    f >> fbuf;
    f >> fbuf2;
    m_Temp=fbuf2;
    this->ChooseMetric(fbuf);

    FEMLightObject::SkipWhiteSpace(f);
    f >> fbuf;
    this->m_Alpha=fbuf;

    FEMLightObject::SkipWhiteSpace(f);
    f >> ibuf;
    if (ibuf == 0)
      {
      this->SetDescentDirectionMinimize();
      }
    else
      {
      this->SetDescentDirectionMaximize();
      }

    FEMLightObject::SkipWhiteSpace(f);
    f >> ibuf;
    this->DoLineSearch(ibuf);

    FEMLightObject::SkipWhiteSpace(f);
    f >> fbuf;
    this->SetTimeStep(fbuf);

    FEMLightObject::SkipWhiteSpace(f);
    f >> fbuf;
    this->SetEnergyReductionFactor(fbuf);

    FEMLightObject::SkipWhiteSpace(f);
    f >> ibuf;
    m_EmployRegridding = (unsigned int) ibuf;

    FEMLightObject::SkipWhiteSpace(f);
    f >> ibuf;
    this->m_FullImageSize[0] = ibuf;

    FEMLightObject::SkipWhiteSpace(f);
    f >> ibuf;
    this->m_FullImageSize[1] = ibuf;

    FEMLightObject::SkipWhiteSpace(f);
    f >> ibuf;
    unsigned int dim=2;
    if (ibuf > 0) dim=3;
    if (dim == 3) this->m_FullImageSize[2] = ibuf;

    FEMLightObject::SkipWhiteSpace(f);
    f >> buffer;
    this->SetMovingFile(buffer);

    FEMLightObject::SkipWhiteSpace(f);
    f >> buffer;
    this->SetFixedFile(buffer);

    FEMLightObject::SkipWhiteSpace(f);
    f >> ibuf;
    FEMLightObject::SkipWhiteSpace(f);
    f >> buffer;

    if (ibuf == 1)
      {
      this->UseLandmarks(true);
      this->SetLandmarkFile(buffer);
      }
    else
      {
      this->UseLandmarks(false);
      }

    FEMLightObject::SkipWhiteSpace(f);
    f >> buffer;
    this->SetResultsFile(buffer);

    FEMLightObject::SkipWhiteSpace(f);
    f >> ibuf;
    FEMLightObject::SkipWhiteSpace(f);
    f >> buffer;

    if (ibuf == 1)
      {
      this->SetWriteDisplacements(true);
      this->SetDisplacementsFile(buffer);
      }
    else
      {
      this->SetWriteDisplacements(false);
      }

    FEMLightObject::SkipWhiteSpace(f);
    f >> ibuf;
    FEMLightObject::SkipWhiteSpace(f);
    f >> buffer;

    if (ibuf == 1)
      {
      this->m_ReadMeshFile=true;
      this->m_MeshFileName=buffer;
      }
    else
      {
      this->m_ReadMeshFile=false;
      }

    f.close();
    std::cout << "Example configured. E " << m_E << " rho " << m_Rho << std::endl;
    return true;
    }
  else
    {
    std::cout << "No configuration file specified...quitting.\n";
    return false;
    }
}
#endif

template<class TMovingImage,class TFixedImage,class TFemObject>
int FEMRegistrationFilter<TMovingImage,TFixedImage,TFemObject>::WriteDisplacementFieldMultiComponent()
// Outputs the displacement field as a multicomponent image  XYZXYZXYZ...
{

  std::cout << "Writing multi-component displacement vector field...";

  typedef itk::ImageFileWriter< FieldType >  FieldWriterType;
  typename FieldWriterType::Pointer  fieldWriter = FieldWriterType::New();

  fieldWriter->SetInput( m_Field );
  fieldWriter->SetFileName("VectorDeformationField.mhd");
  try
    {
    fieldWriter->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << "Error while saving the displacement vector field" << std::endl;
    std::cerr << excp << std::endl;
    }

  std::cout << "done" << std::endl;
  return 0;
}

template<class TMovingImage,class TFixedImage,class TFemObject>
int FEMRegistrationFilter<TMovingImage,TFixedImage,TFemObject>::WriteDisplacementField(unsigned int index)
// Outputs the displacement field for the index provided (0=x,1=y,2=z)
{
  // Initialize the Moving to the displacement field
  typename IndexSelectCasterType::Pointer fieldCaster = IndexSelectCasterType::New();
  fieldCaster->SetInput( m_Field );
  fieldCaster->SetIndex( index );

  // Define the output of the Moving
  typename FloatImageType::Pointer fieldImage = FloatImageType::New();
  fieldCaster->Update();
  fieldImage = fieldCaster->GetOutput();

  // Set up the output filename
  std::string outfile="DeformationField_"+static_cast<char>('x'+index)+std::string("vec.mhd");

  typedef typename FloatImageType::PixelType FType;

  typedef ImageFileWriter<FloatImageType> WriterType;
  typename WriterType::Pointer writer = WriterType::New();
  writer->SetInput(fieldImage);
  writer->SetFileName(outfile.c_str());
  writer->Write();

  return 0;
}

template<class TMovingImage,class TFixedImage,class TFemObject>
void FEMRegistrationFilter<TMovingImage,TFixedImage,TFemObject>::WarpImage( const MovingImageType * ImageToWarp)
{
  // -------------------------------------------------------
  std::cout << "Warping image" << std::endl;

  {
  typename WarperType::Pointer warper = WarperType::New();

  typedef typename WarperType::CoordRepType WarperCoordRepType;
  typedef itk::NearestNeighborInterpolateImageFunction<MovingImageType,WarperCoordRepType>
    InterpolatorType0;
  typedef itk::LinearInterpolateImageFunction<MovingImageType,WarperCoordRepType>
    InterpolatorType1;
  typedef itk::BSplineInterpolateImageFunction<MovingImageType,WarperCoordRepType>
    InterpolatorType2;
  typename InterpolatorType1::Pointer interpolator = InterpolatorType1::New();

  warper = WarperType::New();
  warper->SetInput( ImageToWarp );
  warper->SetDeformationField( m_Field );
  warper->SetInterpolator( interpolator );
  warper->SetOutputOrigin( m_FixedImage->GetOrigin() );
  warper->SetOutputSpacing( m_FixedImage->GetSpacing() );
  warper->SetOutputDirection( m_FixedImage->GetDirection() );
  typename FixedImageType::PixelType padValue = 0;
  warper->SetEdgePaddingValue( padValue );
  warper->Update();

  m_WarpedImage=warper->GetOutput();
  }

}


template<class TMovingImage,class TFixedImage,class TFemObject>
void FEMRegistrationFilter<TMovingImage,TFixedImage,TFemObject>::CreateMesh(double PixelsPerElement,
                                           SolverType *mySolver, ImageSizeType imagesize)
{

/*  InterpolationGridPointType MeshOriginV;
  InterpolationGridPointType MeshSizeV;
  InterpolationGridSizeType ImageSizeV;
  InterpolationGridSizeType ElementsPerDim;
  
//  vnl_vector<double> MeshOriginV; MeshOriginV.set_size(ImageDimension);
//  vnl_vector<double> MeshSizeV;   MeshSizeV.set_size(ImageDimension);
//  vnl_vector<double> ImageSizeV;   ImageSizeV.set_size(ImageDimension);
//  vnl_vector<double> ElementsPerDim;  ElementsPerDim.set_size(ImageDimension);
  for (unsigned int i=0; i<ImageDimension; i++)
    {
    MeshSizeV[i]=(double)imagesize[i]; // FIX ME  make more general

    MeshOriginV[i]=(double)m_ImageOrigin[i];// FIX ME make more general
    ImageSizeV[i]=(double) imagesize[i]+1;//to make sure all elts are inside the interpolation mesh
    ElementsPerDim[i]=MeshSizeV[i]/PixelsPerElement;

    }

  std::cout << " ElementsPerDim " << ElementsPerDim << std::endl;*/

  vnl_vector< unsigned int > pixPerElement;
  pixPerElement.set_size(3);
  pixPerElement[0] = static_cast<unsigned int>( PixelsPerElement );
  pixPerElement[1] = static_cast<unsigned int>( PixelsPerElement );
  pixPerElement[2] = static_cast<unsigned int>( PixelsPerElement );
  
/***VAM***/
#if 0
  if (m_ReadMeshFile)
    {
    
    	typedef itk::SpatialObject<ImageDimension>    SpatialObjectType;
	typedef SpatialObjectType::Pointer            SpatialObjectPointer;
	SpatialObjectPointer Spatial = SpatialObjectType::New();

	typedef itk::SpatialObjectReader<ImageDimension>    SpatialObjectReaderType;
	typedef SpatialObjectReaderType::Pointer            SpatialObjectReaderPointer;
	SpatialObjectReaderPointer SpatialReader = SpatialObjectReaderType::New();
	SpatialReader->SetFileName( m_MeshFileName.c_str() );
	SpatialReader->Update();

	SpatialObjectReaderType::ScenePointer myScene = SpatialReader->GetScene();
	if(!myScene)
	{
		return ;
	}

	// Testing the fe mesh validity
	typedef itk::FEMObjectSpatialObject<ImageDimension>    FEMObjectSpatialObjectType;
	typedef FEMObjectSpatialObjectType::Pointer            FEMObjectSpatialObjectPointer;

	FEMObjectSpatialObjectType::ChildrenListType* children = SpatialReader->GetGroup()->GetChildren();
	if(strcmp((*(children->begin()))->GetTypeName(),"FEMObjectSpatialObject"))
	{
		return ;
	}

	FEMObjectSpatialObjectType::Pointer femSO = 
		dynamic_cast<FEMObjectSpatialObjectType*>((*(children->begin())).GetPointer());

	femSO->GetFEMObject()->FinalizeMesh();
	
    femObject->DeepCopy(femSO->GetFEMObject());

    itk::fem::MaterialLinearElasticity::Pointer m=dynamic_cast<MaterialLinearElasticity*>(femObject->GetMaterial(0));
    if (m)
      {
      m->SetYoungsModulus(this->GetElasticity(m_CurrentLevel));  // Young modulus -- used in the membrane ///
      }
      
      m_Element = femObject->GetElement(0);
    // now scale the mesh to the current scale
    Element::VectorType coord;
    
    int numNodes = femObject->GetNodeContainer()->Size();
 
    for(int i=0; i<numNodes; i++)
      {
      coord = femObject->GetNode(i)->GetCoordinates();
      for (unsigned int ii = 0; ii < ImageDimension; ii++)
        {
        coord[ii] = coord[ii]/(float)m_CurrentImageScaling[ii];
        }
      femObject->GetNode(i)->SetCoordinates(coord);
      }
     mySolver->SetInput(femObject);		
    }
  else 
#endif  
  if (ImageDimension == 2 && dynamic_cast<Element2DC0LinearQuadrilateral*>(&*m_Element) != NULL)
    {
    m_Material->SetYoungsModulus(this->GetElasticity(m_CurrentLevel));
    //FIXME - Use ImageToRectilinearFEMObjectFilter
    //Generate2DRectilinearMesh1(m_Element,femObject,MeshOriginV,MeshSizeV,ElementsPerDim);
    mySolver->SetInput(m_FEMObject);
 //   std::cout << " init interpolation grid : im sz " << ImageSizeV << " MeshSize " << MeshSizeV << std::endl;
    //FIXME
    //mySolver->InitializeInterpolationGrid(ImageSizeV,MeshOriginV,MeshSizeV);
    std::cout << " done initializing interpolation grid " << std::endl;
    }
  else if ( ImageDimension == 3 && dynamic_cast<Element3DC0LinearHexahedron*>(&*m_Element) != NULL)
    {
    m_Material->SetYoungsModulus( this->GetElasticity(m_CurrentLevel));
    std::cout << " generating regular mesh " << std::endl;
    //FIXME
    //Generate3DRectilinearMesh1(m_Element,femObject,MeshOriginV,MeshSizeV,ElementsPerDim);
    ImageToMeshType::Pointer meshFilter = ImageToMeshType::New();
    meshFilter->SetInput( m_MovingImage );
    meshFilter->SetPixelsPerElement( pixPerElement );
    meshFilter->SetElement( &*m_Element );
    meshFilter->Update();
    m_FEMObject = meshFilter->GetOutput();
    m_FEMObject->FinalizeMesh();
    mySolver->SetInput(m_FEMObject);
    std::cout << " generating regular mesh done " << std::endl;
    // the global to local transf is too slow so don't do it.
//    std::cout << " DO NOT init interpolation grid : im sz " << ImageSizeV << " MeshSize " << MeshSizeV << std::endl;
    //mySolver.InitializeInterpolationGrid(ImageSizeV,MeshOriginV,MeshSizeV);
    //std::cout << " done initializing interpolation grid " << std::endl;
    }
  else
    {
    FEMException e(__FILE__, __LINE__);
    e.SetDescription("CreateMesh - wrong image or element type");
    e.SetLocation(ITK_LOCATION);
    throw e;
    }
}


template<class TMovingImage,class TFixedImage,class TFemObject>
void FEMRegistrationFilter<TMovingImage,TFixedImage,TFemObject>
::ApplyImageLoads(TMovingImage*  movingimg, TFixedImage* fixedimg )
{
  m_Load = FEMRegistrationFilter<TMovingImage,TFixedImage,TFemObject>::ImageMetricLoadType::New();
  m_Load->SetMovingImage(movingimg);
  m_Load->SetFixedImage(fixedimg);
  if (!m_Field) this->InitializeField();
#ifndef USEIMAGEMETRIC
  m_Load->SetDeformationField(this->GetDeformationField());
#endif
  m_Load->SetMetric(m_Metric);
  m_Load->InitializeMetric();
  m_Load->SetTemp(m_Temp);
  m_Load->SetGamma(m_Gamma[m_CurrentLevel]);
  ImageSizeType r;
  for (unsigned int dd = 0; dd < ImageDimension; dd++)
    {
    r[dd] = m_MetricWidth[m_CurrentLevel];
    }
  m_Load->SetMetricRadius(r);
  m_Load->SetNumberOfIntegrationPoints(m_NumberOfIntegrationPoints[m_CurrentLevel]);
  m_Load->SetGlobalNumber(m_FEMObject->GetNumberOfLoads()+1); //NOTE SETTING GN FOR FIND LATER
  m_Load->SetSign((Float)m_DescentDirection);
  m_FEMObject->AddNextLoad(&*m_Load);
  m_Load = dynamic_cast<typename FEMRegistrationFilter<TMovingImage,TFixedImage,TFemObject>::ImageMetricLoadType*>
    (&*m_FEMObject->GetLoadWithGlobalNumber(m_FEMObject->GetNumberOfLoads()));
}


template<class TMovingImage,class TFixedImage,class TFemObject>
void FEMRegistrationFilter<TMovingImage,TFixedImage,TFemObject>::ApplyLoads(
								ImageSizeType ImgSz, double* scaling)
{
  //
  // Apply the boundary conditions.  We pin the image corners.
  // First compute which elements these will be.
  ///
  itkDebugMacro( << " applying loads " );

  vnl_vector<Float> pd; pd.set_size(ImageDimension);
  vnl_vector<Float> pu; pu.set_size(ImageDimension);

  if (m_UseLandmarks)
    {
/**FIXME */
/*    LoadArray::iterator loaditerator;
    LoadLandmark::Pointer l3;

    if ( this->m_LandmarkArray.empty() )
      {
      // Landmark loads
      std::ifstream f;
      std::cout << m_LandmarkFileName << std::endl;
      f.open(m_LandmarkFileName.c_str());
      if (f)
        {

        itkDebugMacro( << "Try loading landmarks..." );
        try
          {
          // changes made - kiran
          //mySolver.load.clear(); // NOTE: CLEARING ALL LOADS - LMS MUST BE APPLIED FIRST
          // **mySolver.ClearLoadArray();
          // changes made - kiran
          // **mySolver.Read(f);
          }
        catch (itk::ExceptionObject &err)
          {
          std::cerr << "Exception: cannot read load landmark FEMRegistrationFilter.txx " << err;
          }
        f.close();
        
		// changes made - kiran
       // m_LandmarkArray.resize(mySolver.load.size());
       m_LandmarkArray.resize(mySolver.GetNumberOfLoads());
        unsigned int ct = 0;
        //for(loaditerator = mySolver.load.begin(); loaditerator != mySolver.load.end(); loaditerator++)
        for(loaditerator = mySolver.GetLoadArray().begin(); loaditerator != mySolver.GetLoadArray().end(); loaditerator++)
          // changes made - kiran
          {
          if ((l3 = dynamic_cast<LoadLandmark*>( &(*(*loaditerator)) )) != 0 )
            {
#ifdef USE_FEM_CLONE
            LoadLandmark::Pointer l4 = dynamic_cast<LoadLandmark*>(l3->Clone());
#else
            LoadLandmark::Pointer l4 = dynamic_cast<LoadLandmark*>(l3->CreateAnother());
#endif
            m_LandmarkArray[ct] = l4;
            ct++;
            }
          }
          
		// changes made - kiran
        //mySolver.load.clear(); // NOTE: CLEARING ALL LOADS - LMS MUST BE APPLIED FIRST
        mySolver.ClearLoadArray();
        // changes made - kiran
        }
      else
        {
        std::cout << "no landmark file specified." << std::endl;
        }
*/
      }


  // now scale the landmarks

    itkDebugMacro( " num of LM loads " << m_LandmarkArray.size() );
    /*
     * Step over all the loads again to scale them by the global landmark weight.
     */
    if ( !m_LandmarkArray.empty())
      {
      for(unsigned int lmind = 0; lmind<m_LandmarkArray.size(); lmind++)
        {
        m_LandmarkArray[lmind]->GetElementArray()[0] = NULL;
        
        bool isFound = false;
        itkDebugMacro( << " Prescale Pt " <<  m_LandmarkArray[lmind]->GetTarget() );
        if (scaling)
          {
          m_LandmarkArray[lmind]->ScalePointAndForce(scaling,m_EnergyReductionFactor);
          itkDebugMacro( << " Postscale Pt " <<  m_LandmarkArray[lmind]->GetTarget() << " scale " << scaling[0] );
          }

        pu = m_LandmarkArray[lmind]->GetSource();
        pd = m_LandmarkArray[lmind]->GetPoint();
        
        
        int numElements = m_FEMObject->GetNumberOfElements();
        for (int i=0; i<numElements; i++)
          {
			    if ( m_FEMObject->GetElement(i)->GetLocalFromGlobalCoordinates(pu, pd ) )
            {
            isFound = true;
            m_LandmarkArray[lmind]->SetPoint(pd);
            m_LandmarkArray[lmind]->GetElementArray()[0] =  m_FEMObject->GetElement(i);
            }
          }
          
        m_LandmarkArray[lmind]->SetGlobalNumber(lmind);
#ifdef USE_FEM_CLONE
        LoadLandmark::Pointer l5 = dynamic_cast< LoadLandmark * >( &*m_LandmarkArray[lmind]->Clone() );
#else
        LoadLandmark::Pointer l5 = dynamic_cast< LoadLandmark * >( &*m_LandmarkArray[lmind]->CreateAnother() );
#endif
        m_FEMObject->AddNextLoad(&*l5);
        }
    itkDebugMacro( << " landmarks done" );
    }

  // now apply the BC loads
  LoadBC::Pointer l1;

  //Pin  one corner of image
  unsigned int CornerCounter,ii,EdgeCounter = 0;
  
  int numNodes = m_FEMObject->GetNumberOfNodes();
  
  Element::VectorType coord;
  
  bool EdgeFound;
  unsigned int nodect = 0;
  for (int i=0; i<numNodes; i++)
  {
	  if(EdgeCounter >= ImageDimension)	return;  
 
    coord = m_FEMObject->GetNode(i)->GetCoordinates();
    CornerCounter = 0;
    for (ii = 0; ii < ImageDimension; ii++)
      {
      if (coord[ii] == m_ImageOrigin[ii] || coord[ii] == ImgSz[ii]-1 ) CornerCounter++;
      }
      
    if (CornerCounter == ImageDimension) // the node is located at a true corner
      {
      unsigned int ndofpernode=(*(m_FEMObject->GetNode(i)->m_elements.begin()))->GetNumberOfDegreesOfFreedomPerNode();
      unsigned int numnodesperelt=(*(m_FEMObject->GetNode(i)->m_elements.begin()))->GetNumberOfNodes();
      unsigned int whichnode;

      unsigned int maxnode=numnodesperelt-1;

      typedef typename Node::SetOfElements NodeEltSetType;
      for( NodeEltSetType::iterator elt = m_FEMObject->GetNode(i)->m_elements.begin();
           elt != m_FEMObject->GetNode(i)->m_elements.end(); elt++)
        {
        for (whichnode=0; whichnode<=maxnode; whichnode++)
          {
          coord=(*elt)->GetNode(whichnode)->GetCoordinates();
          CornerCounter=0;

          for (ii=0; ii < ImageDimension; ii++)
            {
            if (coord[ii] == m_ImageOrigin[ii] || coord[ii] == ImgSz[ii]-1 )
              {
              CornerCounter++;
              }
            }
          if (CornerCounter == ImageDimension - 1)
            {
            EdgeFound=true;
            }
          else
            {
            EdgeFound=false;
            }
          if (EdgeFound)
            {
            for (unsigned int jj=0; jj<ndofpernode; jj++)
              {
              itkDebugMacro( " which node " << whichnode );
              itkDebugMacro( " edge coord " << coord );

              l1=LoadBC::New();
              // now we get the element from the node -- we assume we need fix the dof only once
              // even if more than one element shares it.
              
              l1->SetElement(*elt);  
              unsigned int localdof=whichnode*ndofpernode+jj;
              l1->SetDegreeOfFreedom(localdof);
              l1->SetValue(vnl_vector<double>(1,0.0));
       
              m_FEMObject->AddNextLoad(&*l1);
              }
            EdgeCounter++;
            }
          }
        }//end elt loop
      }
    nodect++;
    itkDebugMacro( << " node " << nodect );
    }

  return;
}

template<class TMovingImage,class TFixedImage,class TFemObject>
void FEMRegistrationFilter<TMovingImage,TFixedImage,TFemObject>::IterativeSolve(SolverType *mySolver)
{
  if (!m_Load)
    {
    std::cout << " m_Load not initialized " << std::endl;
    return;
    }

  bool Done=false;
  unsigned int iters=0;
  m_MinE=10.e99;
  Float deltE=0;
  while ( !Done && iters < m_Maxiters[m_CurrentLevel] )
    {
    const Float lastdeltE=deltE;
    const unsigned int DLS=m_DoLineSearchOnImageEnergy;
    //  Reset the variational similarity term to zero.

    Float LastE=m_Load->GetCurrentEnergy();
    m_Load->SetCurrentEnergy(0.0);
    m_Load->InitializeMetric();

    if (!m_Field)
      {
      std::cout << " Big Error -- Field is NULL ";
      }
    //   Assemble the master force vector (from the applied loads)
    // if (iters == 1) // for testing

//VAM - Moved to Solver
#if 0
    if( m_UseMassMatrix )
      {
      mySolver->AssembleFforTimeStep();
      }
    else
      {
      mySolver->AssembleF();
      }
#endif

    // Solve the system of equations for displacements (u=K^-1*F)
    mySolver->Modified();
    mySolver->Update();
    m_Load->PrintCurrentEnergy();

#ifndef USEIMAGEMETRIC
    if (m_DescentDirection == 1)
      {
      deltE=(LastE - m_Load->GetCurrentEnergy());
      }
    else
      {
      deltE=(m_Load->GetCurrentEnergy() - LastE );
      }
#else
    if (m_DescentDirection == 1)
      {
      deltE=(m_Load->GetCurrentEnergy() - LastE );
      }
    else
      {
      deltE=(LastE - m_Load->GetCurrentEnergy());
      }
#endif

    if (  DLS==2 && deltE < 0.0 )
      {
      std::cout << " line search ";
      const float tol = 1.0;//((0.01  < LastE) ? 0.01 : LastE/10.);
      LastE=this->GoldenSection(mySolver,tol,m_LineSearchMaximumIterations);
      deltE=(m_MinE-LastE);
      std::cout << " line search done " << std::endl;
      }

    iters++;

    if (deltE == 0.0)
      {
      std::cout << " no change in energy " << std::endl;
      Done=true;
      }
    if ( (DLS == 0)  && (  iters >= m_Maxiters[m_CurrentLevel] ) )
      {
      Done=true;
      }
    else if ((DLS > 0) &&
             ( iters >= m_Maxiters[m_CurrentLevel] || (deltE < 0.0 && iters > 5 && lastdeltE < 0.0)))
      {
      Done=true;
      }
    float curmaxsol=mySolver->GetCurrentMaxSolution();
    if (curmaxsol == 0)
      {
      curmaxsol=1.0;
      }
    Float mint= m_Gamma[m_CurrentLevel]/curmaxsol;
    if (mint > 1)
      {
      mint = 1.0;
      }

    if (mySolver->GetCurrentMaxSolution() < 0.01 && iters > 2)
      {
      Done=true;
      }
    mySolver->AddToDisplacements(mint);
    m_MinE=LastE;

    InterpolateVectorField(mySolver);


    if (m_EmployRegridding != 0)
      {
      if ( iters % m_EmployRegridding == 0  )
        {
        this->EnforceDiffeomorphism(1.0, mySolver, true);
        //     std::string rfn="warpedimage";
        //     WriteWarpedImage(rfn.c_str());
        }
      }
    // uncomment to write out every deformation SLOW due to interpolating vector field everywhere.
    //else if ( (iters % 5) == 0 || Done ) {
    //WarpImage(m_MovingImage);
    //WriteWarpedImage(m_ResultsFileName.c_str());
    //}
    std::cout << " min E " << m_MinE <<  " delt E " << deltE <<  " iter " << iters << std::endl;
    m_TotalIterations++;
    }
}

template<class TMovingImage,class TFixedImage,class TFemObject>
void FEMRegistrationFilter<TMovingImage,TFixedImage,TFemObject>
::InitializeField()
{

  itkDebugMacro( " allocating deformation field " );

  m_Field = FieldType::New();

  m_FieldRegion.SetSize(m_CurrentLevelImageSize );
  m_Field->SetOrigin( m_FixedImage->GetOrigin() );
  m_Field->SetSpacing( m_FixedImage->GetSpacing() );
  m_Field->SetDirection( m_FixedImage->GetDirection() );
  m_Field->SetLargestPossibleRegion( m_FieldRegion );
  m_Field->SetBufferedRegion( m_FieldRegion );
  m_Field->SetLargestPossibleRegion( m_FieldRegion );
  m_Field->Allocate();

  VectorType disp;
  for (unsigned int t=0; t<ImageDimension; t++)
    {
    disp[t]=0.0;
    }
  FieldIterator fieldIter( m_Field, m_FieldRegion );
  fieldIter.GoToBegin();
  for(; !fieldIter.IsAtEnd(); ++fieldIter )
    {
    fieldIter.Set(disp);
    }
}

template<class TMovingImage,class TFixedImage,class TFemObject>
void
FEMRegistrationFilter<TMovingImage,TFixedImage,TFemObject>::InterpolateVectorField(SolverType *mySolver)
{

  typename FieldType::Pointer field=m_Field;

  if (!field)
    {
    this->InitializeField();
    }
  m_FieldSize=field->GetLargestPossibleRegion().GetSize();

  std::cout << " interpolating vector field of size " << m_FieldSize;

  Float rstep,sstep,tstep;

  vnl_vector<double> Pos;  // solution at the point
  vnl_vector<double> Sol;  // solution at the local point
  vnl_vector<double> Gpt;  // global position given by local point


  VectorType disp;
  for (unsigned int t=0; t<ImageDimension; t++)
    {
    disp[t]=0.0;
    }
  FieldIterator fieldIter( field, field->GetLargestPossibleRegion() );

  fieldIter.GoToBegin();
  typename FixedImageType::IndexType rindex = fieldIter.GetIndex();

  Sol.set_size(ImageDimension);
  Gpt.set_size(ImageDimension);

  if (ImageDimension == 2)
    {
    Element::ConstPointer eltp;

    for(; !fieldIter.IsAtEnd(); ++fieldIter )
      {
      // get element pointer from the solver elt pointer image
      rindex = fieldIter.GetIndex();
      for(unsigned int d=0; d<ImageDimension; d++)
        {
        Gpt[d]=(double)rindex[d];
        }
      eltp=mySolver->GetElementAtPoint(Gpt);
      if (eltp)
        {
        eltp->GetLocalFromGlobalCoordinates(Gpt, Pos);

        unsigned int Nnodes= eltp->GetNumberOfNodes();
        typename Element::VectorType shapef(Nnodes);
        shapef = eltp->ShapeFunctions(Pos);
        Float solval;
        for(unsigned int f=0; f<ImageDimension; f++)
          {
          solval=0.0;
          for(unsigned int n=0; n<Nnodes; n++)
            {
            solval += shapef[n] * mySolver->GetLS()->GetSolutionValue(
              eltp->GetNode(n)->GetDegreeOfFreedom(f) , mySolver->GetTotalSolutionIndex() );
            }
          Sol[f]=solval;
          disp[f] =(Float) 1.0*Sol[f];
          }
        field->SetPixel(rindex, disp );
        }
      }
    }
  
  if (ImageDimension==3)
    {
    // FIXME SHOULD BE 2.0 over meshpixperelt
    rstep=1.25/((double)m_MeshPixelsPerElementAtEachResolution[m_CurrentLevel]);//
    sstep=1.25/((double)m_MeshPixelsPerElementAtEachResolution[m_CurrentLevel]);//
    tstep=1.25/((double)m_MeshPixelsPerElementAtEachResolution[m_CurrentLevel]);//
//  std::cout << " r s t steps " << rstep << " " << sstep << " "<< tstep << std::endl;

    Pos.set_size(ImageDimension);
    // changes made - kiran
    int numElements = mySolver->GetInput()->GetNumberOfElements();
    for (int i=0; i<numElements; i++) 
    //for(  Element::ArrayType::iterator elt=mySolver.el.begin(); elt != mySolver.el.end(); elt++)
    // Element::ArrayType* elements = &(mySolver.GetElementArray());
    //for(  Element::ArrayType::iterator elt = elements->begin();
      //       elt != elements->end(); elt++)
    // changes made - kiran
      {
      Element::Pointer eltp = mySolver->GetInput()->GetElement(i);
      for (double r=-1.0; r <= 1.0; r=r+rstep )
        {
        for (double s=-1.0; s <= 1.0; s=s+sstep )
          {
          for (double t=-1.0; t <= 1.0; t=t+tstep )
            {
            Pos[0]=r;
            Pos[1]=s;
            Pos[2]=t;

            unsigned int Nnodes= eltp->GetNumberOfNodes();
            typename Element::VectorType shapef(Nnodes);

#define FASTHEX
#ifdef FASTHEX
//FIXME temporarily using hexahedron shape f for speed
            shapef[0] = (1 - r) * (1 - s) * (1 - t) * 0.125;
            shapef[1] = (1 + r) * (1 - s) * (1 - t) * 0.125;
            shapef[2] = (1 + r) * (1 + s) * (1 - t) * 0.125;
            shapef[3] = (1 - r) * (1 + s) * (1 - t) * 0.125;
            shapef[4] = (1 - r) * (1 - s) * (1 + t) * 0.125;
            shapef[5] = (1 + r) * (1 - s) * (1 + t) * 0.125;
            shapef[6] = (1 + r) * (1 + s) * (1 + t) * 0.125;
            shapef[7] = (1 - r) * (1 + s) * (1 + t) * 0.125;
#else
            shapef = (*eltp)->ShapeFunctions(Pos);
#endif

            Float solval,posval;
            bool inimage=true;

//        float interperror=0.0;

            for(unsigned int f=0; f<ImageDimension; f++)
              {
              solval=0.0;
              posval=0.0;
              for(unsigned int n=0; n<Nnodes; n++)
                {
                posval += shapef[n]*(((eltp)->GetNodeCoordinates(n))[f]);
                solval += shapef[n] * mySolver->GetLS()->GetSolutionValue(
                  (eltp)->GetNode(n)->GetDegreeOfFreedom(f) , mySolver->GetTotalSolutionIndex() );
                }
              Sol[f]=solval;
              Gpt[f]=posval;

              Float x=Gpt[f];
              long int temp;
              if (x != 0) temp=(long int) ((x)+0.5); else temp=0;// round
              rindex[f]=temp;
              disp[f] =(Float) 1.0*Sol[f];
              if ( temp < 0 || temp > (long int) m_FieldSize[f]-1)  inimage=false;

              }
            if (inimage) field->SetPixel(rindex, disp );
            }
          }
        }//end of for loops
      } // end of elt array loop

    }/* */ // end if imagedimension==3

  // Insure that the values are exact at the nodes. They won't necessarily be unless we use this code.
  std::cout << " interpolation done " << std::endl;
}

template<class TMovingImage,class TFixedImage,class TFemObject>
void FEMRegistrationFilter<TMovingImage,TFixedImage,TFemObject>::ComputeJacobian( float sign,
                                                                       FieldType* field, float smooth)
{
  unsigned int row;
  unsigned int col;
  bool jproduct=true;

  m_MinJacobian=1.0;

  if (!field)
    {
    field=m_Field;
    jproduct=false;
    }

  if ( !m_FloatImage && jproduct)
    {
    std::cout << " allocating m_FloatImage " << std::endl;
    m_FloatImage = FloatImageType::New();
    m_FloatImage->SetLargestPossibleRegion( field->GetLargestPossibleRegion() );
    m_FloatImage->SetBufferedRegion( field->GetLargestPossibleRegion().GetSize() );
    m_FloatImage->Allocate();

    ImageRegionIteratorWithIndex<FloatImageType> wimIter( m_FloatImage, m_FloatImage->GetLargestPossibleRegion()  );
    wimIter.GoToBegin();
    for(; !wimIter.IsAtEnd(); ++wimIter )
      {
      wimIter.Set(1.0);
      }
    }

  typename Element::MatrixType jMatrix,idMatrix;
  jMatrix.set_size(ImageDimension,ImageDimension);

  FieldIterator fieldIter( field, field->GetLargestPossibleRegion() );
  typename FixedImageType::IndexType rindex;
  typename FixedImageType::IndexType ddrindex;
  typename FixedImageType::IndexType ddlindex;

  typename FixedImageType::IndexType difIndex[ImageDimension][2];

  std:: cout << " get jacobian " << std::endl;

  float det;
  unsigned int posoff=1;

  float space=1.0;

  typename FieldType::PixelType dPix;
  typename FieldType::PixelType lpix;
  typename FieldType::PixelType llpix;
  typename FieldType::PixelType rpix;
  typename FieldType::PixelType rrpix;
  typename FieldType::PixelType cpix;

  for(  fieldIter.GoToBegin(); !fieldIter.IsAtEnd(); ++fieldIter )
    {
    rindex=fieldIter.GetIndex();

    bool oktosample=true;

    cpix=field->GetPixel(rindex);
    for(row=0; row< ImageDimension;row++)
      {
      difIndex[row][0]=rindex;
      difIndex[row][1]=rindex;
      ddrindex=rindex;
      ddlindex=rindex;
      if (rindex[row] <
          static_cast<typename FixedImageType::IndexType::IndexValueType>(m_FieldSize[row]-2) )
        {
        difIndex[row][0][row]=rindex[row]+posoff;
        ddrindex[row]=rindex[row]+posoff*2;
        } else oktosample=false;
      if (rindex[row] > 1 )
        {
        difIndex[row][1][row]=rindex[row]-1;
        ddlindex[row]=rindex[row]-2;
        } else oktosample=false;

      float h=1.0;
      space=1.0; // should use image spacing here?

      rpix = field->GetPixel(difIndex[row][1]);
      rpix = rpix*h+cpix*(1.-h);
      lpix = field->GetPixel(difIndex[row][0]);
      lpix = lpix*h+cpix*(1.-h);
      dPix = ( rpix - lpix)*sign*space/(2.0);
      for(col=0; col< ImageDimension;col++)
        {
        Float val;
        if (row == col) val=dPix[col]+1.0;
        else val = dPix[col];
        jMatrix.put(row,col,val);
        }
      }

    //the determinant of the jacobian matrix
    det = (float) vnl_determinant(jMatrix);
    if (det < 0.) det=0.0;
    if ( jproduct && oktosample )  // FIXME - NEED TO COMPOSE THE FULL FIELD
      m_FloatImage->SetPixel(rindex,  m_FloatImage->GetPixel(rindex)*det );
    if ( oktosample)
      {
      if (det < m_MinJacobian) m_MinJacobian=det;
      }
    }
  std::cout << " min Jacobian " << m_MinJacobian << std::endl;

  if (jproduct && m_FloatImage && smooth > 0)
    {
    typedef DiscreteGaussianImageFilter<FloatImageType, FloatImageType> DGF;

    typename DGF::Pointer filter = DGF::New();
    filter->SetVariance(smooth);
    filter->SetMaximumError(.01f);
    filter->SetInput(m_FloatImage);
    filter->Update();
    m_FloatImage=filter->GetOutput();
    }

}

template<class TMovingImage,class TFixedImage,class TFemObject>
void FEMRegistrationFilter<TMovingImage,TFixedImage,TFemObject>::EnforceDiffeomorphism(float thresh,
                                                                            SolverType *mySolver ,  bool onlywriteimages )
{
  itkDebugMacro( << " Checking Jacobian " );
  
  
  // FIXME - WE NEED TO STORE THE PRODUCTS OF THE JACOBIANS
  //          s.t. WE TRACK THE JACOBIAN OF THE O.D.E. FLOW.
  this->ComputeJacobian(1.,NULL);
  if (m_MinJacobian < thresh) //FIXME
    {
    itkDebugMacro( << " Enforcing diffeomorphism " );
    // resize the vector field to full size
    typename FieldType::Pointer fullField=NULL;
    ExpandFactorsType expandFactors[ImageDimension];
    bool resize=false;
    for (unsigned int ef=0; ef<ImageDimension; ef++)
      {
      ExpandFactorsType factor=(ExpandFactorsType)
        ((float) m_FullImageSize[ef]/(float)m_CurrentLevelImageSize[ef]);
      expandFactors[ef]=factor;
      if (factor != 1.) resize=true;
      }
    if (resize)
      fullField=ExpandVectorField(expandFactors,m_Field); // this works - linear interp and expansion of vf
    else fullField=m_Field;

    // FIXME : SHOULD COMPUTE THE JACOBIAN AGAIN AND EXIT THE FUNCTION
    // IF IT'S NOT BELOW THE THRESH.  ALSO, WE SHOULD STORE THE FULL TIME
    // INTEGRATED JACOBIAN FIELD AND MULTIPLY IT BY THE INCREMENTAL.

    // here's where we warp the image

    typename WarperType::Pointer warper = WarperType::New();
    typedef typename WarperType::CoordRepType WarperCoordRepType;
    typedef itk::NearestNeighborInterpolateImageFunction<MovingImageType,WarperCoordRepType>
      InterpolatorType0;
    typedef itk::LinearInterpolateImageFunction<MovingImageType,WarperCoordRepType>
      InterpolatorType1;
    typename InterpolatorType1::Pointer interpolator = InterpolatorType1::New();

    // if using landmarks, warp them

    if (m_UseLandmarks)
      {
      itkDebugMacro( << " warping landmarks " << m_LandmarkArray.size() );

      if(!m_LandmarkArray.empty())
        {
        for(unsigned int lmind=0; lmind<m_LandmarkArray.size(); lmind++)
          {
          itkDebugMacro( << " Old source: " << m_LandmarkArray[lmind]->GetSource() );
          itkDebugMacro( << " Target: " << m_LandmarkArray[lmind]->GetTarget() );

          // Convert the source to warped coords.
          // changes made - kiran
 /*         m_LandmarkArray[lmind]->GetSource()=m_LandmarkArray[lmind]->GetSource()+
            (dynamic_cast<LoadLandmark*>( &*mySolver.load.Find(lmind) )->GetForce() );*/
          m_LandmarkArray[lmind]->GetSource()=m_LandmarkArray[lmind]->GetSource()+
            (dynamic_cast<LoadLandmark*>( &*mySolver->GetOutput()->GetLoadWithGlobalNumber(lmind) )->GetForce() );
          // changes made - kiran
          itkDebugMacro( << " New source: " << m_LandmarkArray[lmind]->GetSource() );
          itkDebugMacro( << " Target: " << m_LandmarkArray[lmind]->GetTarget() );
#ifdef USE_FEM_CLONE
          LoadLandmark::Pointer l5 = dynamic_cast< LoadLandmark * >( &*m_LandmarkArray[lmind]->Clone());
#else
          LoadLandmark::Pointer l5 = dynamic_cast< LoadLandmark * >( &*m_LandmarkArray[lmind]->CreateAnother());
#endif
          // changes made - kiran
          //mySolver.load.push_back(FEMP<Load>(l5));
          mySolver->GetOutput()->AddNextLoad(&*l5);
		  // changes made - kiran
          }
          itkDebugMacro( << " warping landmarks done " );
        } 
        else 
        {
          itkDebugMacro( << " landmark array empty " );
        }
      }

    // store the total deformation by composing with the full field
    if (!m_TotalField && !onlywriteimages)
      {
      itkDebugMacro( << " allocating total deformation field " );

      m_TotalField = FieldType::New();

      m_FieldRegion.SetSize(fullField->GetLargestPossibleRegion().GetSize() );
      m_TotalField->SetLargestPossibleRegion( m_FieldRegion );
      m_TotalField->SetBufferedRegion( m_FieldRegion );
      m_TotalField->SetLargestPossibleRegion( m_FieldRegion );
      m_TotalField->Allocate();

      VectorType disp;
      for (unsigned int t=0; t<ImageDimension; t++)
        {
        disp[t]=0.0;
        }
      FieldIterator fieldIter( m_TotalField, m_FieldRegion );
      fieldIter.GoToBegin();
      for(; !fieldIter.IsAtEnd(); ++fieldIter )
        {
        fieldIter.Set(disp);
        }
      }

    if (onlywriteimages)
      {
      warper = WarperType::New();
      warper->SetInput( m_OriginalMovingImage );
      warper->SetDeformationField( fullField );
      warper->SetInterpolator( interpolator );
      warper->SetOutputOrigin( m_FixedImage->GetOrigin() );
      warper->SetOutputSpacing( m_FixedImage->GetSpacing() );
      warper->SetOutputDirection( m_FixedImage->GetDirection() );
      typename MovingImageType::PixelType padValue = 0;
      warper->SetEdgePaddingValue( padValue );
      warper->Update();
      m_WarpedImage=warper->GetOutput();
      }
    else if (m_TotalField)
      {

      typename InterpolatorType::ContinuousIndexType inputIndex;

      typedef typename InterpolatorType::OutputType InterpolatedType;

      InterpolatedType interpolatedValue;

      m_Interpolator->SetInputImage(fullField);

      typename FixedImageType::IndexType index;
      FieldIterator totalFieldIter( m_TotalField, m_TotalField->GetLargestPossibleRegion() );
      totalFieldIter.GoToBegin();
      unsigned int jj;
      float pathsteplength=0;
      while( !totalFieldIter.IsAtEnd()  )
        {
        index=totalFieldIter.GetIndex();
        for (jj=0; jj<ImageDimension; jj++)
          {
          inputIndex[jj]=(WarperCoordRepType) index[jj];
          interpolatedValue[jj]=0.0;
          }
        if( m_Interpolator->IsInsideBuffer( inputIndex ) )
          {

          interpolatedValue =
            m_Interpolator->EvaluateAtContinuousIndex( inputIndex );

          }
        VectorType interped;
        float temp=0.0;
        for (jj = 0; jj < ImageDimension; jj++)
          {
          interped[jj] = interpolatedValue[jj];
          temp += interped[jj]*interped[jj];
          }
        pathsteplength += vcl_sqrt(temp);
        m_TotalField->SetPixel(index,m_TotalField->GetPixel(index)+interped);
        ++totalFieldIter;
        }
      itkDebugMacro( << " incremental path length " << pathsteplength );


      // then we set the field to zero

        {
        FieldIterator fieldIter( m_Field, m_Field->GetLargestPossibleRegion() );
        fieldIter.GoToBegin();
        while( !fieldIter.IsAtEnd()  )
          {
          VectorType disp;
          disp.Fill(0.0);
          fieldIter.Set(disp);
          ++fieldIter;
          }
        }
      // now do the same for the solver
      unsigned int ii;
      // changes made - kiran
      //Node::ArrayType* nodes = &(mySolver.node);
      // changes made - kiran
      int numNodes = mySolver->GetOutput()->GetNumberOfNodes();
      for( int i=0; i<numNodes; i++)
        {
        // Now put it into the solution!
        for (ii=0; ii < ImageDimension; ii++)
          {
          mySolver->GetLinearSystemWrapper()->
            SetSolutionValue((mySolver->GetOutput()->GetNode(i))->GetDegreeOfFreedom(ii),0.0,mySolver->GetTotalSolutionIndex() );
          mySolver->GetLinearSystemWrapper()->
            SetSolutionValue((mySolver->GetOutput()->GetNode(i))->GetDegreeOfFreedom(ii),0.0,mySolver->GetSolutionTMinus1Index() );
          }
        }


      warper = WarperType::New();
      warper->SetInput( m_OriginalMovingImage );
      warper->SetDeformationField( m_TotalField );
      warper->SetInterpolator( interpolator );
      warper->SetOutputOrigin( m_FixedImage->GetOrigin() );
      warper->SetOutputSpacing( m_FixedImage->GetSpacing() );
      warper->SetOutputDirection( m_FixedImage->GetDirection() );
      typename FixedImageType::PixelType padValue = 0;
      warper->SetEdgePaddingValue( padValue );
      warper->Update();

      // set it as the new moving image

      this->SetMovingImage( warper->GetOutput() );

      m_WarpedImage=m_MovingImage;


      // now repeat the pyramid if necessary

      if ( resize )
        {
        itkDebugMacro( << " re-doing pyramid " );
        typename FixedPyramidType::Pointer   m_MovingPyramid;
        m_MovingPyramid = NULL;
        m_MovingPyramid = FixedPyramidType::New();
        m_MovingPyramid->SetInput( m_MovingImage);
        m_MovingPyramid->SetNumberOfLevels( 1 );
        typedef typename FixedPyramidType::ScheduleType ScheduleType;
        ScheduleType SizeReductionMoving=m_MovingPyramid->GetSchedule();

        ii=m_CurrentLevel;
        for (jj=0; jj<ImageDimension; jj++)
          {
          unsigned int scale=m_ImageScaling[jj]/(unsigned int)vcl_pow(2.0,(double)ii);
          if (scale < 1) scale=1;
          SizeReductionMoving[0][jj]=scale;
          }

        m_MovingPyramid->SetSchedule(SizeReductionMoving);
        m_MovingPyramid->GetOutput( 0 )->Update();
        m_Load->SetMovingImage(m_MovingPyramid->GetOutput(0));

        itkDebugMacro( << " re-doing pyramid done " );
        }
      else
        {
        m_Load->SetMovingImage(this->GetMovingImage( ));
        }
      }
    itkDebugMacro( << " Enforcing diffeomorphism done "  );
    }
}

template<class TMovingImage,class TFixedImage,class TFemObject>
void FEMRegistrationFilter<TMovingImage,TFixedImage,TFemObject>::WriteWarpedImage(const char* fname)
{

  // for image output
  std::ofstream fbin;
  std::string exte=".mhd";
  std::string fnum;

  OStringStream buf;
  buf<<(m_FileCount+10);
  fnum=std::string(buf.str().c_str());

  std::string fullfname=(fname+fnum+exte);

  if (!m_WarpedImage) return;

  typedef MinimumMaximumImageFilter<MovingImageType> MinMaxFilterType;
  typename MinMaxFilterType::Pointer minMaxFilter = MinMaxFilterType::New();
  minMaxFilter->SetInput( m_WarpedImage );
  minMaxFilter->Update();
  float min = minMaxFilter->GetMinimum();
  double shift = -1.0 * static_cast<double>( min );
  double scale = static_cast<double>( minMaxFilter->GetMaximum() );
  scale += shift;
  scale = 255.0 / scale;
  typedef ShiftScaleImageFilter<MovingImageType, MovingImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetInput( m_WarpedImage );
  filter->SetShift( shift );
  filter->SetScale( scale );
  filter->Update();


  typedef unsigned char                                     PIX;
  typedef itk::Image<PIX,ImageDimension>                    WriteImageType;
  typedef itk::CastImageFilter<TMovingImage,WriteImageType> CasterType1;
  typename CasterType1::Pointer caster1 = CasterType1::New();
  caster1->SetInput(filter->GetOutput());
  caster1->Update();
  typename ImageFileWriter<WriteImageType>::Pointer writer;
  writer = ImageFileWriter<WriteImageType>::New();
  writer->SetFileName(fullfname.c_str());
  writer->SetInput(caster1->GetOutput() );
  writer->Write();

  m_FileCount++;

}

template<class TMovingImage,class TFixedImage,class TFemObject>
typename FEMRegistrationFilter<TMovingImage,TFixedImage,TFemObject>::FieldPointer
FEMRegistrationFilter<TMovingImage,TFixedImage,TFemObject>::ExpandVectorField( ExpandFactorsType* expandFactors, FieldType* field)
{
  // re-size the vector field
  if (!field) field=m_Field;

  std::cout << " input field size " << m_Field->GetLargestPossibleRegion().GetSize()
            << " expand factors ";
  VectorType pad;
  for (unsigned int i=0; i< ImageDimension; i++)
    {
    pad[i]=0.0;
    std::cout << expandFactors[i] << " ";
    }
  std::cout << std::endl;
  typename ExpanderType::Pointer m_FieldExpander = ExpanderType::New();
  m_FieldExpander->SetInput(field);
  m_FieldExpander->SetExpandFactors( expandFactors );
  // use default
//TEST_RMV20100728   m_FieldExpander->SetEdgePaddingValue( pad );
  m_FieldExpander->UpdateLargestPossibleRegion();

  m_FieldSize=m_FieldExpander->GetOutput()->GetLargestPossibleRegion().GetSize();

  return m_FieldExpander->GetOutput();
}


template<class TMovingImage,class TFixedImage,class TFemObject>
void FEMRegistrationFilter<TMovingImage,TFixedImage,TFemObject>::SampleVectorFieldAtNodes(SolverType *mySolver)
{

  // Here, we need to iterate through the nodes, get the nodal coordinates,
  // sample the VF at the node and place the values in the SolutionVector.
  unsigned int ii;
  // changes made - kiran
  // Node::ArrayType* nodes = &(mySolver.node);
  int numNodes = mySolver->GetOutput()->GetNumberOfNodes();
  // changes made - kiran
  Element::VectorType coord;
  VectorType SolutionAtNode;

  m_Interpolator->SetInputImage(m_Field);

  for(  int i=0; i<numNodes; i++)
    {
    coord=mySolver->GetOutput()->GetNode(i)->GetCoordinates();
    typename InterpolatorType::ContinuousIndexType inputIndex;
    typedef typename InterpolatorType::OutputType InterpolatedType;
    InterpolatedType interpolatedValue;


    for (unsigned int jj=0; jj<ImageDimension; jj++)
      {
      inputIndex[jj]=(CoordRepType) coord[jj];
      interpolatedValue[jj]=0.0;
      }
    if( m_Interpolator->IsInsideBuffer( inputIndex ) )
      {
      interpolatedValue =
        m_Interpolator->EvaluateAtContinuousIndex( inputIndex );
      }

    for (unsigned int jj=0; jj<ImageDimension; jj++)
      {
      SolutionAtNode[jj]=interpolatedValue[jj];
      }

    // Now put it into the solution!
    for (ii=0; ii < ImageDimension; ii++)
      {
      Float Sol=SolutionAtNode[ii];
      mySolver->GetLinearSystemWrapper()->
        SetSolutionValue(mySolver->GetOutput()->GetNode(i)->GetDegreeOfFreedom(ii),Sol,mySolver->GetTotalSolutionIndex() );
      mySolver->GetLinearSystemWrapper()->
        SetSolutionValue(mySolver->GetOutput()->GetNode(i)->GetDegreeOfFreedom(ii),Sol,mySolver->GetSolutionTMinus1Index() );
      }
    }

}

template<class TMovingImage,class TFixedImage,class TFemObject>
void FEMRegistrationFilter<TMovingImage,TFixedImage,TFemObject>::PrintVectorField(unsigned int modnum)
{
  FieldIterator fieldIter( m_Field, m_Field->GetLargestPossibleRegion() );
  fieldIter.GoToBegin();
  unsigned int ct=0;

  float max=0;
  while( !fieldIter.IsAtEnd()  )
    {
    VectorType disp=fieldIter.Get();
    if ((ct % modnum) == 0)  std::cout << " field pix " << fieldIter.Get() << std::endl;
    for (unsigned int i=0; i<ImageDimension;i++)
      {
      if (vcl_fabs(disp[i]) > max )
        {
        max=vcl_fabs(disp[i]);
        }
      }
    ++fieldIter;
    ct++;

    }
  std::cout << " max  vec " << max << std::endl;
}

template<class TMovingImage,class TFixedImage,class TFemObject>
void FEMRegistrationFilter<TMovingImage,TFixedImage,TFemObject>::MultiResSolve()
{

  vnl_vector<Float> LastResolutionSolution;

  //   Setup a multi-resolution pyramid
  typedef typename FixedPyramidType::ScheduleType ScheduleType;

  for (m_CurrentLevel=0; m_CurrentLevel<m_MaxLevel; m_CurrentLevel++)
    {
    std::cout << " beginning level " << m_CurrentLevel << std::endl;
    
    //   Setup a multi-resolution pyramid
    typedef SolverCrankNicolson<3>   TestSolverType;
    TestSolverType::Pointer SSS = TestSolverType::New();
    typename FixedImageType::SizeType nextLevelSize;
    nextLevelSize.Fill( 0 );
    typename FixedImageType::SizeType lastLevelSize;

    if (m_Maxiters[m_CurrentLevel] > 0)
      {
        {
        typename FixedPyramidType::Pointer   m_MovingPyramid;
        typename FixedPyramidType::Pointer   m_FixedPyramid;
        m_MovingPyramid = FixedPyramidType::New();
        m_FixedPyramid = FixedPyramidType::New();

        m_MovingPyramid->SetInput( m_MovingImage);
        m_FixedPyramid->SetInput( m_FixedImage);

        // set schedule by specifying the number of levels;
        m_MovingPyramid->SetNumberOfLevels( 1 );
        m_FixedPyramid->SetNumberOfLevels( 1 );
        
        ScheduleType SizeReductionMoving=m_MovingPyramid->GetSchedule();
        ScheduleType SizeReductionFixed=m_FixedPyramid->GetSchedule();

        unsigned int ii=m_CurrentLevel;
        for (unsigned int jj=0; jj<ImageDimension; jj++)
          {
          unsigned int scale=m_ImageScaling[jj]/(unsigned int)vcl_pow(2.0,(double)ii);
          unsigned int nextscale = m_ImageScaling[jj]/(unsigned int)vcl_pow(2.0,(double)(ii+1));
          if (scale < 1) scale=1;
          if (nextscale < 1) nextscale=1;
          SizeReductionMoving[0][jj]=scale;
          SizeReductionFixed[0][jj]=scale;
          nextLevelSize[jj]=(long int) ( (float) m_FullImageSize[jj] / (float) nextscale );
          }

        m_MovingPyramid->SetSchedule(SizeReductionMoving);
        m_FixedPyramid->SetSchedule(SizeReductionFixed);
        m_MovingPyramid->GetOutput( 0 )->Update();
        m_FixedPyramid->GetOutput( 0 )->Update();

        lastLevelSize=m_CurrentLevelImageSize;
        m_CurrentLevelImageSize=m_FixedPyramid->GetOutput( 0 )->GetLargestPossibleRegion().GetSize();
        if (m_CurrentLevel == m_MaxLevel-1) nextLevelSize=m_CurrentLevelImageSize;
        
        double scaling[ImageDimension];
        for (unsigned int d=0; d < ImageDimension; d++)
          {
          m_CurrentImageScaling[d]=SizeReductionMoving[0][d];
          if (m_CurrentLevel == 0) scaling[d]=(double)SizeReductionMoving[0][d];
          else scaling[d]=(double) lastLevelSize[d]/(double) m_CurrentLevelImageSize[d];
          std::cout << " scaling " << scaling[d] << std::endl;
        }
        double MeshResolution=(double)this->m_MeshPixelsPerElementAtEachResolution(m_CurrentLevel);

        SSS->SetDeltaT(m_TimeStep);
        SSS->SetRho(m_Rho[m_CurrentLevel]);
        SSS->SetAlpha(m_Alpha);
			       
        CreateMesh(MeshResolution,SSS,m_CurrentLevelImageSize);
        ApplyLoads(m_CurrentLevelImageSize,scaling);
        ApplyImageLoads(m_MovingPyramid->GetOutput(0),
                        m_FixedPyramid->GetOutput(0));

        m_MovingPyramid = FixedPyramidType::New();
        m_FixedPyramid = FixedPyramidType::New();
        
        }

      unsigned int ndofpernode=(m_Element)->GetNumberOfDegreesOfFreedomPerNode();
      unsigned int numnodesperelt=(m_Element)->GetNumberOfNodes()+1;
      unsigned int ndof=SSS->GetInput()->GetNumberOfDegreesOfFreedom();
      unsigned int nzelts;
      nzelts=numnodesperelt*ndofpernode*ndof;
      // Used when reading a mesh from file
      //nzelts=((2*numnodesperelt*ndofpernode*ndof > 25*ndof) ? 2*numnodesperelt*ndofpernode*ndof : 25*ndof);

      LinearSystemWrapperItpack itpackWrapper;
      unsigned int maxits=2*SSS->GetInput()->GetNumberOfDegreesOfFreedom();
      itpackWrapper.SetMaximumNumberIterations(maxits);
      itpackWrapper.SetTolerance(1.e-1);
      itpackWrapper.JacobianConjugateGradient();
      itpackWrapper.SetMaximumNonZeroValuesInMatrix(nzelts);
      SSS->SetLinearSystemWrapper(&itpackWrapper);

//VAM - Moved to Solver
#if 0
      if( m_UseMassMatrix )
        {
        SSS->AssembleKandM();
        }
      else
        {
        SSS->InitializeForSolution();
        SSS->AssembleK();
        }
#endif
      if (m_CurrentLevel > 0)
        {
        this->SampleVectorFieldAtNodes(SSS);
        }
      this->IterativeSolve(SSS);
      }

    // now expand the field for the next level, if necessary.
    if ( m_CurrentLevel == m_MaxLevel-1 && m_Field)
      {
      PrintVectorField(900000);
      std:: cout << " field size " << m_Field->GetLargestPossibleRegion().GetSize() << std::endl;

      }
    else if (m_CurrentLevel < m_MaxLevel-1 && m_Field)
      {
      ExpandFactorsType expandFactors[ImageDimension];
      for (unsigned int ef=0; ef<ImageDimension; ef++)
        {
        expandFactors[ef]=(ExpandFactorsType)
          ((float) nextLevelSize[ef]/(float)m_CurrentLevelImageSize[ef]);
        }
      m_Field=ExpandVectorField(expandFactors,m_Field);
      // this works - linear interp and expansion of vf
      PrintVectorField(900000);
      std:: cout << " field size " << m_Field->GetLargestPossibleRegion().GetSize() << std::endl;

      }

    std::cout << " end level " << m_CurrentLevel;


    }// end image resolution loop

  if (m_TotalField)
    {
    std::cout << " copy field " <<  m_TotalField->GetLargestPossibleRegion().GetSize()
              << " to " <<  m_Field->GetLargestPossibleRegion().GetSize() << std::endl;
    FieldIterator fieldIter( m_TotalField, m_TotalField->GetLargestPossibleRegion() );
    fieldIter.GoToBegin();
    for(; !fieldIter.IsAtEnd(); ++fieldIter )
      {
      typename FixedImageType::IndexType index = fieldIter.GetIndex();
      m_TotalField->SetPixel(index,m_TotalField->GetPixel(index)
                             + m_Field->GetPixel(index));
      }
    }
  return;
}


template<class TMovingImage,class TFixedImage,class TFemObject>
Element::Float FEMRegistrationFilter<TMovingImage,TFixedImage,TFemObject>::EvaluateResidual(SolverType *mySolver,Float t)
{
  Float SimE=m_Load->EvaluateMetricGivenSolution1(mySolver->GetOutput()->GetElementContainer(),t);
  Float maxsim=1.0;
  for (unsigned int i=0; i< ImageDimension; i++)
    {
    maxsim *= (Float)m_FullImageSize[i];
    }
  if ( m_WhichMetric != 0)
    {
    SimE=maxsim-SimE;
    }
  return vcl_fabs(static_cast<double>(SimE)); //+defe;
}

template<class TMovingImage,class TFixedImage,class TFemObject>
void FEMRegistrationFilter<TMovingImage,TFixedImage,TFemObject>::FindBracketingTriplet(SolverType *mySolver,Float* a, Float* b, Float* c)
{
  //  see Numerical Recipes

  const Float Gold=1.618034;
  const Float Glimit=100.0;
  const Float Tiny=1.e-20;
  Float ax=0.0;
  Float bx=1.0;
  Float fa=vcl_fabs(EvaluateResidual(mySolver, ax));
  Float fb=vcl_fabs(EvaluateResidual(mySolver, bx));

  Float dum;
  if ( fb > fa )
    {
    dum=ax; ax=bx; bx=dum;
    dum=fb; fb=fa; fa=dum;
    }

  Float cx=bx+Gold*(bx-ax);  // first guess for c - the 3rd pt needed to bracket the min
  Float fc=vcl_fabs(EvaluateResidual(mySolver, cx));

  Float ulim,u,r,q,fu;
  while (fb > fc  )
    // && vcl_fabs(ax) < 3. && vcl_fabs(bx) < 3. && vcl_fabs(cx) < 3.)
    {
    r=(bx-ax)*(fb-fc);
    q=(bx-cx)*(fb-fa);
    Float denom=(2.0*mySolver->GSSign(mySolver->GSMax(vcl_fabs(q-r),Tiny),q-r));
    u=(bx)-((bx-cx)*q-(bx-ax)*r)/denom;
    ulim=bx + Glimit*(cx-bx);
    if ((bx-u)*(u-cx) > 0.0)
      {
      fu=vcl_fabs(EvaluateResidual(mySolver, u));
      if (fu < fc)
        {
        ax=bx;
        bx=u;
        *a=ax; *b=bx; *c=cx;
        return;
        }
      else if (fu > fb)
        {
        cx=u;
        *a=ax; *b=bx; *c=cx;
        return;
        }

      u=cx+Gold*(cx-bx);
      fu=vcl_fabs(EvaluateResidual(mySolver, u));

      }
    else if ( (cx-u)*(u-ulim) > 0.0)
      {
      fu=vcl_fabs(EvaluateResidual(mySolver, u));
      if (fu < fc)
        {
        bx=cx; cx=u; u=cx+Gold*(cx-bx);
        fb=fc; fc=fu; fu=vcl_fabs(EvaluateResidual(mySolver, u));
        }

      }
    else if ( (u-ulim)*(ulim-cx) >= 0.0)
      {
      u=ulim;
      fu=vcl_fabs(EvaluateResidual(mySolver, u));
      }
    else
      {
      u=cx+Gold*(cx-bx);
      fu=vcl_fabs(EvaluateResidual(mySolver, u));
      }

    ax=bx; bx=cx; cx=u;
    fa=fb; fb=fc; fc=fu;

    }

  if ( vcl_fabs(ax) > 1.e3  || vcl_fabs(bx) > 1.e3 || vcl_fabs(cx) > 1.e3)
    {
    ax=-2.0;  bx=1.0;  cx=2.0;
    } // to avoid crazy numbers caused by bad bracket (u goes nuts)

  *a=ax; *b=bx; *c=cx;
  return;
}

template<class TMovingImage,class TFixedImage,class TFemObject>
Element::Float FEMRegistrationFilter<TMovingImage,TFixedImage,TFemObject>::GoldenSection(
							SolverType *mySolver,Float tol,unsigned int MaxIters)
{
  // We should now have a, b and c, as well as f(a), f(b), f(c),
  // where b gives the minimum energy position;
  Float ax, bx, cx;
  FindBracketingTriplet(mySolver,&ax, &bx, &cx);

  const Float R=0.6180339;
  const Float C=(1.0-R);

  Float x0=ax;
  Float x1;
  Float x2;
  Float x3=cx;
  if (vcl_fabs(cx-bx) > vcl_fabs(bx-ax))
    {
    x1=bx;
    x2=bx+C*(cx-bx);
    }
  else
    {
    x2=bx;
    x1=bx-C*(bx-ax);
    }

  Float f1=vcl_fabs(EvaluateResidual(mySolver, x1));
  Float f2=vcl_fabs(EvaluateResidual(mySolver, x2));
  unsigned int iters=0;
  while (vcl_fabs(x3-x0) > tol*(vcl_fabs(x1)+vcl_fabs(x2)) && iters < MaxIters)
    {
    iters++;
    if (f2 < f1)
      {
      x0=x1; x1=x2; x2=R*x1+C*x3;
      f1=f2; f2=vcl_fabs(EvaluateResidual(mySolver, x2));
      }
    else
      {
      x3=x2; x2=x1; x1=R*x2+C*x0;
      f2=f1; f1=vcl_fabs(EvaluateResidual(mySolver, x1));
      }
    }
  Float xmin,fmin;
  if (f1<f2)
    {
    xmin=x1;
    fmin=f1;
    }
  else
    {
    xmin=x2;
    fmin=f2;
    }

  mySolver->SetEnergyToMin(xmin);
  std:: cout << " emin " << fmin <<  " at xmin " << xmin << std::endl;
  return vcl_fabs(static_cast<double>(fmin));
}


template<class TMovingImage,class TFixedImage,class TFemObject>
void FEMRegistrationFilter<TMovingImage,TFixedImage,TFemObject>::
PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf( os, indent );

  if (m_Load)
    {
    os << indent << "Load = " << m_Load;
    }
  else
    {
    os << indent << "Load = " << "(None)" << std::endl;
    }
  if (m_Interpolator)
    {
    os << indent << "Interpolator = " << m_Interpolator;
    }
  else
    {
    os << indent << "Interpolator = " << "(None)" << std::endl;
    }
}

}} // end namespace itk::fem

#endif
