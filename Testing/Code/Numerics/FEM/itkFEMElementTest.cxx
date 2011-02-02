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

int itkFEMElementTest(int ac, char *av[])
{
  // NOTE TO THE USER: if you would like to run the menu-based test,
  // you will need to change the two paths below to point to the
  // appropriate directory in your ITK tree from your executable
  // folder.

  // Filename containing list of possible input files
  char listloc[] = "../../Insight/Testing/Data/Input/FEM/input-list";

  // Path to input files
  char filepath[] = "../../Insight/Testing/Data/Input/FEM/";

  // File input stream
  std::ifstream f;

  // Storage for list of or user-specified input file(s)
  char **filelist;
  char   buffer[80] = { '\0' };
  int    numfiles = 0;
  char * fname;

  // Solvers being tested
  int numsolvers = 3;
  int currsolver;
  int s;

  // Output comments
  char comment;

  if ( MATLAB_OUTPUT ) { comment = MATLAB_COMMENT; }
  else if ( IDL_OUTPUT )
    {
    comment = IDL_COMMENT;
    }
  else { comment = DEFAULT_COMMENT; }

  // This test can be run in two different ways:
  //    1. by specifying an input file as a run-time argument
  //    2. by using the built-in menu of input files

  if ( ac < 2 )
  // Display the menu
    {
    std::cout << "Loading menu..." << std::endl;

    f.open(listloc, std::ios::in);
    if ( !f )
      {
      std::cout << "ERROR: null file handle - couldn't read input file list" << std::endl;
      std::cout << "Test FAILED" << std::endl;
      return EXIT_FAILURE;
      }

    f >> numfiles;
    filelist = new char *[numfiles];
    for ( int k = 0; k < numfiles; k++ )
      {
      f >> buffer;
      filelist[k] = new char[strlen(buffer) + 1];
      strcpy(filelist[k], buffer);
      }
    f.close();

    // Prompt the user to select a problem
    int ch = -1;
    while ( ch < 0 || ch >= numfiles )
      {
      for ( int j = 0; j < numfiles; j++ )
        {
        std::cout << j << ": " << filelist[j] << std::endl;
        }
      //std::cout << std::endl << "NOTE: some of these problems follow an older
      // data file" << std::endl;
      //std::cout << "format, and have not yet been updated.  They may end in
      // \"Abort\"." << std::endl;
      std::cout << std::endl << "Select an FEM problem to solve:  ";
      std::cin >> ch;
      }

    // Print the name of the selected problem
    std::cout << std::endl << comment << "FEM Problem: " << filelist[ch] << std::endl;

    // Construct the file name appropriately from the list
    fname = new char[strlen(filepath) + strlen(filelist[ch]) + 5];
    strcpy(fname, filepath);
    strcat(fname, filelist[ch]);
    }
  // Accept a user-specified file
  else
    {
    std::cout << "User-specified file..." << std::endl;

    fname = new char[strlen(av[1]) + 5];
    strcpy(fname, av[1]);

    // Print the name of the user-specified problem
    std::cout << std::endl << comment << "FEM Input: " << fname << std::endl;

    // Check if a solver is specified as well
    if ( ac == 3 )
      {
      currsolver = *av[2];
      std::cout << "currsolver = " << currsolver << std::endl;
      }
    }

  // Open a file handle & associate it with the input file
  f.open(fname, std::ios::binary);
  if ( !f )
    {
    std::cout << "ERROR: null file handle...terminating." << std::endl;
    std::cout << "Test FAILED" << std::endl;
    return EXIT_FAILURE;
    }

  try
    {
    // Declare the FEM solver & associated input stream and read the
    // input file

    std::cout << comment << "Solver()" << std::endl;
    itk::fem::Solver S;
    std::cout << comment << "Read()" << std::endl;
    S.Read(f);
    f.close();
    delete[] fname;

    // Call the appropriate sequence of Solver methods to solve the
    // problem

    std::cout << comment << "GenerateGFN()" << std::endl;
    S.GenerateGFN();          // Generate global freedom numbers for system DOFs

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
        S.SetLinearSystemWrapper(&lsw_itpack);
        }
      else if ( s == 1 )
        {
        // Dense VNL
        std::cout << std::endl << comment << ">>>>>Using LinearSystemWrapperDenseVNL" << std::endl;
        S.SetLinearSystemWrapper(&lsw_dvnl);
        }
      else
        {
        // Sparse VNL - default
        std::cout << std::endl << comment << ">>>>>Using LinearSystemWrapperVNL" << std::endl;
        S.SetLinearSystemWrapper(&lsw_vnl);
        }

      std::cout << comment << "AssembleK()" << std::endl;
      S.AssembleK();            // Assemble the global stiffness matrix K

      std::cout << comment << "DecomposeK()" << std::endl;
      S.DecomposeK();           // Invert K

      std::cout << comment << "AssembleF()" << std::endl;
      S.AssembleF();            // Assemble the global load vector F

      std::cout << comment << "Solver::Solve()" << std::endl;
      S.Solve();                // Solve the system Ku=F for u

#if DEBUG_FEM_TESTS
      PrintK(S, s, comment);
      PrintF(S, s, comment);
      PrintNodalCoordinates(S, s, comment);
      PrintU(S, s, comment);
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

#endif
