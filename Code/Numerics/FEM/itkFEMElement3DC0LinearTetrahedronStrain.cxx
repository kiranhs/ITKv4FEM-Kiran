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

#include "itkFEMElement3DC0LinearTetrahedronStrain.h"
#include "vnl/vnl_math.h"

namespace itk
{
namespace fem
{
// Explicit New() method, used here because we need to split the itkNewMacro()
// in order to overload the CreateAnother() method.
Element3DC0LinearTetrahedronStrain::Pointer Element3DC0LinearTetrahedronStrain::New(void)
{
  Pointer smartPtr = ::itk::ObjectFactory< Self >::Create();
  if(smartPtr.IsNull())
  {
    smartPtr = static_cast<Pointer>(new Self);
  }
  smartPtr->UnRegister();
  return smartPtr;
}

// Overload the CreateAnother() method
::itk::LightObject::Pointer Element3DC0LinearTetrahedronStrain::CreateAnother(void) const
{
  ::itk::LightObject::Pointer smartPtr;
  Pointer copyPtr = Self::New().GetPointer();
  
  copyPtr->SetNode(0, this->GetNode(0));
  copyPtr->SetNode(1, this->GetNode(1));
  copyPtr->SetNode(2, this->GetNode(2));
  copyPtr->SetNode(3, this->GetNode(3));
  copyPtr->SetMaterial( this->GetMaterial( ) );
  copyPtr->SetGlobalNumber( this->GetGlobalNumber() );
  
  smartPtr = static_cast<Pointer>(copyPtr);
  
  return smartPtr;
}

  
Element3DC0LinearTetrahedronStrain
::Element3DC0LinearTetrahedronStrain():Superclass()
{}

Element3DC0LinearTetrahedronStrain
::Element3DC0LinearTetrahedronStrain(
  NodeIDType ns_[],
  Material::ConstPointer m_):Superclass()
{
  // Set the geometrical points
  for ( int k = 0; k < 4; k++ )
    {
    this->SetNode(k, ns_[k]);
    }

  /*
   * Initialize the pointer to material object and check that
   * we were given the pointer to the right class.
   * If the material class was incorrect an exception is thrown.
   */
  m_mat = dynamic_cast< const MaterialLinearElasticity * >( &*m_ );
  
  if ( ! m_mat )
    {
    throw FEMExceptionWrongClass(__FILE__,
                                 __LINE__,
                                 "Element3DC0LinearTetrahedronStrain::Element3DC0LinearTetrahedronStrain()");
    }
}

}
}  // end namespace itk::fem
