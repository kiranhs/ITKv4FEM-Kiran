/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    BinaryMedianImageFilter.cxx
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

//  Software Guide : BeginCommandLineArgs
//    INPUTS: {BinaryThresholdImageFilterOutput.png}
//    OUTPUTS: {BinaryMedianImageFilterOutput.png}
//    2 2
//  Software Guide : EndCommandLineArgs
//
//    BinaryThresholdImageFilterOutput.png was obtained from the BinaryThreshold
//    ImageFilter example.
//
//  Software Guide : BeginLatex
//
//  The \doxygen{BinaryMedianImageFilter} is commonly used as a robust approach
//  for noise reduction. BinaryMedianImageFilter computes the value of each
//  output pixel as the statistical median of the neighborhood of values around
//  the corresponding input pixel. When the input images are binary, the
//  implementation can be optimized by simply counting the number of pixels
//  ON/OFF around the current pixel. 
//
//  This filter will work on images of any dimension thanks to the internal use
//  of \doxygen{NeighborhoodIterator} and \doxygen{NeighborhoodOperator}. The
//  size of the neighborhood over which the median is computed can be set by
//  the user.
//
//  \index{itk::BinaryMedianImageFilter}
//
//  Software Guide : EndLatex 


#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"


//  Software Guide : BeginLatex
//
//  The header file corresponding to this filter should be included first.
//
//  \index{itk::BinaryMedianImageFilter!header}
//
//  Software Guide : EndLatex 


// Software Guide : BeginCodeSnippet
#include "itkBinaryMedianImageFilter.h"
// Software Guide : EndCodeSnippet


int main( int argc, char * argv[] )
{
  if( argc < 4 )
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << "  inputImageFile outputImageFile radiusX radiusY" << std::endl;
    return EXIT_FAILURE;
    }


  //  Software Guide : BeginLatex
  //
  //  Then the pixel and image types of the input and output must be defined.
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  typedef   unsigned char  InputPixelType;
  typedef   unsigned char  OutputPixelType;

  typedef itk::Image< InputPixelType,  2 >   InputImageType;
  typedef itk::Image< OutputPixelType, 2 >   OutputImageType;
  // Software Guide : EndCodeSnippet


  typedef itk::ImageFileReader< InputImageType  >  ReaderType;
  typedef itk::ImageFileWriter< OutputImageType >  WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName( argv[1] );
  writer->SetFileName( argv[2] );

  //  Software Guide : BeginLatex
  //
  //  Using the image types, it is now possible to define the filter type
  //  and create the filter object.
  //
  //  \index{itk::BinaryMedianImageFilter!instantiation}
  //  \index{itk::BinaryMedianImageFilter!New()}
  //  \index{itk::BinaryMedianImageFilter!Pointer}
  // 
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  typedef itk::BinaryMedianImageFilter<
               InputImageType, OutputImageType >  FilterType;

  FilterType::Pointer filter = FilterType::New();
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //
  //  The size of the neighborhood is defined along every dimension by
  //  passing a \code{SizeType} object with the corresponding values. The
  //  value on each dimension is used as the semi-size of a rectangular
  //  box. For example, in $2D$ a size of \(1,2\) will result in a $3 \times
  //  5$ neighborhood.
  //
  //  \index{itk::BinaryMedianImageFilter!Radius}
  //  \index{itk::BinaryMedianImageFilter!Neighborhood}
  //
  //  Software Guide : EndLatex 

  const unsigned int radiusX = atoi( argv[3] );
  const unsigned int radiusY = atoi( argv[4] );

  // Software Guide : BeginCodeSnippet
  InputImageType::SizeType indexRadius;
  
  indexRadius[0] = radiusX; // radius along x
  indexRadius[1] = radiusY; // radius along y

  filter->SetRadius( indexRadius );
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The input to the filter can be taken from any other filter, for example
  //  a reader. The output can be passed down the pipeline to other filters,
  //  for example, a writer. An update call on any downstream filter will
  //  trigger the execution of the median filter.
  //
  //  \index{itk::BinaryMedianImageFilter!SetInput()}
  //  \index{itk::BinaryMedianImageFilter!GetOutput()}
  //
  //  Software Guide : EndLatex 


  // Software Guide : BeginCodeSnippet
  filter->SetInput( reader->GetOutput() );
  writer->SetInput( filter->GetOutput() );
  writer->Update();
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  // 
  // \begin{figure}
  // \center
  // \includegraphics[width=0.44\textwidth]{BinaryThresholdImageFilterOutput.eps}
  // \includegraphics[width=0.44\textwidth]{BinaryMedianImageFilterOutput.eps}
  // \itkcaption[Effect of the BinaryMedian filter.]{Effect of the
  // BinaryMedianImageFilter on a slice from a MRI proton density brain image
  // that has been thresholded in order to produce a binary image.}
  // \label{fig:BinaryMedianImageFilterOutput}
  // \end{figure}
  //
  //  Figure \ref{fig:BinaryMedianImageFilterOutput} illustrates the effect of
  //  the BinaryMedianImageFilter filter on a slice of MRI brain image using a
  //  neighborhood radius of \(2,2\), which corresponds to a $ 5 \times 5 $
  //  classical neighborhood.  The filtered image demonstrates the capability
  //  of this filter for reducing noise both in the background and foreground of
  //  the image, as well as smoothing the contours of the regions.
  //  
  //  Software Guide : EndLatex 


  return EXIT_SUCCESS;
}
