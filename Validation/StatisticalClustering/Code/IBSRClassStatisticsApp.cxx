/*=========================================================================

Program:   Insight Segmentation & Registration Toolkit
Module:    IBSRClassStatisticsApp.cxx
Language:  C++
Date:      $Date$
Version:   $Revision$

Copyright (c) Insight Software Consortium. All rights reserved.
See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

This software is distributed WITHOUT ANY WARRANTY; without even 
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "OptionList.h"
#include "ValidationSampleGenerator.h"

void print_usage()
{
  std::cout << "kmeans clustering 1.0 (17. Dec. 2001)" << std::endl ;

  std::cout << "usage: IBSRClassStatisticsApp --images file..."  << std::endl ;
  std::cout << "       --mask file --slice-offset int"  << std::endl ;

  std::cout << "" << std::endl ;

  std::cout << "--images file" << std::endl ;
  std::cout << "        image file name with intesnity values [meta image format]"  
            << std::endl ;

  std::cout << "--mask file" << std::endl ;
  std::cout << "        class label image file name that will have the class labels for pixels" 
            << std::endl ;
  std::cout << "        in the target image file [meta image format]"  << std::endl ;

  std::cout << "example: IBSRClassStatisticsGeneratorApp --image 20Normals_T1_brain/110_3.mhd" 
            << std::endl ;
  std::cout << "         --mask 20Normals_T1_seg/110_3.mhd --slice-offset 0" << std::endl ;
}

int main(int argc, char* argv[])
{
  if (argc <= 1)
    {
      print_usage() ;
      exit(0) ;
    }

  OptionList options(argc, argv) ;

  std::vector< std::string > imageFileNames ;
  std::string maskFileName ;
  int sliceOffset ;
  try
    {
      options.GetMultiStringOption("images", &imageFileNames, true) ;
      options.GetStringOption("mask", &maskFileName, true) ;
      sliceOffset = options.GetIntOption("slice-offset", 2, true) ;
    }
  catch(OptionList::RequiredOptionMissing e)
    {
      std::cout << "Error: The '" << e.OptionTag 
                << "' option is required but missing." 
                << std::endl ;
      return 1 ;
    }
 
  std::vector< unsigned int > selectedClasses ;
  selectedClasses.push_back(128) ; // csf
  selectedClasses.push_back(192) ; // gray
  selectedClasses.push_back(254) ; // white

  typedef itk::Image< unsigned char, 3 > ImageType ;
  typedef itk::FixedArray< unsigned char, 1 > VectorPixelType ;
  typedef itk::Image< VectorPixelType, 3 > VectorImageType ;
  typedef ValidationSampleGenerator< ImageType, 
    ImageType, VectorImageType > SampleGeneratorType ;
  SampleGeneratorType sampleGenerator ;

  sampleGenerator.SetOutputSampleType(SampleGeneratorType::LIST_SAMPLE) ;
  sampleGenerator.SetImageFileNames(imageFileNames) ;
  sampleGenerator.SetClassMaskImageFileName(maskFileName.c_str(), 0, sliceOffset) ;
  sampleGenerator.SetSelectedClasses(selectedClasses) ;
  sampleGenerator.SetOutputNormalized(false) ;

  sampleGenerator.GenerateData() ;
  std::cout << "sample created." << std::endl ;

  std::string::size_type caseNoBegin = imageFileNames[0].find_last_of('/') + 1 ;
  if ( caseNoBegin > imageFileNames[0].length() )
    {
      caseNoBegin = 0 ;
    }
  
  std::string::size_type caseNoLength = imageFileNames[0].rfind(".mhd") - caseNoBegin ;
  std::string caseNo = imageFileNames[0].substr(caseNoBegin, caseNoLength) ;
  std::cout << "\"class\" \"size\" \"mean.1\" \"sigma.1\"" << std::endl ;
  
  for ( int i = 0 ; i < selectedClasses.size() ; i++ )
    { 
      //      std::cout << caseNo ;
      std::cout << " " << selectedClasses[i] ;
      std::cout << " " << sampleGenerator.GetClassSize(selectedClasses[i]) ;
      std::cout << " " << sampleGenerator.GetClassMean(selectedClasses[i])[0] ;
      std::cout << " " 
                << sampleGenerator.GetClassCovariance(selectedClasses[i]).GetVnlMatrix()[0][0] ;
      std::cout << std::endl ;
    }

  return 0 ;
}
