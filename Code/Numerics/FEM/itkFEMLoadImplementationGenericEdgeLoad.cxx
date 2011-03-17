/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFEMLoadImplementationGenericEdgeLoad.cxx
  Language:  C++
  Date:      $Date$
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

#include "itkFEMLoadImplementationGenericEdgeLoad.h"

namespace itk
{
namespace fem
{
/**
 * Handles EdgeLoad on 2D linear quad stress element
 */
void
LoadImplementationGenericEdgeLoad
::Implementation(Element::ConstPointer element, LoadEdge::Pointer load, Element::VectorType & Fe)
{
  const unsigned int NnDOF = element->GetNumberOfDegreesOfFreedomPerNode();
  //const unsigned int Nnodes = element->GetNumberOfNodes();
  const unsigned int EdgeNum = load->GetEdge();

  vnl_matrix<itk::fem::Element::Float> Force = load->GetForce();

  const std::vector< std::vector<int> > EdgeIds = element->GetEdgeIds();

  Fe.set_size( element->GetNumberOfDegreesOfFreedom() );
  Fe.fill(0.0);

  //int NEdges = EdgeIds.size();
  int NEdgePts = (EdgeIds[0]).size();
  int EdgePt;
  // access the edge points.
  for (int i=0; i<NEdgePts; i++)
  {
	  EdgePt = (EdgeIds[EdgeNum])[i];
	  for (unsigned int j=0; j<NnDOF; j++)
	  {
		  Fe[NnDOF*EdgePt + j] = Fe[NnDOF*EdgePt + j] + Force[i][j];
	  }	  
  }
  
}
}
}  // end namespace itk::fem
