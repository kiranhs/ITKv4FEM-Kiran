/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#include "itkFEMElementTest.h"
#include "itksys/SystemTools.hxx"

int itkFEMElement2DTest(int ac, char *av[])
{
  
  

  // Solvers being tested
  int numsolvers = 3;
  int currsolver;
  int s;

  // Output comments
  char comment = DEFAULT_COMMENT;
  
  
  typedef itk::SpatialObjectReader<2>         SpatialObjectReaderType;
	typedef SpatialObjectReaderType::Pointer    SpatialObjectReaderPointer;
	SpatialObjectReaderPointer SpatialReader = SpatialObjectReaderType::New();
	SpatialReader->SetFileName( argv[1] );
	SpatialReader->Update();

	SpatialObjectReaderType::ScenePointer myScene = SpatialReader->GetScene();
	
	std::cout<<"Scene Test: "<<std::endl;
	if(!myScene)
	{
		std::cout<<"[FAILED]"<<std::endl;
		return EXIT_FAILURE;
	}
	else
	{
	  std::cout<<"[PASSED]"<<std::endl;
	}

	// Testing the fe mesh validity
	typedef itk::FEMObjectSpatialObject<2>          FEMObjectSpatialObjectType;
	typedef FEMObjectSpatialObjectType::Pointer     FEMObjectSpatialObjectPointer;

	FEMObjectSpatialObjectType::ChildrenListType* children = SpatialReader->GetGroup()->GetChildren();
	
	std::cout<<"FEM Spatial Object Test: "<<std::endl;
	if(strcmp((*(children->begin()))->GetTypeName(),"FEMObjectSpatialObject"))
	{
		std::cout<<"[FAILED]"<<std::endl;
		return EXIT_FAILURE;
	}
	else
	{
	  std::cout<<"[PASSED]"<<std::endl;
	}

	FEMObjectSpatialObjectType::Pointer femSO = 
		dynamic_cast<FEMObjectSpatialObjectType*>((*(children->begin())).GetPointer());

	femSO->GetFEMObject()->FinalizeMesh();

 
  
  double *expectedSolution = NULL;
  double tolerance;

  
  //Run the Solver using all of the available numeric solvers
  
  try
    {
    // Declare the FEM solver & associated input stream and read the
    // input file

    typedef itk::fem::Solver1<2>    Solver2DType;
    Solver2DType::Pointer solver = Solver2DType::New();
    solver->SetInput( femSO->GetFEMObject() );
    
    // Declare and initialize linear system wrapper objects

    itk::fem::LinearSystemWrapperDenseVNL lsw_dvnl;
    itk::fem::LinearSystemWrapperItpack   lsw_itpack;
    itk::fem::LinearSystemWrapperVNL      lsw_vnl;

    for ( s = 0; s < numsolvers; s++ )
      {
      if ( s == 2 )
        {
        // Itpack
        std::cout << std::endl << comment << ">>>>>Using LinearSystemWrapperItpack" << std::endl;
        lsw_itpack.SetMaximumNonZeroValuesInMatrix(1000);
        solver->SetLinearSystemWrapper(&lsw_itpack);
        }
      else if ( s == 1 )
        {
        // Dense VNL
        std::cout << std::endl << comment << ">>>>>Using LinearSystemWrapperDenseVNL" << std::endl;
        solver->SetLinearSystemWrapper(&lsw_dvnl);
        }
      else
        {
        // Sparse VNL - default
        std::cout << std::endl << comment << ">>>>>Using LinearSystemWrapperVNL" << std::endl;
        solver->SetLinearSystemWrapper(&lsw_vnl);
        }

      solver->Update();
      

        if (modelFile == "quad2-small.fem")
        {
          tolerance=10e-10;
          double quad2smallExpectedSolution[8] = {
            0, 0,
            2.97334e-07, -1.20555e-06,
            1.944e-06, -1.32333e-06,
          0, 0};
          expectedSolution = &(quad2smallExpectedSolution[0]);
        }
        else if (modelFile == "quad2-strain.fem")
        {
          tolerance=10e-10;
          double quad2strainExpectedSolution[8] = {
            0, 0,
            2.56204e-07, -1.02482e-06,
            1.67956e-06, -1.19562e-06,
          0, 0};
          expectedSolution = &(quad2strainExpectedSolution[0]);
        }
        else if (modelFile == "quad4.fem")
        {
          tolerance=10e-10;
          double quad4ExpectedSolution[8] = {
            0, 0,
            0, 0,
            0, 0,
          0, 0};
          expectedSolution = &(quad4ExpectedSolution[0]);
        }
        else if (modelFile == "quad6-grav.fem")
        {
          tolerance=10e-10;
          double quad6gravExpectedSolution[8] = {
            0, 0,
            0, 0,
            -5.32164e-08, 1.59649e-07,
          5.32164e-08, 1.59649e-07};
          expectedSolution = &(quad6gravExpectedSolution[0]);
        }
        else if (modelFile == "quad-lm.fem")
        {
          tolerance=10e-7;
          double quadlmExpectedSolution[8] = {
            0, 0,
            -8.76093e-05, -0.0135944,
            -0.00420457, 0.00477804,
            -0.0163679, -0.0360446,
          };
          expectedSolution = &(quadlmExpectedSolution[0]);
        }
        else if (modelFile == "trapezoid.fem")
        {
          tolerance=10e-10;
          double trapezoidExpectedSolution[8] = {
            0, 0,
            0, 0,
            0, 0,
          0, 0};
          expectedSolution = &(trapezoidExpectedSolution[0]);
        }
        else if (modelFile == "tri2.fem")
        {
          tolerance=10e-6;
          double tri2ExpectedSolution[8] = {
            0, 0,
            9.86667e-07, -2.028e-05,
            -9.76e-06, -5.67867e-05,
          -2.87733e-05, -9.68267e-05};
          expectedSolution = &(tri2ExpectedSolution[0]);
          
        }
        else if (modelFile == "tri3.fem")
        {
          tolerance=10e-10;
          double tri3ExpectedSolution[6] = {
            0, 0,
            0, 0,
          0, 0};
          expectedSolution = &(tri3ExpectedSolution[0]);
        }
        else if (modelFile == "tri3-e.fem")
        {
          tolerance=10e-10;
          double tri3eExpectedSolution[6] = {
            0, 0,
            0, 0,
          0, 0};
          expectedSolution = &(tri3eExpectedSolution[0]);
        }
        else if (modelFile == "tri3-q.fem")
        {
          tolerance=10e-9;
          double tri3qExpectedSolution[12] = {
            0, 0,
            -3.315e-07, 1.57527e-06,
            4.98323e-06, 7.36775e-07,
            -5.3625e-08, 2.18676e-06,
            8.32488e-07, 1.04065e-06,
          5.22113e-07, 2.42889e-06};
          expectedSolution = &(tri3qExpectedSolution[0]);
        }
        else if (modelFile == "truss.fem")
        {
          tolerance=10e-7;
          double trussExpectedSolution[11] = {
            0, 0, -8.36189e-08,
            0.00010695, -2.22984e-07, 0,
            -0.0298698, 0, 8.36189e-08,
          0.0691513, -0.0940715};
          expectedSolution = &(trussExpectedSolution[0]);
        }
        else
        {
          std::cout << "WARNING: Unknown solution for this model, " << modelFile << std::endl;
        } 
        
        
#if DEBUG_FEM_TESTS
      PrintK(S, s, comment);
      PrintF(S, s, comment);
      PrintNodalCoordinates(S, s, comment);
      PrintU(S, s, comment);
      
      if (expectedSolution != NULL)
      {
        bool foundError = CheckDisplacements(S, s, comment, expectedSolution, tolerance);
        if ( foundError )
        {
          //return EXIT_FAILURE;
        }
      }
#endif

      std::cout << comment << "Done" << std::endl;

      std::cout << comment << "Test PASSED" << std::endl;
      }
    }
  catch ( ::itk::ExceptionObject & err )
    {
    std::cerr << "ITK exception detected: "  << err;
    std::cout << "Test FAILED" << std::endl;

    return EXIT_FAILURE;
    }

  
  
  delete[] fname;
  
  
  return EXIT_SUCCESS;
}

#if DEBUG_FEM_TESTS

void PrintK(itk::fem::Solver & S, int s, char)
// Print K - the global stiffness matrix
{
  itk::fem::LinearSystemWrapper::Pointer lsw = S.GetLinearSystemWrapper();

  std::cout << std::endl << "k" << s << "=[";
  for ( unsigned int j = 0; j < lsw->GetSystemOrder(); j++ )
    {
    if ( IDL_OUTPUT ) { std::cout << " ["; }
    for ( unsigned int k = 0; k < lsw->GetSystemOrder(); k++ )
      {
      if ( k > 0 ) { std::cout << ",  "; }
      std::cout << lsw->GetMatrixValue(j, k);
      }
    if ( IDL_OUTPUT )
      {
      if ( j < lsw->GetSystemOrder() - 1 ) { std::cout << " ], $" << std::endl; }
      else  { std::cout << "]"; }
      }
    else if ( MATLAB_OUTPUT )
      {
      std::cout << std::endl;
      }
    }
  std::cout << "];" << std::endl;
}

void PrintF(itk::fem::Solver & S, int s, char)
// Print F - the global load vector
{
  itk::fem::LinearSystemWrapper::Pointer lsw = S.GetLinearSystemWrapper();

  std::cout << std::endl << "f" << s << "=[";
  for ( unsigned int j = 0; j < lsw->GetSystemOrder(); j++ )
    {
    if ( j > 0 ) { std::cout << ",  "; }
    std::cout << lsw->GetVectorValue(j);
    }
  std::cout << "];" << std::endl;
}

void PrintNodalCoordinates(itk::fem::Solver & S, int w, char comment)
// Print the nodal coordinates
{
  std::cout << std::endl << comment << "Nodal coordinates: " << std::endl;

  std::cout << "xyz" << w << "=[";
  // changes made - kiran
  //for ( itk::fem::Solver::NodeArray::iterator n = S.node.begin(); n !=
  // S.node.end(); n++) {
  for ( itk::fem::Solver::NodeArray::iterator n = S.GetNodeArray().begin();
        n != S.GetNodeArray().end(); n++ )
    {
    // changes made - kiran
    if ( IDL_OUTPUT ) { std::cout << " ["; }
    // FIXME: this will generate errors in IDL - needs to be comma-delimited
    std::cout << ( *n )->GetCoordinates();
    if ( IDL_OUTPUT )
      {
      // changes made - kiran
      //if ((n+1) != S.node.end()) { std::cout << " ], $" << std::endl; }
      if ( ( n + 1 ) != S.GetNodeArray().end() ) { std::cout << " ], $" << std::endl; }
      // changes made - kiran
      else { std::cout << "]"; }
      }
    else if ( MATLAB_OUTPUT )
      {
      std::cout << std::endl;
      }
    }
  std::cout << "];" << std::endl;
}

void PrintU(itk::fem::Solver & S, int s, char comment)
// Prints the components of the problem for debugging/reporting purposes
{
  std::cout << std::endl << comment << "Displacements: " << std::endl;
  std::cout << "u" << s << "=[";
  // changes made - kiran
  //for( ::itk::fem::Solver::NodeArray::iterator n = S.node.begin();
  // n!=S.node.end(); n++) {
  for ( ::itk::fem::Solver::NodeArray::iterator n = S.GetNodeArray().begin(); n != S.GetNodeArray().end(); n++ )
    {
    // changes made - kiran
    if ( IDL_OUTPUT ) { std::cout << " ["; }
    /** For each DOF in the node... */
    for ( unsigned int d = 0, dof;
          ( dof = ( *n )->GetDegreeOfFreedom(d) ) != ::itk::fem::Element::InvalidDegreeOfFreedomID;
          d++ )
      {
      if ( d > 0 && d != ::itk::fem::Element::InvalidDegreeOfFreedomID ) { std::cout << ", "; }
      std::cout << S.GetSolution(dof);
      }
    if ( IDL_OUTPUT )
      {
      // changes made - kiran
      //if ((n+1) != S.node.end()) { std::cout << " ], $" << std::endl; }
      if ( ( n + 1 ) != S.GetNodeArray().end() ) { std::cout << " ], $" << std::endl; }
      // changes made - kiran
      else { std::cout << "]"; }
      }
    else if ( MATLAB_OUTPUT )
      {
      std::cout << std::endl;
      }
    }
  std::cout << "];" << std::endl;
}

bool CheckDisplacements(itk::fem::Solver & S, int s, char comment, double *expectedResults, double tolerance)
// Prints the components of the problem for debugging/reporting purposes
{
  std::cout << std::endl << comment << "Check Displacements: " << std::endl;
  int index = 0;
  bool foundError = false;
  
  std::cout << std::endl << comment << "NodeArray: " << std::endl;
  
  for ( ::itk::fem::Solver::NodeArray::iterator n = S.GetNodeArray().begin(); n != S.GetNodeArray().end(); n++ )
  {
    for ( unsigned int d = 0, dof;
         ( dof = ( *n )->GetDegreeOfFreedom(d) ) != ::itk::fem::Element::InvalidDegreeOfFreedomID;
         d++ )
    {
      double result = S.GetSolution(dof);
      if (fabs(result - expectedResults[index]) > tolerance)
      {
        std::cout << "Error: Result (" << result << ") expected (" << expectedResults[index] << ") with tolerance (" << tolerance << ")" << std::endl;
        foundError = true;
      }
      index++;
    }
  }
  
  return foundError;
}


#endif
