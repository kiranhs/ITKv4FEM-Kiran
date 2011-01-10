/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkSystemInformationTest.cxx
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

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <Code/Common/itkSystemInformationTest.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>

#if defined(ITK_BINARY_DIR)
# define ITK_SYSTEM_INFORMATION_DIR ITK_BINARY_DIR
#else
# define ITK_SYSTEM_INFORMATION_DIR ITKTesting_BINARY_DIR
#endif

// Construct the name of the notes file.
#define ITK_SYSTEM_INFORMATION_NOTES \
  ITK_SYSTEM_INFORMATION_DIR "/Testing/HTML/TestingResults/Sites/" \
  ITKTesting_SITE "/" ITKTesting_BUILD_NAME "/BuildNameNotes.xml"

std::string itkGetCurrentDateTime(const char* format)
{
  char buf[1024];
  time_t t;
  time(&t);
  strftime(buf, sizeof(buf), format, localtime(&t));
  return buf;
}


void itkSystemInformationPrintFile(const char* name, std::ostream& os,
                                   bool note=false )
{
  if (!note)
    {
    os << "================================================================\n";
    }
  struct stat fs;
  if(stat(name, &fs) != 0)
    {
    os << "The file \"" << name << "\" does not exist.\n";
    return;
    }

#ifdef _WIN32
  std::ifstream fin(name, std::ios::in);
#else
  std::ifstream fin(name, std::ios::in);
#endif

  if(fin)
    {
    if (!note)
      {
      os << "Contents of \"" << name << "\":\n";
      os << "----------------------------------------------------------------\n";
      }
    const int bufferSize = 4096;
    char bufferIn[bufferSize];
    char bufferOut[6*bufferSize]; // worst case scenario
    // This copy loop is very sensitive on certain platforms with
    // slightly broken stream libraries (like HPUX).  Normally, it is
    // incorrect to not check the error condition on the fin.read()
    // before using the data, but the fin.gcount() will be zero if an
    // error occurred.  Therefore, the loop should be safe everywhere.
    while(fin)
      {
      fin.read(bufferIn, bufferSize);
      if(fin.gcount())
        {
        // convert buffer to an XML safe form
        const char *s = bufferIn;
        char *x = bufferOut;
        *x = '\0';
        for (int i = 0; i < static_cast<int>(fin.gcount()); i++)
          {
          // replace all special characters
          switch (*s)
            {
            case '&':
              strcat(x, "&amp;"); x += 5;
              break;
            case '"':
              strcat(x, "&quot;"); x += 6;
              break;
            case '\'':
              strcat(x, "&apos;"); x += 6;
              break;
            case '<':
              strcat(x, "&lt;"); x += 4;
              break;
            case '>':
              strcat(x, "&gt;"); x += 4;
              break;
            default:
              *x = *s; x++;
              *x = '\0'; // explicitly terminate the new string
            }
          s++;
          }
        os.write(bufferOut, x - bufferOut);
        }
      }
    os.flush();
    }
  else
    {
    os << "Error opening \"" << name << "\" for reading.\n";
    }
}

int main(int,char *[])
{
  const char* files[] =
    {
      ITK_SYSTEM_INFORMATION_DIR "/CMakeCache.txt", 
      ITK_SYSTEM_INFORMATION_DIR "/itkConfigure.h",
      ITK_SYSTEM_INFORMATION_DIR "/CMakeFiles/CMakeOutput.log",
      ITK_SYSTEM_INFORMATION_DIR "/CMakeFiles/CMakeError.log",
      ITK_SYSTEM_INFORMATION_DIR "/CMakeOutput.log",
      ITK_SYSTEM_INFORMATION_DIR "/CMakeError.log",
      ITK_SYSTEM_INFORMATION_DIR "/ITKBuildSettings.cmake",
      ITK_SYSTEM_INFORMATION_DIR "/ITKLibraryDepends.cmake",
      ITK_SYSTEM_INFORMATION_DIR "/ITKConfig.cmake",
      0
    };
  
  const char** f;  
  for(f = files; *f; ++f)
    {
    itkSystemInformationPrintFile(*f, std::cout);
    }
  
  std::ofstream outf(ITK_SYSTEM_INFORMATION_NOTES, std::ios::out);
  if(outf)
    {
    std::cout << "Also writing this information to file " << ITK_SYSTEM_INFORMATION_NOTES << "\n";
  
    outf << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
    outf << "<Site BuildName=\"" << ITKTesting_BUILD_NAME << "\"  Name=\""
         << ITKTesting_SITE << "\">" << std::endl;
    outf << "<BuildNameNotes>" << std::endl;
    for(f = files; *f; ++f)
      {
      outf << "<Note Name=\"" << *f << "\">" << std::endl;
      outf << "<DateTime>"
           << itkGetCurrentDateTime("%a %b %d %Y %H:%M:%S %Z")
           << "</DateTime>" << std::endl;
      outf << "<Text>" << std::endl;
    
      itkSystemInformationPrintFile(*f, outf, true);

      outf << "</Text>" << std::endl;
      outf << "</Note>" << std::endl;
      }
    
    outf << "</BuildNameNotes>" << std::endl;
    outf << "</Site>" << std::endl;
    outf.close();
    }
  else
    {
    std::cerr << "Error writing this information to file " << ITK_SYSTEM_INFORMATION_NOTES << "\n";
    return EXIT_FAILURE;
    }
  
  return EXIT_SUCCESS;
} 

// This test has been derived from the equivalent test in VTK:
/*=========================================================================

  Program:   Visualization Toolkit
  Module:    itkSystemInformationTest.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 1993-2002 Ken Martin, Will Schroeder, Bill Lorensen 
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
