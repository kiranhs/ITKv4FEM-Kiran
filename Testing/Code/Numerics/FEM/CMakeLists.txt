IF( NOT ITK_DISABLE_CXX_TESTING )
IF(ITK_BINARY_DIR)
  IF(TCL_TCLSH)
    ADD_TEST(PrintSelf-FEM ${TCL_TCLSH}
             ${ITK_SOURCE_DIR}/Utilities/Dart/PrintSelfCheck.tcl  
             ${ITK_SOURCE_DIR}/Code/Numerics/FEM)
  ENDIF(TCL_TCLSH)
ENDIF(ITK_BINARY_DIR)



SET(FEM_TESTS ${CXX_TEST_PATH}/itkFEMTests)
SET(FEM_TESTS2 ${CXX_TEST_PATH}/itkFEMTests2)
## ##
SET(FEM_TESTS3 ${CXX_TEST_PATH}/itkFEMTests3)

SET(FEM_SRCS
#itkFEMElementTest.cxx
itkFEMExceptionTest.cxx
#itkFEMGenerateMeshTest.cxx
itkFEMElement2DMembraneTest.cxx
itkFEMElement3DMembraneTest.cxx
itkFEMElement2DStrainTest.cxx
)
SET(FEM_SRCS2
itkFEMElement2DQuadraticTriangularTest.cxx
itkFEMLinearSystemWrapperItpackTest.cxx
itkFEMLinearSystemWrapperItpackTest2.cxx
itkFEMLinearSystemWrapperVNLTest.cxx
itkFEMLinearSystemWrapperDenseVNLTest.cxx
itkFEMPArrayTest.cxx
)

## ##
SET(FEM_SRCS3
    itkFEMElement2DC0LinearTriangleStressTest.cxx
    itkFEMElement2DC0LinearQuadrilateralStrainItpackTest.cxx
    itkFEMElement2DC0LinearTriangleStrainTest.cxx
    itkFEMElement2DC0LinearTriangleMembraneTest.cxx
    itkFEMElement2DC0LinearQuadrilateralStressTest.cxx
    itkFEMElement2DC0LinearQuadrilateralStrainTest.cxx
    itkFEMElement2DC0LinearQuadrilateralMembraneTest.cxx
    itkFEMElement2DC0QuadraticTriangleStrainTest.cxx
    itkFEMElement2DC0QuadraticTriangleStressTest.cxx
    itkFEMElement2DC0LinearLineStressTest.cxx
    itkFEMElement2DC1BeamTest.cxx
    itkFEMElement3DC0LinearHexahedronStrainTest.cxx
    itkFEMElement3DC0LinearHexahedronMembraneTest.cxx
    itkFEMElement3DC0LinearTetrahedronStrainTest.cxx
    itkFEMElement3DC0LinearTetrahedronMembraneTest.cxx
    itkFEMLoadBCMFCTest.cxx
    itkFEMLoadBCMFCTestUser.cxx
    itkFEMLoadEdgeTest.cxx
    itkFEMLoadGravConstTest.cxx
    itkFEMLandmarkLoadImplementationTest.cxx
   # itkFEMRegistrationFilterTest.cxx
   # itkFEMSolverTest2D.cxx
    itkFEMSolverTest3D.cxx
    itkImageToRectilinearFEMObjectFilter2DTest.cxx
    itkImageToRectilinearFEMObjectFilter3DTest.cxx
    itkFEMElement2DTest.cxx
    itkFEMElement3DTest.cxx
    )
    
#ADD_TEST(itkFEMC0HexahedralElement-NodalLoads-BCs ${FEM_TESTS} itkFEMElementTest ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/hexa2.fem)
#ADD_TEST(itkFEMC0HexahedralElement-NoLoads ${FEM_TESTS} itkFEMElementTest ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/hexa3.fem)
#ADD_TEST(itkFEMC0HexahedralElement-GravityLoad-BCs ${FEM_TESTS} itkFEMElementTest ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/hexa4-grav.fem)
#ADD_TEST(itkFEMC0QuadElement-NodalLoads-BCs ${FEM_TESTS} itkFEMElementTest ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/quad2-small.fem)
#ADD_TEST(itkFEMC0QuadElement-Strain ${FEM_TESTS} itkFEMElementTest ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/quad2-strain.fem)
#ADD_TEST(itkFEMC0QuadElement-NoLoads ${FEM_TESTS} itkFEMElementTest ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/quad4.fem)
#ADD_TEST(itkFEMC0QuadElement-GravityLoad-BCs ${FEM_TESTS} itkFEMElementTest ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/quad6-grav.fem)
#ADD_TEST(itkFEMLoadLandmarkImplementation ${FEM_TESTS} itkFEMElementTest ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/quad-lm.fem)

# Edge loads are not re-implemented yet
#ADD_TEST(itkFEMC0QuadElement-EdgeLoad-BCs ${FEM_TESTS} itkFEMElementTest ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/quad_1_edge.fem)

#ADD_TEST(itkFEMC0TetrahedralElement-NodalLoads-BCs ${FEM_TESTS} itkFEMElementTest ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/tetra2.fem)
#ADD_TEST(itkFEMC0TetrahedralElement-NoLoads ${FEM_TESTS} itkFEMElementTest ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/tetra3.fem)
#ADD_TEST(itkFEMC0TetrahedralElement-GravityLoad-BCs ${FEM_TESTS} itkFEMElementTest ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/tetra4-grav.fem)
#ADD_TEST(itkFEMC0QuadElementTrapezoidalGeometry-NoLoads ${FEM_TESTS} itkFEMElementTest ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/trapezoid.fem)
#ADD_TEST(itkFEMC0TriangularElement-NodalLoads-BCs ${FEM_TESTS} itkFEMElementTest ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/tri2.fem)
#ADD_TEST(itkFEMC0TriangularElement-NoLoads ${FEM_TESTS} itkFEMElementTest ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/tri3.fem)
#ADD_TEST(itkFEMC0TriangularElement-Strain ${FEM_TESTS} itkFEMElementTest ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/tri3-e.fem)
#ADD_TEST(itkFEMC0TriangularElement-Quadratic ${FEM_TESTS} itkFEMElementTest ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/tri3-q.fem)
#ADD_TEST(itkFEMTruss ${FEM_TESTS} itkFEMElementTest ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/truss.fem)

ADD_TEST(itkFEMLinearSystemWrapperItpackTest  ${FEM_TESTS2} itkFEMLinearSystemWrapperItpackTest)

ADD_TEST(itkFEMLinearSystemWrapperItpackTest1  ${FEM_TESTS2} itkFEMLinearSystemWrapperItpackTest 0 )
ADD_TEST(itkFEMLinearSystemWrapperItpackTest2  ${FEM_TESTS2} itkFEMLinearSystemWrapperItpackTest 1 )
ADD_TEST(itkFEMLinearSystemWrapperItpackTest3  ${FEM_TESTS2} itkFEMLinearSystemWrapperItpackTest 2 )
ADD_TEST(itkFEMLinearSystemWrapperItpackTest4  ${FEM_TESTS2} itkFEMLinearSystemWrapperItpackTest 3 )
ADD_TEST(itkFEMLinearSystemWrapperItpackTest5  ${FEM_TESTS2} itkFEMLinearSystemWrapperItpackTest 4 )
ADD_TEST(itkFEMLinearSystemWrapperItpackTest6  ${FEM_TESTS2} itkFEMLinearSystemWrapperItpackTest 5 )
ADD_TEST(itkFEMLinearSystemWrapperItpackTest7  ${FEM_TESTS2} itkFEMLinearSystemWrapperItpackTest 6 )

ADD_TEST(itkFEMLinearSystemWrapperItpackTest8   ${FEM_TESTS2} itkFEMLinearSystemWrapperItpackTest2 0 )
ADD_TEST(itkFEMLinearSystemWrapperItpackTest9   ${FEM_TESTS2} itkFEMLinearSystemWrapperItpackTest2 1 )
ADD_TEST(itkFEMLinearSystemWrapperItpackTest10  ${FEM_TESTS2} itkFEMLinearSystemWrapperItpackTest2 2 )
ADD_TEST(itkFEMLinearSystemWrapperItpackTest11  ${FEM_TESTS2} itkFEMLinearSystemWrapperItpackTest2 3 )
ADD_TEST(itkFEMLinearSystemWrapperItpackTest12  ${FEM_TESTS2} itkFEMLinearSystemWrapperItpackTest2 4 )
ADD_TEST(itkFEMLinearSystemWrapperItpackTest13  ${FEM_TESTS2} itkFEMLinearSystemWrapperItpackTest2 5 )
ADD_TEST(itkFEMLinearSystemWrapperItpackTest14  ${FEM_TESTS2} itkFEMLinearSystemWrapperItpackTest2 6 )

ADD_TEST(itkFEMLinearSystemWrapperVNLTest  ${FEM_TESTS2} itkFEMLinearSystemWrapperVNLTest)
ADD_TEST(itkFEMLinearSystemWrapperDenseVNLTest  ${FEM_TESTS2} itkFEMLinearSystemWrapperDenseVNLTest)
ADD_TEST(itkFEMPArrayTest  ${FEM_TESTS2} itkFEMPArrayTest)

ADD_TEST(itkFEMExceptionTest ${FEM_TESTS} itkFEMExceptionTest)
#ADD_TEST(itkFEMGenerateMeshTest ${FEM_TESTS} itkFEMGenerateMeshTest)
ADD_TEST(itkFEMElement2DMembraneTest ${FEM_TESTS} itkFEMElement2DMembraneTest)
ADD_TEST(itkFEMElement3DMembraneTest ${FEM_TESTS} itkFEMElement3DMembraneTest)
ADD_TEST(itkFEMElement2DStrainTest ${FEM_TESTS} itkFEMElement2DStrainTest)
ADD_TEST(itkFEMElement2DQuadraticTriangularTest ${FEM_TESTS2} itkFEMElement2DQuadraticTriangularTest)

#ADD_TEST(itkFEMBar2DTest ${FEM_TESTS} itkFEMBar2DTest)

## ##
ADD_TEST(itkFEMElement2DC0LinearQuadrilateralStrainTest ${FEM_TESTS3}
         itkFEMElement2DC0LinearQuadrilateralStrainTest ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/2DC0LinearQuadrilateralStrainTest.meta
         ${ITK_TEST_OUTPUT_DIR}/2DC0LinearQuadrilateralStrainTestWrite.meta)

ADD_TEST(itkFEMElement2DC0LinearQuadrilateralStressTest ${FEM_TESTS3}
         itkFEMElement2DC0LinearQuadrilateralStressTest ${ITK_TEST_OUTPUT_DIR}/2DC0LinearQuadrilateralStressTestWrite.meta)

ADD_TEST(itkFEMElement2DC0LinearQuadrilateralMembraneTest ${FEM_TESTS3}
         itkFEMElement2DC0LinearQuadrilateralMembraneTest ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/2DC0LinearQuadrilateralMembraneTest.meta
         ${ITK_TEST_OUTPUT_DIR}/2DC0LinearQuadrilateralMembraneTestWrite.meta)

ADD_TEST(itkFEMElement2DC0LinearTriangleMembraneTest ${FEM_TESTS3}
         itkFEMElement2DC0LinearTriangleMembraneTest ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/2DC0LinearTriangleMembraneTest.meta
         ${ITK_TEST_OUTPUT_DIR}/2DC0LinearTriangleMembraneTestWrite.meta)
         
ADD_TEST(itkFEMElement2DC0LinearTriangleStressTest ${FEM_TESTS3}
         itkFEMElement2DC0LinearTriangleStressTest ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/2DC0LinearTriangleStressTest.meta

         ${ITK_TEST_OUTPUT_DIR}/2DC0LinearTriangleStressTestWrite.meta)

ADD_TEST(itkFEMElement2DC0LinearTriangleStrainTest ${FEM_TESTS3}
         itkFEMElement2DC0LinearTriangleStrainTest ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/2DC0LinearTriangleStrainTest.meta
         ${ITK_TEST_OUTPUT_DIR}/2DC0LinearTriangleStrainTestWrite.meta)

ADD_TEST(itkFEMElement2DC0QuadraticTriangleStrainTest ${FEM_TESTS3}
         itkFEMElement2DC0QuadraticTriangleStrainTest ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/2DC0QuadraticTriangleStrainTest.meta
         ${ITK_TEST_OUTPUT_DIR}/2DC0QuadraticTriangleStrainTestWrite.meta)

ADD_TEST(itkFEMElement2DC0QuadraticTriangleStressTest ${FEM_TESTS3}
         itkFEMElement2DC0QuadraticTriangleStressTest ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/2DC0QuadraticTriangleStressTest.meta
         ${ITK_TEST_OUTPUT_DIR}/2DC0QuadraticTriangleStressTestWrite.meta)

ADD_TEST(itkFEMElement2DC0LinearLineStressTest ${FEM_TESTS3}
         itkFEMElement2DC0LinearLineStressTest ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/2DC0LinearLineStressTest.meta
         ${ITK_TEST_OUTPUT_DIR}/2DC0LinearLineStressTestWrite.meta)

ADD_TEST(itkFEMElement2DC0LinearQuadrilateralStrainItpackTest ${FEM_TESTS3}
         itkFEMElement2DC0LinearQuadrilateralStrainItpackTest ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/2DC0LinearQuadrilateralStrainTest.meta
         ${ITK_TEST_OUTPUT_DIR}/2DC0LinearQuadrilateralStrainTestWrite.meta)

ADD_TEST(itkFEMElement2DC1BeamTest ${FEM_TESTS3}
         itkFEMElement2DC1BeamTest ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/2DC1BeamTest.meta
         ${ITK_TEST_OUTPUT_DIR}/2DC1BeamTestWrite.meta)

ADD_TEST(itkFEMElement3DC0LinearHexahedronStrainTest ${FEM_TESTS3}
         itkFEMElement3DC0LinearHexahedronStrainTest ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/3DC0LinearHexahedronStrainTest.meta
         ${ITK_TEST_OUTPUT_DIR}/3DC0LinearHexahedronStrainTestWrite.meta)

ADD_TEST(itkFEMElement3DC0LinearHexahedronMembraneTest ${FEM_TESTS3}
         itkFEMElement3DC0LinearHexahedronMembraneTest ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/3DC0LinearHexahedronMembraneTest.meta
         ${ITK_TEST_OUTPUT_DIR}/3DC0LinearHexahedronMembraneTestWrite.meta)

ADD_TEST(itkFEMElement3DC0LinearTetrahedronStrainTest ${FEM_TESTS3}
         itkFEMElement3DC0LinearTetrahedronStrainTest ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/3DC0LinearTetrahedronStrainTest.meta
         ${ITK_TEST_OUTPUT_DIR}/3DC0LinearTetrahedronStrainTestWrite.meta)

ADD_TEST(itkFEMElement3DC0LinearTetrahedronMembraneTest ${FEM_TESTS3}
         itkFEMElement3DC0LinearTetrahedronMembraneTest ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/3DC0LinearTetrahedronMembraneTest.meta
         ${ITK_TEST_OUTPUT_DIR}/3DC0LinearTetrahedronMembraneTestWrite.meta)

ADD_TEST(itkFEMLoadBCMFCTest ${FEM_TESTS3} 
         itkFEMLoadBCMFCTest ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/LoadBCMFCTest.meta
         ${ITK_TEST_OUTPUT_DIR}/LoadBCMFCTestWrite.meta)

ADD_TEST(itkFEMLoadBCMFCTestUser ${FEM_TESTS3} itkFEMLoadBCMFCTestUser)

ADD_TEST(itkFEMLoadEdgeTest ${FEM_TESTS3}
         itkFEMLoadEdgeTest ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/LoadEdgeTest.meta
         ${ITK_TEST_OUTPUT_DIR}/LoadEdgeTestWrite.meta)

ADD_TEST(itkFEMLoadGravConstTest ${FEM_TESTS3}
         itkFEMLoadGravConstTest ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/LoadGravConstTest.meta 
         ${ITK_TEST_OUTPUT_DIR}/LoadGravConstWrite.meta)

ADD_TEST(itkFEMLandmarkLoadImplementationTest ${FEM_TESTS3}
         itkFEMLandmarkLoadImplementationTest)

#ADD_TEST(itkFEMRegistrationFilterTest ${FEM_TESTS3}
#         itkFEMRegistrationFilterTest)

#ADD_TEST(itkFEMSolverTest2D ${FEM_TESTS3}
 #        itkFEMSolverTest2D ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/2DC0LinearTriangleStressTest.meta
  #       ${ITK_TEST_OUTPUT_DIR}/2DC0LinearTriangleStressTestWrite_NewSolver.meta)

ADD_TEST(itkFEMSolverTest3D ${FEM_TESTS3}
         itkFEMSolverTest3D ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/3DC0LinearHexahedronMembraneTest.meta
         ${ITK_TEST_OUTPUT_DIR}/3DC0LinearHexahedronMembraneTestWrite_NewSolver.meta)

ADD_TEST(itkImageToRectilinearFEMObjectFilter2DTest ${FEM_TESTS3}
         itkImageToRectilinearFEMObjectFilter2DTest ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/circle.png
         20 20 7 7 64 49 3 0 0 0 10 14 7 33 7 28 3 0 0 1 9 8 10 11 12 20 19 33 37 38 46 45)

ADD_TEST(itkImageToRectilinearFEMObjectFilter3DTest ${FEM_TESTS3}
         itkImageToRectilinearFEMObjectFilter3DTest ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/HeadMRVolumeWithDirection.mhd
         10 10 10 4 6 4 175 96 3 0 0 0 0 10 24 41.5692 0 33 122.497 100.172 0 3 0 0 1 6 5 35 36 41 40 10 12 13 18 17 47 48 53 52 33 46 47 52 51 81 82 87 86)


# 3D Element Tests
ADD_TEST(itkFEMC0HexahedralElement-NodalLoads-BCs-Meta ${FEM_TESTS3} itkFEMElement3DTest ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/hexa2.meta)
ADD_TEST(itkFEMC0HexahedralElement-NoLoads-Meta ${FEM_TESTS3} itkFEMElement3DTest ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/hexa3.meta)
ADD_TEST(itkFEMC0HexahedralElement-GravityLoad-BCs-Meta ${FEM_TESTS3} itkFEMElement3DTest ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/hexa4-grav.meta)
ADD_TEST(itkFEMC0TetrahedralElement-NodalLoads-BCs-Meta ${FEM_TESTS3} itkFEMElement3DTest ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/tetra2.meta)
ADD_TEST(itkFEMC0TetrahedralElement-NoLoads-Meta ${FEM_TESTS3} itkFEMElement3DTest ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/tetra3.meta)
ADD_TEST(itkFEMC0TetrahedralElement-GravityLoad-BCs-Meta ${FEM_TESTS3} itkFEMElement3DTest ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/tetra4-grav.meta)

# 2D Element Tests
ADD_TEST(itkFEMC0QuadElement-NodalLoads-BCs-Meta ${FEM_TESTS3} itkFEMElement2DTest ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/quad2-small.meta)
ADD_TEST(itkFEMC0QuadElement-Strain-Meta ${FEM_TESTS3} itkFEMElement2DTest ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/quad2-strain.meta)
ADD_TEST(itkFEMC0QuadElement-NoLoads-Meta ${FEM_TESTS3} itkFEMElement2DTest ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/quad4.meta)
ADD_TEST(itkFEMC0QuadElement-GravityLoad-BCs-Meta ${FEM_TESTS3} itkFEMElement2DTest ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/quad6-grav.meta)
ADD_TEST(itkFEMLoadLandmarkImplementation-Meta ${FEM_TESTS3} itkFEMElement2DTest ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/quad-lm.meta)
ADD_TEST(itkFEMC0TriangularElement-NodalLoads-BCs-Meta ${FEM_TESTS3} itkFEMElement2DTest ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/tri2.meta)
ADD_TEST(itkFEMC0TriangularElement-NoLoads-Meta ${FEM_TESTS3} itkFEMElement2DTest ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/tri3.meta)
ADD_TEST(itkFEMC0TriangularElement-Strain-Meta ${FEM_TESTS3} itkFEMElement2DTest ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/tri3-e.meta)
ADD_TEST(itkFEMC0TriangularElement-Quadratic-Meta ${FEM_TESTS3} itkFEMElement2DTest ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/tri3-q.meta)
ADD_TEST(itkFEMTruss-Meta ${FEM_TESTS3} itkFEMElement2DTest ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/truss.meta)
# Edge loads are not re-implemented yet - NEED TO Reimplement for ITKv4
#ADD_TEST(itkFEMC0QuadElement-EdgeLoad-BCs ${FEM_TESTS} itkFEMElementTest ${ITK_SOURCE_DIR}/Testing/Code/Numerics/FEM/Data/quad_1_edge.fem)

  
#ADD_TEST (itkFEMBar2DTest ${FEM_TESTS} itkFEMBar2DTest)

# NOTE: the two commands below are mutually exclusive!  Do not
# uncomment both at the same time!

# To run the FEM input menu-based test locally, uncomment the line below
#ADD_EXECUTABLE(itkFEMElementTestMenu itkFEMElementTest.cxx)
#TARGET_LINK_LIBRARIES(itkFEMElementTestMenu ITKNumerics ITKFEM)
#ADD_EXECUTABLE(itkFEMExceptionTest itkFEMExceptionTest.cxx)
#TARGET_LINK_LIBRARIES(itkFEMExceptionTest ITKFEM)
#ADD_EXECUTABLE(itkFEMGenerateMeshTest itkFEMGenerateMeshTest.cxx)
#TARGET_LINK_LIBRARIES(itkFEMGenerateMeshTest ITKNumerics ITKFEM)
#ADD_EXECUTABLE(itkFEMElement2DQuadraticTriangularTest itkFEMElement2DQuadraticTriangularTest.cxx)
#TARGET_LINK_LIBRARIES(itkFEMElement2DQuadraticTriangularTest ITKNumerics ITKFEM)

# To create the standard ITK test program, uncomment the line below
ADD_EXECUTABLE(itkFEMTests itkFEMTests.cxx ${FEM_SRCS})
IF(UNIX)
  TARGET_LINK_LIBRARIES (itkFEMTests -lm )
ENDIF(UNIX)
TARGET_LINK_LIBRARIES(itkFEMTests ITKNumerics ITKFEM)

ADD_EXECUTABLE(itkFEMTests2 itkFEMTests2.cxx ${FEM_SRCS2})
IF(UNIX)
  TARGET_LINK_LIBRARIES (itkFEMTests2 -lm )
ENDIF(UNIX)
TARGET_LINK_LIBRARIES(itkFEMTests2 ITKNumerics ITKFEM)

## ## ##
ADD_EXECUTABLE(itkFEMTests3 itkFEMTests3.cxx ${FEM_SRCS3})
IF(UNIX)
  TARGET_LINK_LIBRARIES (itkFEMTests3 -lm )
ENDIF(UNIX)
TARGET_LINK_LIBRARIES(itkFEMTests3 ITKNumerics ITKFEM)

ADD_EXECUTABLE(itkFEMFactoryTest itkFEMFactoryTest.cxx)
IF(UNIX)
  TARGET_LINK_LIBRARIES (itkFEMFactoryTest -lm )
ENDIF(UNIX)
TARGET_LINK_LIBRARIES(itkFEMFactoryTest ITKNumerics ITKFEM)

ENDIF( NOT ITK_DISABLE_CXX_TESTING )
