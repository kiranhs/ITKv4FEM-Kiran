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

#ifndef __itkFEMElementStd_txx
#define __itkFEMElementStd_txx

#include "itkFEMElementStd.h"

namespace itk
{
namespace fem
{
template< unsigned int VNumberOfPoints, unsigned int VNumberOfSpatialDimensions, class TBaseClass >
ElementStd< VNumberOfPoints, VNumberOfSpatialDimensions, TBaseClass >
::ElementStd()
{
  // Set all node ids to 0 (undefined).
  for ( int i = 0; i < NumberOfNodes; i++ )
    {
    this->m_node[i] = 0;
    }
}

template< unsigned int VNumberOfPoints, unsigned int VNumberOfSpatialDimensions, class TBaseClass >
void 
ElementStd< VNumberOfPoints, VNumberOfSpatialDimensions, TBaseClass >
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "# Nodes: " << NumberOfNodes << std::endl;
  for (unsigned int i =0;i<NumberOfNodes;i++)
  {
    os << indent << "Node (" << i << "): " << this->m_node[i] << std::endl;
  }
}


}
}  // end namespace itk::fem

#endif // #ifndef __itkFEMElementStd_txx
