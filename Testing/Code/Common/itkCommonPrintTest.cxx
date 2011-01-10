/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkCommonPrintTest.cxx
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
#include "itkVector.h"
#include "itkPoint.h"
#include "itkMesh.h"

#include "itkAcosImageAdaptor.h"
#include "itkAddImageAdaptor.h"
#include "itkAffineTransform.h"
#include "itkAsinImageAdaptor.h"
#include "itkAtanImageAdaptor.h"
#include "itkAzimuthElevationToCartesianTransform.h"
#include "itkBSplineDeformableTransform.h"
#include "itkBSplineDerivativeKernelFunction.h"
#include "itkBSplineInterpolationWeightFunction.h"
#include "itkBSplineKernelFunction.h"
#include "itkBinaryThresholdImageFunction.h"
#include "itkBloxBoundaryPointImage.h"
#include "itkBloxBoundaryProfileImage.h"
#include "itkBloxCoreAtomImage.h"
#include "itkBloxBoundaryPointToCoreAtomImageFilter.h"
#include "itkBloxImage.h"
#include "itkBoundingBox.h"
#include "itkCellInterfaceVisitor.h"
#include "itkCenteredAffineTransform.h"
#include "itkCenteredRigid2DTransform.h"
#include "itkCenteredTransformInitializer.h"
#include "itkCentralDifferenceImageFunction.h"
#include "itkColorTable.h"
#include "itkCommand.h"
#include "itkConicShellInteriorExteriorSpatialFunction.h"
#include "itkCoreAtomImageToDistanceMatrixProcess.h"
#include "itkCosImageAdaptor.h"
#include "itkCreateObjectFunction.h"
#include "itkDifferenceImageFilter.h"
#include "itkDynamicLoader.h"
#include "itkElasticBodyReciprocalSplineKernelTransform.h"
#include "itkElasticBodySplineKernelTransform.h"
#include "itkEllipsoidInteriorExteriorSpatialFunction.h"
#include "itkEquivalencyTable.h"
#include "itkEuler2DTransform.h"
#include "itkEuler3DTransform.h"
#include "itkExpImageAdaptor.h"
#include "itkExpNegativeImageAdaptor.h"
#include "itkFastMutexLock.h"
#include "itkFileOutputWindow.h"
#include "itkFiniteCylinderSpatialFunction.h"
#include "itkFrustumSpatialFunction.h"
#include "itkGaussianBlurImageFunction.h"
#include "itkGaussianDerivativeImageFunction.h"
#include "itkGaussianDerivativeSpatialFunction.h"
#include "itkGaussianKernelFunction.h"
#include "itkGaussianSpatialFunction.h"
#include "itkIdentityTransform.h"
#include "itkImage.h"
#include "itkImageAdaptor.h"
#include "itkImageDuplicator.h"
#include "itkImageBase.h"
#include "itkImageRegionMultidimensionalSplitter.h"
#include "itkImageRegionSplitter.h"
#include "itkImportImageContainer.h"
#include "itkKLMSegmentationBorder.h"
#include "itkKLMSegmentationRegion.h"
#include "itkKernelTransform.h"
#include "itkLevelSetFunction.h"
#include "itkLightProcessObject.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkLog10ImageAdaptor.h"
#include "itkLogImageAdaptor.h"
#include "itkMapContainer.h"
#include "itkMatrixResizeableDataObject.h"
#include "itkMaximumDecisionRule.h"
#include "itkMaximumRatioDecisionRule.h"
#include "itkMeanImageFunction.h"
#include "itkMedianImageFunction.h"
#include "itkMesh.h"
#include "itkMeshSource.h"
#include "itkMeshToMeshFilter.h"
#include "itkMetaDataObject.h"
#include "itkMinimumDecisionRule.h"
#include "itkMultiThreader.h"
#include "itkMutexLock.h"
#include "itkNearestNeighborInterpolateImageFunction.h"
#include "itkNeighborhoodBinaryThresholdImageFunction.h"
#include "itkNeighborhoodOperatorImageFunction.h"
#include "itkNthElementImageAdaptor.h"
#include "itkObjectStore.h"
#include "itkOneWayEquivalencyTable.h"
#include "itkPointLocator.h"
#include "itkPointSet.h"
#include "itkProgressAccumulator.h"
#include "itkQuaternionRigidTransform.h"
#include "itkRGBToVectorImageAdaptor.h"
#include "itkRigid2DTransform.h"
#include "itkRigid3DPerspectiveTransform.h"
#include "itkRigid3DTransform.h"
#include "itkScaleSkewVersor3DTransform.h"
#include "itkScaleTransform.h"
#include "itkSegmentationBorder.h"
#include "itkSegmentationRegion.h"
#include "itkSimilarity2DTransform.h"
#include "itkSinImageAdaptor.h"
#include "itkSphereSpatialFunction.h"
#include "itkSqrtImageAdaptor.h"
#include "itkSymmetricEllipsoidInteriorExteriorSpatialFunction.h"
#include "itkTanImageAdaptor.h"
#include "itkTextOutput.h"
#include "itkThinPlateR2LogRSplineKernelTransform.h"
#include "itkThinPlateSplineKernelTransform.h"
#include "itkTorusInteriorExteriorSpatialFunction.h"
#include "itkTransform.h"
#include "itkTranslationTransform.h"
#include "itkValarrayImageContainer.h"
#include "itkVarianceImageFunction.h"
#include "itkVectorContainer.h"
#include "itkVectorLinearInterpolateImageFunction.h"
#include "itkVectorToRGBImageAdaptor.h"
#include "itkVectorImage.h"
#include "itkVersion.h"
#include "itkVersorRigid3DTransform.h"
#include "itkVersorTransform.h"
#include "itkVolumeSplineKernelTransform.h"
#include "itkXMLFileOutputWindow.h"

#include "itkRedPixelAccessor.h"

struct TestObject
{
  float vector[3];
  int counter; 
};


int itkCommonPrintTest(int , char* [])
{
  typedef itk::Image<float,2> InputType;
  typedef itk::Image<unsigned char,2> CharType;
  typedef itk::Image<float,2> OutputType;
  
  typedef itk::Point<float,2> PointType;
  typedef itk::Mesh<PointType>  MeshType;

  typedef itk::Vector<float,2> VectorType;
  typedef itk::Image<VectorType, 2> VectorImageType;

  typedef itk::RGBPixel<unsigned short> RGBPixelType;
  typedef itk::Image<RGBPixelType,2> RGBImageType;

  // Used for CenteredTransformInitializer
  typedef itk::CenteredRigid2DTransform<float> TransformType;

  // Used for BloxImage
  typedef itk::BloxPixel<PointType> BloxPixelType;
  
  // Used for ImageAdaptor
  typedef itk::RedPixelAccessor<float> RedAccessorType;
  
  // Used for CoreAtomImageToDistanceMatrixProcess
  typedef itk::BloxBoundaryPointToCoreAtomImageFilter<3>::TOutputImage BloxCAImageType;
  
  itk::AcosImageAdaptor<InputType,InputType>::Pointer AcosImageAdaptorObj =
    itk::AcosImageAdaptor<InputType,InputType>::New();
  std::cout << "------------AcosImageAdaptor" << AcosImageAdaptorObj;
  
  itk::AddImageAdaptor<InputType>::Pointer AddImageAdaptorObj =
    itk::AddImageAdaptor<InputType>::New();
  std::cout << "------------AddImageAdaptor" << AddImageAdaptorObj;
  
  itk::AffineTransform<float,3>::Pointer AffineTransformObj =
    itk::AffineTransform<float,3>::New();
  std::cout << "------------AffineTransform" << AffineTransformObj;
  
  itk::AsinImageAdaptor<InputType,InputType>::Pointer AsinImageAdaptorObj =
    itk::AsinImageAdaptor<InputType,InputType>::New();
  std::cout << "------------AsinImageAdaptor" << AsinImageAdaptorObj;
  
  itk::AtanImageAdaptor<InputType,InputType>::Pointer AtanImageAdaptorObj =
    itk::AtanImageAdaptor<InputType,InputType>::New();
  std::cout << "------------AtanImageAdaptor" << AtanImageAdaptorObj;
  
  itk::AzimuthElevationToCartesianTransform<float,3>::Pointer AzimuthElevationToCartesianTransformObj =
    itk::AzimuthElevationToCartesianTransform<float,3>::New();
  std::cout << "------------AzimuthElevationToCartesianTransform" << AzimuthElevationToCartesianTransformObj;
  
  itk::BSplineDeformableTransform<float,3,3>::Pointer BSplineDeformableTransformObj =
    itk::BSplineDeformableTransform<float,3,3>::New();
  std::cout << "------------BSplineDeformableTransform" << BSplineDeformableTransformObj;
  
  itk::BSplineDerivativeKernelFunction<3>::Pointer BSplineDerivativeKernelFunctionObj =
    itk::BSplineDerivativeKernelFunction<3>::New();
  std::cout << "------------BSplineDerivativeKernelFunction" << BSplineDerivativeKernelFunctionObj;
  
  itk::BSplineInterpolationWeightFunction<float,2,3>::Pointer BSplineInterpolationWeightFunctionObj =
    itk::BSplineInterpolationWeightFunction<float,2,3>::New();
  std::cout << "------------BSplineInterpolationWeightFunction" << BSplineInterpolationWeightFunctionObj;
  
  itk::BSplineKernelFunction<3>::Pointer BSplineKernelFunctionObj =
    itk::BSplineKernelFunction<3>::New();
  std::cout << "------------BSplineKernelFunction" << BSplineKernelFunctionObj;
  
  itk::BinaryThresholdImageFunction<InputType>::Pointer BinaryThresholdImageFunctionObj =
    itk::BinaryThresholdImageFunction<InputType>::New();
  std::cout << "------------BinaryThresholdImageFunction" << BinaryThresholdImageFunctionObj;
  
  itk::BloxBoundaryPointImage<3>::Pointer BloxBoundaryPointImageObj =
    itk::BloxBoundaryPointImage<3>::New();
  std::cout << "------------BloxBoundaryPointImage" << BloxBoundaryPointImageObj;
  
  itk::BloxBoundaryProfileImage<3>::Pointer BloxBoundaryProfileImageObj =
    itk::BloxBoundaryProfileImage<3>::New();
  std::cout << "------------BloxBoundaryProfileImage" << BloxBoundaryProfileImageObj;
  
  itk::BloxCoreAtomImage<3>::Pointer BloxCoreAtomImageObj =
    itk::BloxCoreAtomImage<3>::New();
  std::cout << "------------BloxCoreAtomImage" << BloxCoreAtomImageObj;
  
  itk::BloxImage<BloxPixelType,3>::Pointer BloxImageObj =
    itk::BloxImage<BloxPixelType,3>::New();
  std::cout << "------------BloxImage" << BloxImageObj;
  
  itk::BoundingBox<unsigned long>::Pointer BoundingBoxObj =
    itk::BoundingBox<unsigned long>::New();
  std::cout << "------------BoundingBox" << BoundingBoxObj;
  
  itk::CenteredAffineTransform<double,3>::Pointer CenteredAffineTransformObj =
    itk::CenteredAffineTransform<double,3>::New();
  std::cout << "------------CenteredAffineTransform" << CenteredAffineTransformObj;
  
  itk::CenteredRigid2DTransform<double>::Pointer CenteredRigid2DTransformObj =
    itk::CenteredRigid2DTransform<double>::New();
  std::cout << "------------CenteredRigid2DTransform" << CenteredRigid2DTransformObj;
  
  itk::CenteredTransformInitializer<TransformType,InputType,InputType>::Pointer CenteredTransformInitializerObj =
    itk::CenteredTransformInitializer<TransformType,InputType,InputType>::New();
  std::cout << "------------CenteredTransformInitializer" << CenteredTransformInitializerObj;
  
  itk::CentralDifferenceImageFunction<InputType>::Pointer CentralDifferenceImageFunctionObj =
    itk::CentralDifferenceImageFunction<InputType>::New();
  std::cout << "------------CentralDifferenceImageFunction" << CentralDifferenceImageFunctionObj;
  
  itk::ColorTable<float>::Pointer ColorTableObj =
    itk::ColorTable<float>::New();
  std::cout << "------------ColorTable" << ColorTableObj;
  
  itk::ConicShellInteriorExteriorSpatialFunction<3>::Pointer ConicShellInteriorExteriorSpatialFunctionObj =
    itk::ConicShellInteriorExteriorSpatialFunction<3>::New();
  std::cout << "------------ConicShellInteriorExteriorSpatialFunction" << ConicShellInteriorExteriorSpatialFunctionObj;
  
  itk::CoreAtomImageToDistanceMatrixProcess<BloxCAImageType>::Pointer CoreAtomImageToDistanceMatrixProcessObj = 
    itk::CoreAtomImageToDistanceMatrixProcess<BloxCAImageType>::New();
  std::cout << "------------CoreAtomImageToDistanceMatrixProcess" << CoreAtomImageToDistanceMatrixProcessObj;

  itk::CosImageAdaptor<InputType,InputType>::Pointer CosImageAdaptorObj =
    itk::CosImageAdaptor<InputType,InputType>::New();
  std::cout << "------------CosImageAdaptor" << CosImageAdaptorObj;
  
  itk::DifferenceImageFilter<InputType,OutputType>::Pointer DifferenceImageFilterObj =
    itk::DifferenceImageFilter<InputType,OutputType>::New();
  std::cout << "------------DifferenceImageFilter" << DifferenceImageFilterObj;
  
  itk::DynamicLoader::Pointer DynamicLoaderObj =
    itk::DynamicLoader::New();
  std::cout << "------------DynamicLoader" << DynamicLoaderObj;
  
  itk::ElasticBodyReciprocalSplineKernelTransform<double,3>::Pointer ElasticBodyReciprocalSplineKernelTransformObj =
    itk::ElasticBodyReciprocalSplineKernelTransform<double,3>::New();
  std::cout << "------------ElasticBodyReciprocalSplineKernelTransform" << ElasticBodyReciprocalSplineKernelTransformObj;
  
  itk::ElasticBodySplineKernelTransform<double,3>::Pointer ElasticBodySplineKernelTransformObj =
    itk::ElasticBodySplineKernelTransform<double,3>::New();
  std::cout << "------------ElasticBodySplineKernelTransform" << ElasticBodySplineKernelTransformObj;
  
  itk::EllipsoidInteriorExteriorSpatialFunction<2,PointType>::Pointer EllipsoidInteriorExteriorSpatialFunctionObj =
    itk::EllipsoidInteriorExteriorSpatialFunction<2,PointType>::New();
  std::cout << "------------EllipsoidInteriorExteriorSpatialFunction" << EllipsoidInteriorExteriorSpatialFunctionObj;

  itk::EquivalencyTable::Pointer EquivalencyTableObj =
    itk::EquivalencyTable::New();
  std:: cout << "-------------EquivalencyTable " << EquivalencyTableObj;
  
  itk::Euler2DTransform<double>::Pointer Euler2DTransformObj =
    itk::Euler2DTransform<double>::New();
  std::cout << "------------Euler2DTransform" << Euler2DTransformObj;
  
  itk::Euler3DTransform<double>::Pointer Euler3DTransformObj =
    itk::Euler3DTransform<double>::New();
  std::cout << "------------Euler3DTransform" << Euler3DTransformObj;
  
  itk::ExpImageAdaptor<InputType,OutputType>::Pointer ExpImageAdaptorObj =
    itk::ExpImageAdaptor<InputType,OutputType>::New();
  std::cout << "------------ExpImageAdaptor" << ExpImageAdaptorObj;
  
  itk::ExpNegativeImageAdaptor<InputType,OutputType>::Pointer ExpNegativeImageAdaptorObj =
    itk::ExpNegativeImageAdaptor<InputType,OutputType>::New();
  std::cout << "------------ExpNegativeImageAdaptor" << ExpNegativeImageAdaptorObj;
  
  itk::FastMutexLock::Pointer FastMutexLockObj =
    itk::FastMutexLock::New();
  std::cout << "------------FastMutexLock" << FastMutexLockObj;
  
  itk::FileOutputWindow::Pointer FileOutputWindowObj =
    itk::FileOutputWindow::New();
  std::cout << "------------FileOutputWindow" << FileOutputWindowObj;

  itk::FiniteCylinderSpatialFunction<2,PointType>::Pointer FiniteCylinderSpatialFunctionObj = 
    itk::FiniteCylinderSpatialFunction<2,PointType>::New();
  std::cout << "------------FiniteCylinderSpatialFunction" << FiniteCylinderSpatialFunctionObj;
  
  itk::FrustumSpatialFunction<2,PointType>::Pointer FrustumSpatialFunctionObj =
    itk::FrustumSpatialFunction<2,PointType>::New();
  std::cout << "------------FrustumSpatialFunction" << FrustumSpatialFunctionObj;
  
  itk::GaussianKernelFunction::Pointer GaussianKernelFunctionObj =
    itk::GaussianKernelFunction::New();
  std::cout << "------------GaussianKernelFunction" << GaussianKernelFunctionObj;
  
  itk::GaussianSpatialFunction<float,2,PointType>::Pointer GaussianSpatialFunctionObj =
    itk::GaussianSpatialFunction<float,2,PointType>::New();
  std::cout << "------------GaussianSpatialFunction" << GaussianSpatialFunctionObj;
  
  itk::IdentityTransform<double,3>::Pointer IdentityTransformObj =
    itk::IdentityTransform<double,3>::New();
  std::cout << "------------IdentityTransform" << IdentityTransformObj;
  
  itk::Image<VectorType,2>::Pointer ImageObj =
    itk::Image<VectorType,2>::New();
  std::cout << "------------Image" << ImageObj;
  
  itk::ImageAdaptor<RGBImageType,RedAccessorType>::Pointer ImageAdaptorObj =
    itk::ImageAdaptor<RGBImageType,RedAccessorType>::New();
  std::cout << "------------ImageAdaptor" << ImageAdaptorObj;
  
  itk::ImageBase<3>::Pointer ImageBaseObj =
    itk::ImageBase<3>::New();
  std::cout << "------------ImageBase" << ImageBaseObj;
  
  itk::ImageRegionMultidimensionalSplitter<3>::Pointer ImageRegionMultidimensionalSplitterObj =
    itk::ImageRegionMultidimensionalSplitter<3>::New();
  std::cout << "------------ImageRegionMultidimensionalSplitter" << ImageRegionMultidimensionalSplitterObj;
  
  itk::ImageRegionSplitter<3>::Pointer ImageRegionSplitterObj =
    itk::ImageRegionSplitter<3>::New();
  std::cout << "------------ImageRegionSplitter" << ImageRegionSplitterObj;
  
  itk::ImportImageContainer<unsigned long,PointType>::Pointer ImportImageContainerObj =
    itk::ImportImageContainer<unsigned long,PointType>::New();
  std::cout << "------------ImportImageContainer" << ImportImageContainerObj;
  
  itk::KLMSegmentationBorder::Pointer KLMSegmentationBorderObj =
    itk::KLMSegmentationBorder::New();
  std::cout << "------------KLMSegmentationBorder" << KLMSegmentationBorderObj;
  
  itk::KLMSegmentationRegion::Pointer KLMSegmentationRegionObj =
    itk::KLMSegmentationRegion::New();
  std::cout << "------------KLMSegmentationRegion" << KLMSegmentationRegionObj;
  
  itk::KernelTransform<double,3>::Pointer KernelTransformObj =
    itk::KernelTransform<double,3>::New();
  std::cout << "------------KernelTransform" << KernelTransformObj;
  
  itk::LevelSetFunction<InputType>::Pointer LevelSetFunctionObj =
    itk::LevelSetFunction<InputType>::New();
  std::cout << "------------LevelSetFunction" << LevelSetFunctionObj;
  
  itk::LightProcessObject::Pointer LightProcessObjectObj =
    itk::LightProcessObject::New();
  std::cout << "------------LightProcessObject" << LightProcessObjectObj;
  
  itk::LinearInterpolateImageFunction<InputType,float>::Pointer LinearInterpolateImageFunctionObj =
    itk::LinearInterpolateImageFunction<InputType,float>::New();
  std::cout << "------------LinearInterpolateImageFunction" << LinearInterpolateImageFunctionObj;
  
  itk::Log10ImageAdaptor<InputType,OutputType>::Pointer Log10ImageAdaptorObj =
    itk::Log10ImageAdaptor<InputType,OutputType>::New();
  std::cout << "------------Log10ImageAdaptor" << Log10ImageAdaptorObj;
  
  itk::LogImageAdaptor<InputType,OutputType>::Pointer LogImageAdaptorObj =
    itk::LogImageAdaptor<InputType,OutputType>::New();
  std::cout << "------------LogImageAdaptor" << LogImageAdaptorObj;
  
  itk::MapContainer<unsigned long,PointType>::Pointer MapContainerObj =
    itk::MapContainer<unsigned long,PointType>::New();
  std::cout << "------------MapContainer" << MapContainerObj;
  
  itk::MatrixResizeableDataObject<double>::Pointer MatrixResizeableDataObjectObj = 
    itk::MatrixResizeableDataObject<double>::New();
  std::cout << "------------MatrixResizeableDataObject" << MatrixResizeableDataObjectObj;

  itk::MaximumDecisionRule::Pointer MaximumDecisionRuleObj =
    itk::MaximumDecisionRule::New();
  std::cout << "------------MaximumDecisionRule" << MaximumDecisionRuleObj;

  itk::MaximumRatioDecisionRule::Pointer MaximumRatioDecisionRuleObj =
    itk::MaximumRatioDecisionRule::New();
  std::cout << "------------MaximumRatioDecisionRule" << MaximumRatioDecisionRuleObj;

  itk::MeanImageFunction<InputType,float>::Pointer MeanImageFunctionObj =
    itk::MeanImageFunction<InputType,float>::New();
  std::cout << "------------MeanImageFunction" << MeanImageFunctionObj;
  
  itk::MedianImageFunction<InputType,float>::Pointer MedianImageFunctionObj =
    itk::MedianImageFunction<InputType,float>::New();
  std::cout << "------------MedianImageFunction" << MedianImageFunctionObj;
  
  itk::Mesh<PointType,2>::Pointer MeshObj =
    itk::Mesh<PointType,2>::New();
  std::cout << "------------Mesh" << MeshObj;
  
  itk::MeshSource<MeshType>::Pointer MeshSourceObj =
    itk::MeshSource<MeshType>::New();
  std::cout << "------------MeshSource" << MeshSourceObj;
  
  itk::MeshToMeshFilter<MeshType,MeshType>::Pointer MeshToMeshFilterObj =
    itk::MeshToMeshFilter<MeshType,MeshType>::New();
  std::cout << "------------MeshToMeshFilter" << MeshToMeshFilterObj;
#if 0
  itk::MetaDataObject<VectorImageType>::Pointer MetaDataObjectObj =
    itk::MetaDataObject<VectorImageType>::New();
  std::cout << "------------MetaDataObject" << MetaDataObjectObj;
#endif
  itk::MinimumDecisionRule::Pointer MinimumDecisionRuleObj =
    itk::MinimumDecisionRule::New();
  std::cout << "------------MinimumDecisionRule" << MinimumDecisionRuleObj;
  
  itk::MultiThreader::Pointer MultiThreaderObj =
    itk::MultiThreader::New();
  std::cout << "------------MultiThreader" << MultiThreaderObj;
  
  itk::MutexLock::Pointer MutexLockObj =
    itk::MutexLock::New();
  std::cout << "------------MutexLock" << MutexLockObj;
  
  itk::NearestNeighborInterpolateImageFunction<InputType,float>::Pointer NearestNeighborInterpolateImageFunctionObj =
    itk::NearestNeighborInterpolateImageFunction<InputType,float>::New();
  std::cout << "------------NearestNeighborInterpolateImageFunction" << NearestNeighborInterpolateImageFunctionObj;
  
  itk::NeighborhoodBinaryThresholdImageFunction<InputType,float>::Pointer NeighborhoodBinaryThresholdImageFunctionObj =
    itk::NeighborhoodBinaryThresholdImageFunction<InputType,float>::New();
  std::cout << "------------NeighborhoodBinaryThresholdImageFunction" << NeighborhoodBinaryThresholdImageFunctionObj;
  
  itk::NthElementImageAdaptor<InputType,PointType>::Pointer NthElementImageAdaptorObj =
    itk::NthElementImageAdaptor<InputType,PointType>::New();
  std::cout << "------------NthElementImageAdaptor" << NthElementImageAdaptorObj;
  
  itk::ObjectStore<TestObject>::Pointer ObjectStoreObj =
    itk::ObjectStore<TestObject>::New();
  std::cout << "------------ObjectStore" << ObjectStoreObj;
  
  itk::OneWayEquivalencyTable::Pointer OneWayEquivalencyTableObj =
    itk::OneWayEquivalencyTable::New();
  std:: cout << "-------------OneWayEquivalencyTable " << OneWayEquivalencyTableObj;

  itk::PointLocator<unsigned long, 3,float>::Pointer PointLocatorObj =
    itk::PointLocator<unsigned long, 3,float>::New();
  std::cout << "------------PointLocator" << PointLocatorObj;
  
  itk::PointSet<PointType,2>::Pointer PointSetObj =
    itk::PointSet<PointType,2>::New();
  std::cout << "------------PointSet" << PointSetObj;
  
  itk::ProgressAccumulator::Pointer ProgressAccumulatorObj =
    itk::ProgressAccumulator::New();
  std::cout << "------------ProgressAccumulator" << ProgressAccumulatorObj;
  
  itk::QuaternionRigidTransform<double>::Pointer QuaternionRigidTransformObj =
    itk::QuaternionRigidTransform<double>::New();
  std::cout << "------------QuaternionRigidTransform" << QuaternionRigidTransformObj;
  
  itk::RGBToVectorImageAdaptor<RGBImageType>::Pointer RGBToVectorImageAdaptorObj =
    itk::RGBToVectorImageAdaptor<RGBImageType>::New();
  std::cout << "------------RGBToVectorImageAdaptor" << RGBToVectorImageAdaptorObj;
  
  itk::Rigid2DTransform<double>::Pointer Rigid2DTransformObj =
    itk::Rigid2DTransform<double>::New();
  std::cout << "------------Rigid2DTransform" << Rigid2DTransformObj;
  
  itk::Rigid3DPerspectiveTransform<double>::Pointer Rigid3DPerspectiveTransformObj =
    itk::Rigid3DPerspectiveTransform<double>::New();
  std::cout << "------------Rigid3DPerspectiveTransform" << Rigid3DPerspectiveTransformObj;
  
  itk::Rigid3DTransform<double>::Pointer Rigid3DTransformObj =
    itk::Rigid3DTransform<double>::New();
  std::cout << "------------Rigid3DTransform" << Rigid3DTransformObj;
  
  itk::ScaleTransform<float,3>::Pointer ScaleTransformObj =
    itk::ScaleTransform<float,3>::New();
  std::cout << "------------ScaleTransform" << ScaleTransformObj;
  
  itk::SegmentationBorder::Pointer SegmentationBorderObj =
    itk::SegmentationBorder::New();
  std::cout << "------------SegmentationBorder" << SegmentationBorderObj;
  
  itk::SegmentationRegion::Pointer SegmentationRegionObj =
    itk::SegmentationRegion::New();
  std::cout << "------------SegmentationRegion" << SegmentationRegionObj;
  
  itk::Similarity2DTransform<double>::Pointer Similarity2DTransformObj =
    itk::Similarity2DTransform<double>::New();
  std::cout << "------------Similarity2DTransform" << Similarity2DTransformObj;
  
  itk::SinImageAdaptor<InputType,OutputType>::Pointer SinImageAdaptorObj =
    itk::SinImageAdaptor<InputType,OutputType>::New();
  std::cout << "------------SinImageAdaptor" << SinImageAdaptorObj;
  
  itk::SphereSpatialFunction<2,PointType>::Pointer SphereSpatialFunctionObj =
    itk::SphereSpatialFunction<2,PointType>::New();
  std::cout << "------------SphereSpatialFunction" << SphereSpatialFunctionObj;
  
  itk::SqrtImageAdaptor<InputType,OutputType>::Pointer SqrtImageAdaptorObj =
    itk::SqrtImageAdaptor<InputType,OutputType>::New();
  std::cout << "------------SqrtImageAdaptor" << SqrtImageAdaptorObj;
  
  itk::SymmetricEllipsoidInteriorExteriorSpatialFunction<>::Pointer SymmetricEllipsoidInteriorExteriorSpatialFunctionObj =
    itk::SymmetricEllipsoidInteriorExteriorSpatialFunction<>::New();
  std::cout << "------------SymmetricEllipsoidInteriorExteriorSpatialFunction" << SymmetricEllipsoidInteriorExteriorSpatialFunctionObj;
  
  itk::TanImageAdaptor<InputType,OutputType>::Pointer TanImageAdaptorObj =
    itk::TanImageAdaptor<InputType,OutputType>::New();
  std::cout << "------------TanImageAdaptor" << TanImageAdaptorObj;
  
  itk::TextOutput::Pointer TextOutputObj =
    itk::TextOutput::New();
  std::cout << "------------TextOutput" << TextOutputObj;
  
  itk::ThinPlateR2LogRSplineKernelTransform<double,3>::Pointer ThinPlateR2LogRSplineKernelTransformObj =
    itk::ThinPlateR2LogRSplineKernelTransform<double,3>::New();
  std::cout << "------------ThinPlateR2LogRSplineKernelTransform" << ThinPlateR2LogRSplineKernelTransformObj;
  
  itk::ThinPlateSplineKernelTransform<double,3>::Pointer ThinPlateSplineKernelTransformObj =
    itk::ThinPlateSplineKernelTransform<double,3>::New();
  std::cout << "------------ThinPlateSplineKernelTransform" << ThinPlateSplineKernelTransformObj;
  
  itk::TorusInteriorExteriorSpatialFunction<>::Pointer TorusInteriorExteriorSpatialFunctionObj =
    itk::TorusInteriorExteriorSpatialFunction<>::New();
  std::cout << "------------TorusInteriorExteriorSpatialFunction" << TorusInteriorExteriorSpatialFunctionObj;
  
#ifndef ITK_USE_OPTIMIZED_REGISTRATION_METHODS
  itk::Transform<double,3,3>::Pointer TransformObj =  // In the thread-safe version, this class is now abstract
    itk::Transform<double,3,3>::New();
  std::cout << "------------Transform" << TransformObj;
#endif
  
  itk::TranslationTransform<double,3>::Pointer TranslationTransformObj =
    itk::TranslationTransform<double,3>::New();
  std::cout << "------------TranslationTransform" << TranslationTransformObj;
  
  itk::ValarrayImageContainer<unsigned long,PointType>::Pointer ValarrayImageContainerObj =
    itk::ValarrayImageContainer<unsigned long,PointType>::New();
  std::cout << "------------ValarrayImageContainer" << ValarrayImageContainerObj;
  
  itk::VarianceImageFunction<InputType,float>::Pointer VarianceImageFunctionObj =
    itk::VarianceImageFunction<InputType,float>::New();
  std::cout << "------------VarianceImageFunction" << VarianceImageFunctionObj;
  
  itk::VectorContainer<unsigned long,PointType>::Pointer VectorContainerObj =
    itk::VectorContainer<unsigned long,PointType>::New();
  std::cout << "------------VectorContainer" << VectorContainerObj;
  
  itk::VectorImage< float, 3 >::Pointer VectorImageObj = 
    itk::VectorImage< float, 3 >::New();
  std::cout << "------------VectorImage" << VectorImageObj;

  itk::VectorLinearInterpolateImageFunction<VectorImageType,float>::Pointer VectorLinearInterpolateImageFunctionObj =
    itk::VectorLinearInterpolateImageFunction<VectorImageType,float>::New();
  std::cout << "------------VectorLinearInterpolateImageFunction" << VectorLinearInterpolateImageFunctionObj;
  
  itk::VectorToRGBImageAdaptor<VectorImageType>::Pointer VectorToRGBImageAdaptorObj =
    itk::VectorToRGBImageAdaptor<VectorImageType>::New();
  std::cout << "------------VectorToRGBImageAdaptor" << VectorToRGBImageAdaptorObj;
  
  itk::Version::Pointer VersionObj =
    itk::Version::New();
  std::cout << "------------Version" << VersionObj;
  
  itk::ScaleSkewVersor3DTransform<double>::Pointer ScaleSkewVersor3DTransformObj =
    itk::ScaleSkewVersor3DTransform<double>::New();
  std::cout << "------------ScaleSkewVersor3DTransform" << ScaleSkewVersor3DTransformObj;
  
  itk::VersorRigid3DTransform<double>::Pointer VersorRigid3DTransformObj =
    itk::VersorRigid3DTransform<double>::New();
  std::cout << "------------VersorRigid3DTransform" << VersorRigid3DTransformObj;
  
  itk::VersorTransform<double>::Pointer VersorTransformObj =
    itk::VersorTransform<double>::New();
  std::cout << "------------VersorTransform" << VersorTransformObj;
  
  itk::VolumeSplineKernelTransform<double,3>::Pointer VolumeSplineKernelTransformObj =
    itk::VolumeSplineKernelTransform<double,3>::New();
  std::cout << "------------VolumeSplineKernelTransform" << VolumeSplineKernelTransformObj;
  
  itk::XMLFileOutputWindow::Pointer XMLFileOutputWindowObj =
    itk::XMLFileOutputWindow::New();
  std::cout << "------------XMLFileOutputWindow" << XMLFileOutputWindowObj;

  
  return EXIT_SUCCESS;
}
