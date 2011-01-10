/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    RGBPointSet.cxx
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

//  Software Guide : BeginLatex
//
//  The following example illustrates how a point set can be parameterized to
//  manage a particular pixel type. In this case, pixels of RGB type are used.
//  The first step is then to include the header files of the
//  \doxygen{RGBPixel} and \doxygen{PointSet} classes.
//
//  \index{itk::PointSet!RGBPixel}
//
//  Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet
#include "itkRGBPixel.h"
#include "itkPointSet.h"
// Software Guide : EndCodeSnippet


int main(int, char *[])
{
  //  Software Guide : BeginLatex
  //
  //  Then, the pixel type can be defined by selecting the type to be used to
  //  represent each one of the RGB components.
  //
  //  \index{itk::RGBPixel!Instantiation}
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  typedef itk::RGBPixel< float >    PixelType;
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //
  //  The newly defined pixel type is now used to instantiate the PointSet
  //  type and subsequently create a point set object.
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  typedef itk::PointSet< PixelType, 3 > PointSetType;
  PointSetType::Pointer  pointSet = PointSetType::New();
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //
  //  The following code is generating a sphere and assigning RGB values to
  //  the points. The components of the RGB values in this example are
  //  computed to represent the position of the points.
  //
  //  \index{itk::PointSet!SetPoint()}
  //  \index{itk::PointSet!SetPointData()}
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  PointSetType::PixelType   pixel;
  PointSetType::PointType   point;
  unsigned int pointId =  0;
  const double radius = 3.0;

  for(unsigned int i=0; i<360; i++)
    {
    const double angle = i * vnl_math::pi / 180.0;
    point[0] = radius * vcl_sin( angle );
    point[1] = radius * vcl_cos( angle );
    point[2] = 1.0;
    pixel.SetRed(    point[0] * 2.0 );
    pixel.SetGreen(  point[1] * 2.0 );
    pixel.SetBlue(   point[2] * 2.0 );
    pointSet->SetPoint( pointId, point );   
    pointSet->SetPointData( pointId, pixel );   
    pointId++;
    }
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //
  //  All the points on the PointSet are visited using the following code.
  //
  //  \index{itk::PointSet!GetPoints()}
  //  \index{itk::PointSet!points iterator}
  //  \index{itk::PointSet!iterating points}
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  typedef  PointSetType::PointsContainer::ConstIterator     PointIterator;
  PointIterator pointIterator = pointSet->GetPoints()->Begin();
  PointIterator pointEnd      = pointSet->GetPoints()->End();
  while( pointIterator != pointEnd ) 
    {
    point = pointIterator.Value();
    std::cout << point << std::endl;  
    ++pointIterator;
    }
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //
  //  Note that here the \code{ConstIterator} was used instead of the
  //  \code{Iterator} since the pixel values are not expected to be modified.
  //  ITK supports const-correctness at the API level. 
  //
  //  Software Guide : EndLatex 


  //  Software Guide : BeginLatex
  //
  //  All the pixel values on the PointSet are visited using the following code.
  //
  //  \index{itk::PointSet!GetPointData()}
  //  \index{itk::PointSet!data iterator}
  //  \index{itk::PointSet!iterating point data}
  //
  //  Software Guide : EndLatex 


  // Software Guide : BeginCodeSnippet
  typedef  PointSetType::PointDataContainer::ConstIterator     PointDataIterator;
  PointDataIterator pixelIterator = pointSet->GetPointData()->Begin();
  PointDataIterator pixelEnd      = pointSet->GetPointData()->End();
  while( pixelIterator != pixelEnd ) 
    {
    pixel = pixelIterator.Value();
    std::cout << pixel << std::endl;  
    ++pixelIterator;
    }
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  Again, please note the use of the  \code{ConstIterator} instead of the
  //  \code{Iterator}. 
  //
  //  \index{ConstIterator}
  //  \index{const-correctness}
  //
  //  Software Guide : EndLatex 

  return 0;

}
