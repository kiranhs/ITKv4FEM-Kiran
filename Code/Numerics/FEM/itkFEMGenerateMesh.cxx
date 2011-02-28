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

#include "itkFEMGenerateMesh.h"
#include "itkFEMElement2DC0LinearQuadrilateral.h"
#include "itkFEMElement3DC0LinearHexahedron.h"
#include <math.h>
namespace itk
{
namespace fem
{
/**
 * Generate a rectangular mesh of quadrilateral elements
 */
void Generate2DRectilinearMesh(itk::fem::Element::ConstPointer e0,
                               Solver & S,
                               vnl_vector< double > & orig,
                               vnl_vector< double > & size,
                               vnl_vector< double > & Nel)
{
  // Check for correct number of dimensions
  if ( orig.size() != Element2DC0LinearQuadrilateral::NumberOfSpatialDimensions
       || size.size() != Element2DC0LinearQuadrilateral::NumberOfSpatialDimensions
       || Nel.size() != Element2DC0LinearQuadrilateral::NumberOfSpatialDimensions )
    {
    throw FEMException(__FILE__, __LINE__, "GenerateMesh<Element2DC0LinearQuadrilateral>::Rectangular");
    }

  // Clear existing elements, loads and nodes in Solver
  // changes made - kiran
  /*
   S.load.clear();
   S.el.clear();
   S.node.clear();
   */
  S.ClearLoadArray();
  S.ClearElementArray();
  S.ClearNodeArray();
  // changes made - kiran

  // Number of nodes in each dimension
  Nel[0] = vcl_floor(Nel[0]);
  Nel[1] = vcl_floor(Nel[1]);
  double Ni = static_cast< double >( Nel[0] );
  double Nj = static_cast< double >( Nel[1] );

  // Create nodes
  Node::Pointer n;
  int           gn = 0; // number of node
  for ( double j = 0; j <= Nj; j++ )
    {
    for ( double i = 0; i <= Ni; i++ )
      {
      n = new Node(orig[0] + i * size[0] / Nel[0], orig[1] + j * size[1] / Nel[1]);
      // changes made - kiran
      //n->GN=gn;
      n->SetGlobalNumber(gn);
      // changes made - kiran
      gn++;
      // changes made - kiran
      //S.node.push_back(FEMP<Node>(n));
      S.AddNextNode(n);
      // changes made - kiran
      }
    }

  // Create elements
  gn = 0; // global number of the element
  Element2DC0LinearQuadrilateral::Pointer e;
  for ( unsigned int j = 0; j < Nj; j++ )
    {
    for ( unsigned int i = 0; i < Ni; i++ )
      {
#ifndef FEM_USE_SMART_POINTERS
      e = dynamic_cast< Element2DC0LinearQuadrilateral * >( e0->Clone() );
#else
      e = dynamic_cast< Element2DC0LinearQuadrilateral * >( e0->Clone().GetPointer() );
#endif
      // changes made - kiran
      /*e->SetNode(0,S.node.Find((unsigned int) (i+  (Ni+1)*j)     ));
      e->SetNode(1,S.node.Find((unsigned int) (i+1+(Ni+1)*j)     ));
      e->SetNode(2,S.node.Find((unsigned int) (i+1+(Ni+1)*(j+1)) ));
      e->SetNode(3,S.node.Find((unsigned int) (i+  (Ni+1)*(j+1)) ));*/
#ifndef FEM_USE_SMART_POINTERS
      e->SetNode( 0, S.GetNode( (unsigned int)( i +  ( Ni + 1 ) * j ) ) );
      e->SetNode( 1, S.GetNode( (unsigned int)( i + 1 + ( Ni + 1 ) * j ) ) );
      e->SetNode( 2, S.GetNode( (unsigned int)( i + 1 + ( Ni + 1 ) * ( j + 1 ) ) ) );
      e->SetNode( 3, S.GetNode( (unsigned int)( i +  ( Ni + 1 ) * ( j + 1 ) ) ) );
#else
      e->SetNode( 0, static_cast<itk::fem::Element::Node*> (S.GetNode( (unsigned int)( i +  ( Ni + 1 ) * j ) ) ));
      e->SetNode( 1, static_cast<itk::fem::Element::Node*> (S.GetNode( (unsigned int)( i + 1 + ( Ni + 1 ) * j ) ) ));
      e->SetNode( 2, static_cast<itk::fem::Element::Node*> (S.GetNode( (unsigned int)( i + 1 + ( Ni + 1 ) * ( j + 1 ) ) ) ));
      e->SetNode( 3, static_cast<itk::fem::Element::Node*> (S.GetNode( (unsigned int)( i +  ( Ni + 1 ) * ( j + 1 ) ) ) ));
#endif
      // changes made - kiran
      //e->GN=gn;
      e->SetGlobalNumber(gn);
      // changes made - kiran
      gn++;
      //S.el.push_back(FEMP<Element>(e));
#ifndef FEM_USE_SMART_POINTERS
      S.AddNextElement(e);
#endif
      // changes made - kiran
      }
    }
}

void Generate2DRectilinearMesh1(itk::fem::Element::ConstPointer e0,
                               itk::fem::FEMObject<2> *femObject,
                               vnl_vector< double > & orig,
                               vnl_vector< double > & size,
                               vnl_vector< double > & Nel)
{
  // Check for correct number of dimensions
  if ( orig.size() != Element2DC0LinearQuadrilateral::NumberOfSpatialDimensions
       || size.size() != Element2DC0LinearQuadrilateral::NumberOfSpatialDimensions
       || Nel.size() != Element2DC0LinearQuadrilateral::NumberOfSpatialDimensions )
    {
    throw FEMException(__FILE__, __LINE__, "GenerateMesh<Element2DC0LinearQuadrilateral>::Rectangular");
    }

  femObject->GetLoadContainer()->Initialize();
  femObject->GetElementContainer()->Initialize();
  femObject->GetNodeContainer()->Initialize();

  // Number of nodes in each dimension
  Nel[0] = vcl_floor(Nel[0]);
  Nel[1] = vcl_floor(Nel[1]);
  double Ni = static_cast< double >( Nel[0] );
  double Nj = static_cast< double >( Nel[1] );

  // Create nodes
  Node::Pointer n;
  int           gn = 0; // number of node
  for ( double j = 0; j <= Nj; j++ )
    {
    for ( double i = 0; i <= Ni; i++ )
      {
      n = new Node(orig[0] + i * size[0] / Nel[0], orig[1] + j * size[1] / Nel[1]);
      n->SetGlobalNumber(gn);
      gn++;
      femObject->AddNextNode(n);
      }
    }

  // Create elements
  gn = 0; // global number of the element
  Element2DC0LinearQuadrilateral::Pointer e;
  for ( unsigned int j = 0; j < Nj; j++ )
    {
    for ( unsigned int i = 0; i < Ni; i++ )
      {
      e = dynamic_cast< Element2DC0LinearQuadrilateral * >( &*e0->Clone() );
      e->SetNode( 0, static_cast<itk::fem::Element::Node*> (femObject->GetNode( (unsigned int)( i +  ( Ni + 1 ) * j ) ) ) );
      e->SetNode( 1, static_cast<itk::fem::Element::Node*> (femObject->GetNode( (unsigned int)( i + 1 + ( Ni + 1 ) * j ) ) ) );
      e->SetNode( 2, static_cast<itk::fem::Element::Node*> (femObject->GetNode( (unsigned int)( i + 1 + ( Ni + 1 ) * ( j + 1 ) ) ) ) );
        e->SetNode( 3, static_cast<itk::fem::Element::Node*> (femObject->GetNode( (unsigned int)( i +  ( Ni + 1 ) * ( j + 1 ) ) ) ) );
      e->SetGlobalNumber(gn);
      gn++;
      femObject->AddNextElement(&*e);
      }
    }
}

/**
 * Generate a rectangular mesh of hexahedron elements
 */
void Generate3DRectilinearMesh
  (itk::fem::Element::ConstPointer e0, Solver & S, vnl_vector< double > & orig,
  vnl_vector< double > & size, vnl_vector< double > & Nel)
{
  // Check for correct number of dimensions
  if ( orig.size() != Element3DC0LinearHexahedron::NumberOfSpatialDimensions
       || size.size() != Element3DC0LinearHexahedron::NumberOfSpatialDimensions
       || Nel.size() != Element3DC0LinearHexahedron::NumberOfSpatialDimensions )
    {
    throw FEMException(__FILE__, __LINE__, "GenerateMesh<Element2DC0LinearQuadrilateral>::Rectangular");
    }

  // Number of nodes in each dimension
  Nel[0] = vcl_floor(Nel[0]);
  Nel[1] = vcl_floor(Nel[1]);
  Nel[2] = vcl_floor(Nel[2]);
  double Ni = static_cast< double >( Nel[0] );
  double Nj = static_cast< double >( Nel[1] );
  double Nk = static_cast< double >( Nel[2] );

  // Create nodes
  Node::Pointer n;
  int           gn = 0; // number of node
  for ( double k = 0; k <= Nk; k++ )
    {
    for ( double j = 0; j <= Nj; j++ )
      {
      for ( double i = 0; i <= Ni; i++ )
        {
        double xx, yy, zz;
        xx = orig[0] + i * size[0] / Nel[0];
        yy = orig[1] + j * size[1] / Nel[1];
        zz = orig[2] + k * size[2] / Nel[2];
//std::cout << " xx " << xx << " yy " << yy << " zz " << zz << std::endl;
        n = new Node(xx, yy, zz);
        // changes made - kiran
        //n->GN=gn;
        n->SetGlobalNumber(gn);
        // changes made - kiran
        gn++;
        // changes made - kiran
        //S.node.push_back(FEMP<Node>(n));
        S.AddNextNode(n);
        // changes made - kiran
        }
      }
    }

  // Create elements
  gn = 0; // global number of the element
  itk::fem::Element3DC0LinearHexahedron::Pointer e;
  for ( unsigned int k = 0; k < Nk; k++ )
    {
    for ( unsigned int j = 0; j < Nj; j++ )
      {
      for ( unsigned int i = 0; i < Ni; i++ )
        {
#ifndef FEM_USE_SMART_POINTERS
        e = dynamic_cast< Element3DC0LinearHexahedron * >( e0->Clone() );
#else
        e = dynamic_cast< Element3DC0LinearHexahedron * >( e0->Clone().GetPointer() );
#endif
        // changes made - kiran
        /*       e->SetNode(0,S.node.Find((unsigned int) (i+  (Ni+1)*(j  +(Nj+1)*k) )));
               e->SetNode(1,S.node.Find((unsigned int) (i+1+(Ni+1)*(j  +(Nj+1)*k) )));
               e->SetNode(2,S.node.Find((unsigned int) (i+1+(Ni+1)*(j+1+(Nj+1)*k) )));
               e->SetNode(3,S.node.Find((unsigned int) (i+  (Ni+1)*(j+1+(Nj+1)*k) )));
               e->SetNode(4,S.node.Find((unsigned int) (i+  (Ni+1)*(j  +(Nj+1)*(k+1)) )));
               e->SetNode(5,S.node.Find((unsigned int) (i+1+(Ni+1)*(j  +(Nj+1)*(k+1)) )));
               e->SetNode(6,S.node.Find((unsigned int) (i+1+(Ni+1)*(j+1+(Nj+1)*(k+1)) )));
               e->SetNode(7,S.node.Find((unsigned int) (i+  (Ni+1)*(j+1+(Nj+1)*(k+1)) )));*/
#ifndef FEM_USE_SMART_POINTERS
        e->SetNode( 0, S.GetNode( (unsigned int)( i +  ( Ni + 1 ) * ( j  + ( Nj + 1 ) * k ) ) ) );
        e->SetNode( 1, S.GetNode( (unsigned int)( i + 1 + ( Ni + 1 ) * ( j  + ( Nj + 1 ) * k ) ) ) );
        e->SetNode( 2, S.GetNode( (unsigned int)( i + 1 + ( Ni + 1 ) * ( j + 1 + ( Nj + 1 ) * k ) ) ) );
        e->SetNode( 3, S.GetNode( (unsigned int)( i +  ( Ni + 1 ) * ( j + 1 + ( Nj + 1 ) * k ) ) ) );
        e->SetNode( 4, S.GetNode( (unsigned int)( i +  ( Ni + 1 ) * ( j  + ( Nj + 1 ) * ( k + 1 ) ) ) ) );
        e->SetNode( 5, S.GetNode( (unsigned int)( i + 1 + ( Ni + 1 ) * ( j  + ( Nj + 1 ) * ( k + 1 ) ) ) ) );
        e->SetNode( 6, S.GetNode( (unsigned int)( i + 1 + ( Ni + 1 ) * ( j + 1 + ( Nj + 1 ) * ( k + 1 ) ) ) ) );
        e->SetNode( 7, S.GetNode( (unsigned int)( i +  ( Ni + 1 ) * ( j + 1 + ( Nj + 1 ) * ( k + 1 ) ) ) ) );
#else
        e->SetNode( 0, static_cast<itk::fem::Element::Node*> (S.GetNode( (unsigned int)( i +  ( Ni + 1 ) * ( j  + ( Nj + 1 ) * k ) ) ) ));
        e->SetNode( 1, static_cast<itk::fem::Element::Node*> (S.GetNode( (unsigned int)( i + 1 + ( Ni + 1 ) * ( j  + ( Nj + 1 ) * k ) ) ) ));
        e->SetNode( 2, static_cast<itk::fem::Element::Node*> (S.GetNode( (unsigned int)( i + 1 + ( Ni + 1 ) * ( j + 1 + ( Nj + 1 ) * k ) ) ) ));
        e->SetNode( 3, static_cast<itk::fem::Element::Node*> (S.GetNode( (unsigned int)( i +  ( Ni + 1 ) * ( j + 1 + ( Nj + 1 ) * k ) ) ) ));
        e->SetNode( 4, static_cast<itk::fem::Element::Node*> (S.GetNode( (unsigned int)( i +  ( Ni + 1 ) * ( j  + ( Nj + 1 ) * ( k + 1 ) ) ) ) ));
        e->SetNode( 5, static_cast<itk::fem::Element::Node*> (S.GetNode( (unsigned int)( i + 1 + ( Ni + 1 ) * ( j  + ( Nj + 1 ) * ( k + 1 ) ) ) ) ));
        e->SetNode( 6, static_cast<itk::fem::Element::Node*> (S.GetNode( (unsigned int)( i + 1 + ( Ni + 1 ) * ( j + 1 + ( Nj + 1 ) * ( k + 1 ) ) ) ) ));
        e->SetNode( 7, static_cast<itk::fem::Element::Node*> (S.GetNode( (unsigned int)( i +  ( Ni + 1 ) * ( j + 1 + ( Nj + 1 ) * ( k + 1 ) ) ) ) ));
#endif
        // changes made - kiran
        //e->GN=gn;
        e->SetGlobalNumber(gn);
        gn++;
        //S.el.push_back(FEMP<Element>(e));
#ifndef FEM_USE_SMART_POINTERS
        S.AddNextElement(&*e);
        // changes made - kiran
#endif
          //FIXME - Port to FEMOBJECT
        }
      }
    }
}

void Generate3DRectilinearMesh
(itk::fem::Element::ConstPointer e0, itk::fem::FEMObject<3> *femObject, vnl_vector< double > & orig,
  vnl_vector< double > & size, vnl_vector< double > & Nel)
{
  // Check for correct number of dimensions
  if ( orig.size() != Element3DC0LinearHexahedron::NumberOfSpatialDimensions
       || size.size() != Element3DC0LinearHexahedron::NumberOfSpatialDimensions
       || Nel.size() != Element3DC0LinearHexahedron::NumberOfSpatialDimensions )
    {
    throw FEMException(__FILE__, __LINE__, "GenerateMesh<Element2DC0LinearQuadrilateral>::Rectangular");
    }

  // Number of nodes in each dimension
  Nel[0] = vcl_floor(Nel[0]);
  Nel[1] = vcl_floor(Nel[1]);
  Nel[2] = vcl_floor(Nel[2]);
  double Ni = static_cast< double >( Nel[0] );
  double Nj = static_cast< double >( Nel[1] );
  double Nk = static_cast< double >( Nel[2] );

  // Create nodes
  Node::Pointer n;
  int           gn = 0; // number of node
  for ( double k = 0; k <= Nk; k++ )
    {
    for ( double j = 0; j <= Nj; j++ )
      {
      for ( double i = 0; i <= Ni; i++ )
        {
        double xx, yy, zz;
        xx = orig[0] + i * size[0] / Nel[0];
        yy = orig[1] + j * size[1] / Nel[1];
        zz = orig[2] + k * size[2] / Nel[2];
        n = new Node(xx, yy, zz);
        n->SetGlobalNumber(gn);
        gn++;
        femObject->AddNextNode(n);
        }
      }
    }

  // Create elements
  gn = 0; // global number of the element
  itk::fem::Element3DC0LinearHexahedron::Pointer e;
  for ( unsigned int k = 0; k < Nk; k++ )
    {
    for ( unsigned int j = 0; j < Nj; j++ )
      {
      for ( unsigned int i = 0; i < Ni; i++ )
        {
        e = dynamic_cast< Element3DC0LinearHexahedron * >( e0->Clone().GetPointer() );
        e->SetNode( 0, static_cast<itk::fem::Element::Node*> (femObject->GetNode( (unsigned int)( i +  ( Ni + 1 ) * ( j  + ( Nj + 1 ) * k ) ) ) ) );
        e->SetNode( 1, static_cast<itk::fem::Element::Node*> (femObject->GetNode( (unsigned int)( i + 1 + ( Ni + 1 ) * ( j  + ( Nj + 1 ) * k ) ) ) ) );
        e->SetNode( 2, static_cast<itk::fem::Element::Node*> (femObject->GetNode( (unsigned int)( i + 1 + ( Ni + 1 ) * ( j + 1 + ( Nj + 1 ) * k ) ) ) ) );
        e->SetNode( 3, static_cast<itk::fem::Element::Node*> (femObject->GetNode( (unsigned int)( i +  ( Ni + 1 ) * ( j + 1 + ( Nj + 1 ) * k ) ) ) ) );
        e->SetNode( 4, static_cast<itk::fem::Element::Node*> (femObject->GetNode( (unsigned int)( i +  ( Ni + 1 ) * ( j  + ( Nj + 1 ) * ( k + 1 ) ) ) ) ) );
        e->SetNode( 5, static_cast<itk::fem::Element::Node*> (femObject->GetNode( (unsigned int)( i + 1 + ( Ni + 1 ) * ( j  + ( Nj + 1 ) * ( k + 1 ) ) ) ) ) );
        e->SetNode( 6, static_cast<itk::fem::Element::Node*> (femObject->GetNode( (unsigned int)( i + 1 + ( Ni + 1 ) * ( j + 1 + ( Nj + 1 ) * ( k + 1 ) ) ) ) ) );
        e->SetNode( 7, static_cast<itk::fem::Element::Node*> (femObject->GetNode( (unsigned int)( i +  ( Ni + 1 ) * ( j + 1 + ( Nj + 1 ) * ( k + 1 ) ) ) ) ) );
        e->SetGlobalNumber(gn);
        gn++;
        femObject->AddNextElement(&*e);
        }
      }
    }
}
}
}  // end namespace itk::fem
