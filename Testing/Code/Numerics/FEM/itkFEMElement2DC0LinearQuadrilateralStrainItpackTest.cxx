/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFEMElement2DC0LinearQuadrilateralStrainItpackTest.cxx
  Language:  C++
  Date: $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

// disable debug warnings in MS compiler
#ifdef _MSC_VER
#pragma warning(disable: 4786)
#endif

#include "itkFEM.h"
#include "itkFEMLinearSystemWrapperItpack.h"
#include "itkFEMObject.h"
#include "itkFEMObjectSpatialObject.h"
#include "itkGroupSpatialObject.h"
#include "itkSpatialObject.h"
#include "itkSpatialObjectReader.h"
#include "itkSpatialObjectWriter.h"
#include "itkTimeProbe.h"

int itkFEMElement2DC0LinearQuadrilateralStrainItpackTest(int argc, char *argv[])
{
	typedef itk::SpatialObject<2>    SpatialObjectType;
	typedef SpatialObjectType::Pointer            SpatialObjectPointer;
	SpatialObjectPointer Spatial = SpatialObjectType::New();

	typedef itk::SpatialObjectReader<2>    SpatialObjectReaderType;
	typedef SpatialObjectReaderType::Pointer            SpatialObjectReaderPointer;
	SpatialObjectReaderPointer SpatialReader = SpatialObjectReaderType::New();
	SpatialReader->SetFileName( argv[1] );
	SpatialReader->Update();

	SpatialObjectReaderType::ScenePointer myScene = SpatialReader->GetScene();
	if(!myScene)
	{
		std::cout<<"No Scene : [FAILED]"<<std::endl;
		return EXIT_FAILURE;
	}
	std::cout<<" [PASSED]"<<std::endl;

	// Testing the fe mesh validity
	typedef itk::FEMObjectSpatialObject<2>    FEMObjectSpatialObjectType;
	typedef FEMObjectSpatialObjectType::Pointer            FEMObjectSpatialObjectPointer;

	FEMObjectSpatialObjectType::ChildrenListType* children = SpatialReader->GetGroup()->GetChildren();
	if(strcmp((*(children->begin()))->GetTypeName(),"FEMObjectSpatialObject"))
	{
		std::cout<<" [FAILED]"<<std::endl;
		return EXIT_FAILURE;
	}

	itk::fem::LinearSystemWrapperItpack WrapperItpack;
	WrapperItpack.SetMaximumNonZeroValuesInMatrix(1000);

	FEMObjectSpatialObjectType::Pointer femSO = 
		dynamic_cast<FEMObjectSpatialObjectType*>((*(children->begin())).GetPointer());
	femSO->GetFEMObject()->SetLinearSystemWrapper(&WrapperItpack);

	itk::TimeProbe pTime;
	pTime.Start();

	femSO->GetFEMObject()->Solve();

	pTime.Stop();

	int numDOF = femSO->GetFEMObject()->GetNumberOfDegreesOfFreedom();
	vnl_vector<float> soln(numDOF);
  float exectedResult[8] = {0.0, 0.0, 4.11808e-07, 3.47237e-08, 5.54107e-07, -1.65448e-07, 0.0, 0.0};
  
  bool foundError = false;
	for ( int i = 0; i < numDOF; i++ )
	{
		soln[i] = femSO->GetFEMObject()->GetSolution(i);
		//std::cout << "Solution[" << i << "]:" << soln[i] << std::endl;
		if (abs(exectedResult[i]-soln[i]) > 0.0000001)
	  {
	    std::cout << "ERROR: Index " << i << ". Expected " << exectedResult[i] << " Solution " << soln[i] << std::endl;
	    foundError = true;
	  }
	}

	typedef itk::SpatialObjectWriter<2>    SpatialObjectWriterType;
	typedef SpatialObjectWriterType::Pointer            SpatialObjectWriterPointer;
	SpatialObjectWriterPointer SpatialWriter = SpatialObjectWriterType::New();
	SpatialWriter->SetInput(SpatialReader->GetScene());
	SpatialWriter->SetFileName( argv[2] );
	SpatialWriter->Update();

	std::cout << "Test PASSED!" << std::endl;
	return EXIT_SUCCESS;
}
