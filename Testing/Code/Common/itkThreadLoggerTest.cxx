/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkThreadLoggerTest.cxx
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
   //Warning about: identifier was truncated to '255' characters in the debug information (MVC6.0 Debug)
#pragma warning( disable : 4786 )
#endif

#include <iostream>
#include <fstream>
#include "itkStdStreamLogOutput.h"
#include "itkThreadLogger.h"



class LogTester
{
public:
  itk::Logger* GetLogger() { return m_Logger; }
  void SetLogger(itk::Logger* logger) { m_Logger = logger; }
  void log() {
    itkLogMacro( DEBUG, "DEBUG message by itkLogMacro\n" );
    itkLogMacro( INFO, "INFO message by itkLogMacro\n" );
    itkLogMacro( WARNING, "WARNING message by itkLogMacro\n" );
    itkLogMacro( CRITICAL, "CRITICAL message by itkLogMacro\n" );
    itkLogMacro( FATAL, "FATAL message by itkLogMacro\n" );
    itkLogMacro( MUSTFLUSH, "MUSTFLUSH message by itkLogMacro\n" );
  }
  static void logStatic(LogTester* tester)
  {
    itkLogMacroStatic( tester, DEBUG, "DEBUG message by itkLogMacroStatic\n" );
    itkLogMacroStatic( tester, INFO, "INFO message by itkLogMacroStatic\n" );
    itkLogMacroStatic( tester, WARNING, "WARNING message by itkLogMacroStatic\n" );
    itkLogMacroStatic( tester, CRITICAL, "CRITICAL message by itkLogMacroStatic\n" );
    itkLogMacroStatic( tester, FATAL, "FATAL message by itkLogMacroStatic\n" );
    itkLogMacroStatic( tester, MUSTFLUSH, "MUSTFLUSH message by itkLogMacroStatic\n" );
  }
private:
  itk::Logger* m_Logger;
};



int itkThreadLoggerTest( int argc, char * argv[] )
{
  try
    {
    if (argc < 2)
      {
      std::cout << "Usage: " << argv[0] << " logFilename" << std::endl;
      return EXIT_FAILURE;
      }
    

    // Create an ITK StdStreamLogOutputs
    itk::StdStreamLogOutput::Pointer coutput = itk::StdStreamLogOutput::New();
    itk::StdStreamLogOutput::Pointer foutput = itk::StdStreamLogOutput::New();
    coutput->SetStream(std::cout);
    std::ofstream fout(argv[1]);
    foutput->SetStream(fout);

    // Create an ITK ThreadLogger
    itk::ThreadLogger::Pointer logger = itk::ThreadLogger::New();

    std::cout << "Testing itk::ThreadLogger" << std::endl;

    // Setting the logger
    logger->SetName("org.itk.threadLogger");
    logger->SetPriorityLevel(itk::LoggerBase::INFO);
    logger->SetLevelForFlushing(itk::LoggerBase::CRITICAL);

    std::cout << "  Adding console and file stream LogOutputs" << std::endl;
    logger->AddLogOutput(coutput);
    logger->AddLogOutput(foutput);

    // Printing the logger's member variables
    std::cout << "  Name: " << logger->GetName() << std::endl;
    std::cout << "  Priority Level: " << logger->GetPriorityLevel() << std::endl;
    std::cout << "  Level For Flushing: " << logger->GetLevelForFlushing() << std::endl;

    // Logging by the itkLogMacro from a class with itk::ThreadLogger
    LogTester tester;
    tester.SetLogger(logger);
    tester.log();
    // Logging by the itkLogMacroStatic from a class with itk::ThreadLogger
    LogTester::logStatic(&tester);

    std::cout << "  The printed order of 'Messages ##' below might not be predictable because of multi-threaded logging" << std::endl;
    std::cout << "  But the logged messages will be in order." << std::endl;
    std::cout << "  Each line is an atom for synchronization." << std::endl;
    // Writing by the logger
    logger->Write(itk::LoggerBase::DEBUG, "This is the DEBUG message.\n");
    std::cout << "  Message #1" << std::endl;
    logger->Write(itk::LoggerBase::INFO, "This is the INFO message.\n");
    logger->Write(itk::LoggerBase::WARNING, "This is the WARNING message.\n");
    std::cout << "  Message #2" << std::endl;
    logger->Write(itk::LoggerBase::CRITICAL, "This is the CRITICAL message.\n");
    logger->Write(itk::LoggerBase::FATAL, "This is the FATAL message.\n");
    logger->Write(itk::LoggerBase::MUSTFLUSH, "This is the MUSTFLUSH message.\n");
    std::cout << "  Message #3" << std::endl;
    logger->Flush();
    std::cout << "  Flushing by the ThreadLogger is synchronized." << std::endl;
    }
  catch(...)
    {
    std::cerr << "Exception catched !!" << std::endl;
    return EXIT_FAILURE;
    }

  std::cout << "[PASSED]" << std::endl;
  return EXIT_SUCCESS;
}


