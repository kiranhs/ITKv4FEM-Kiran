/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFEMFEMObject.cxx
  Language:  C++
  Date:  $Date$
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

#include "itkFEMObject.h"

#include "itkFEMLoadNode.h"
#include "itkFEMLoadElementBase.h"
#include "itkFEMLoadBC.h"
#include "itkFEMLoadBCMFC.h"
#include "itkFEMLoadLandmark.h"

#include <algorithm>

namespace itk
{
namespace fem
{
/*
 * Default constructor for FEMObject class
 */
template<unsigned int VDimension>
FEMObject<VDimension>::FEMObject()
{
NGFN = 0;
NMFC = 0;
  this->SetLinearSystemWrapper(&m_lsVNL);
  this->m_ElementContainer = ElementContainerType::New();
  this->m_NodeContainer = NodeContainerType::New();
  this->m_LoadContainer = LoadContainerType::New();
  this->m_MaterialContainer = MaterialContainerType::New();
}

template<unsigned int VDimension>
FEMObject<VDimension>::~FEMObject()
{
	
}

template<unsigned int VDimension>
void FEMObject<VDimension>::Clear(void)
{
  this->m_NodeContainer->Initialize();
  this->m_ElementContainer->Initialize();
  this->m_LoadContainer->Initialize();
  this->m_MaterialContainer->Initialize();

  this->NGFN = 0;
  this->NMFC = 0;
  this->SetLinearSystemWrapper(&m_lsVNL);
}

/**
 * Change the LinearSystemWrapper object used to solve
 * system of equations.
 */
template<unsigned int VDimension>
void FEMObject<VDimension>::SetLinearSystemWrapper(LinearSystemWrapper::Pointer ls)
{
  m_ls = ls; // update the pointer to LinearSystemWrapper object

  this->InitializeLinearSystemWrapper();
}

template<unsigned int VDimension>
void FEMObject<VDimension>::InitializeLinearSystemWrapper(void)
{
  // set the maximum number of matrices and vectors that
  // we will need to store inside.
  m_ls->SetNumberOfMatrices(1);
  m_ls->SetNumberOfVectors(2);
  m_ls->SetNumberOfSolutions(1);
}

/**
 * Assign a global freedom number to each DOF in a system.
 */
template<unsigned int VDimension>
void FEMObject<VDimension>::GenerateGFN()
{
  // Clear the list of elements and global freedom numbers in nodes
  // FIXME: should be removed once Mesh is there
  int numNodes = this->m_NodeContainer->Size();
  for ( int n = 0; n < numNodes; n++ )
    {
    Node::Pointer np = this->GetNode(n);
    np->m_elements.clear();
    np->ClearDegreesOfFreedom();
    }

int numElements = this->m_ElementContainer->Size();
  for ( int e = 0; e < numElements; e++ ) // step over
                                                                    // all
                                                                    // elements
    {
    // Add the elemens in the nodes list of elements
    // FIXME: should be removed once Mesh is there
    Element::Pointer el = this->GetElement(e);
    unsigned int Npts = el->GetNumberOfNodes();
    for ( unsigned int pt = 0; pt < Npts; pt++ )
      {
      el->GetNode(pt)->m_elements.insert(el);
      }
    }

  /**
   * Assign new ID to every DOF in a system
   */

  // Start numbering DOFs from 0
  NGFN = 0;

  // Step over all elements
  for ( int e = 0; e < numElements; e++ )
    {
    // FIXME: Write a code that checks if two elements are compatible, when they
    // share a node
    Element::Pointer el = GetElement(e);
    for ( unsigned int n = 0; n < el->GetNumberOfNodes(); n++ )
      {
      for ( unsigned int dof = 0; dof < el->GetNumberOfDegreesOfFreedomPerNode(); dof++ )
        {
        if ( el->GetNode(n)->GetDegreeOfFreedom(dof) == Element::InvalidDegreeOfFreedomID )
          {
          el->GetNode(n)->SetDegreeOfFreedom(dof, NGFN);
          NGFN++;
          }
        }
      }
    } // end for e

  //  NGFN=Element::GetGlobalDOFCounter()+1;
  if ( NGFN > 0 )
    {
    return;            // if we got 0 DOF, somebody forgot to define the
                       // system...
    }
}

/**
 * Assemble the master stiffness matrix (also apply the MFCs to K)
 */
template<unsigned int VDimension>
void FEMObject<VDimension>::AssembleK()
{
  // if no DOFs exist in a system, we have nothing to do
  if ( NGFN <= 0 ) { return; }

  NMFC = 0;  // reset number of MFC in a system

  /**
   * Before we can start the assembly procedure, we need to know,
   * how many boundary conditions if form of MFCs are there in a system.
   */

  // search for MFC's in Loads array, because they affect the master stiffness
  // matrix
  int numLoads = this->m_LoadContainer->Size();
  for ( int l = 0; l < numLoads; l++ )
    {
    if ( LoadBCMFC::Pointer l1 = dynamic_cast< LoadBCMFC * >( &*this->GetLoad(l) ) )
      {
      // store the index of an LoadBCMFC object for later
      // changes made - kiran
      //l1->Index=NMFC;
      l1->SetIndex(NMFC);
      // changes made - kiran

      // increase the number of MFC
      NMFC++;
      }
    }

  /**
   * Now we can assemble the master stiffness matrix from
   * element stiffness matrices.
   *
   * Since we're using the Lagrange multiplier method to apply the MFC,
   * each constraint adds a new global DOF.
   */
  this->InitializeMatrixForAssembly(NGFN + NMFC);

  /**
   * Step over all elements
   */
   int numElements = this->m_ElementContainer->Size();
  for ( int e = 0; e < numElements; e++ )
    {
    // Call the function that actually moves the element matrix
    // to the master matrix.
    this->AssembleElementMatrix(&*this->GetElement(e));
    }

  /**
   * Step over all the loads again to add the landmark contributions
   * to the appropriate place in the stiffness matrix
   */
  for ( int l2 = 0; l2 < numLoads; l2++ )
    {
    if ( LoadLandmark::Pointer l3 = dynamic_cast< LoadLandmark * >( &*this->GetLoad(l2)) )
      {
      l3->AssignToElement(this->m_ElementContainer);
      // changes made - kiran
      //Element::Pointer ep = const_cast<Element*>( l3->el[0] );
      //this->AssembleLandmarkContribution( ep , l3->eta );
      Element::Pointer ep = const_cast< Element * >( l3->GetElement(0) );
      this->AssembleLandmarkContribution( ep, l3->GetEta() );
      // changes made - kiran
      }
    }

  this->FinalizeMatrixAfterAssembly();
}

template<unsigned int VDimension>
void FEMObject<VDimension>::RenumberNodeContainer()
{

  int numNodes = this->m_NodeContainer->Size();
  for (int i = 0; i < numNodes; i++ )
    {
    // changes made - kiran
//		(*i)->GN=j;
    this->GetNode(i)->SetGlobalNumber(i);
    // changes made - kiran
    }
}

template<unsigned int VDimension>
void FEMObject<VDimension>::InitializeMatrixForAssembly(unsigned int N)
{
  // We use LinearSystemWrapper object, to store the K matrix.
  this->m_ls->SetSystemOrder(N);
  this->m_ls->InitializeMatrix();
}

template<unsigned int VDimension>
void FEMObject<VDimension>::AssembleLandmarkContribution(Element::Pointer e, float eta)
{
  // Copy the element "landmark" matrix for faster access.
  Element::MatrixType Le;

  e->GetLandmarkContributionMatrix(eta, Le);

  // ... same for number of DOF
  int Ne = e->GetNumberOfDegreesOfFreedom();

  // step over all rows in element matrix
  for ( int j = 0; j < Ne; j++ )
    {
    // step over all columns in element matrix
    for ( int k = 0; k < Ne; k++ )
      {
      // error checking. all GFN should be =>0 and <NGFN
      if ( e->GetDegreeOfFreedom(j) >= NGFN
           || e->GetDegreeOfFreedom(k) >= NGFN  )
        {
        throw FEMExceptionSolution(__FILE__, __LINE__, "FEMObject::AssembleLandmarkContribution()", "Illegal GFN!");
        }

      /**
       * Here we finaly update the corresponding element
       * in the master stiffness matrix. We first check if
       * element in Le is zero, to prevent zeros from being
       * allocated in sparse matrix.
       */
      if ( Le[j][k] != float(0.0) )
        {
        this->m_ls->AddMatrixValue(e->GetDegreeOfFreedom(j), e->GetDegreeOfFreedom(k), Le[j][k]);
        }
      }
    }
}

template<unsigned int VDimension>
void FEMObject<VDimension>::AssembleElementMatrix(Element::Pointer e)
{
  // Copy the element stiffness matrix for faster access.
  Element::MatrixType Ke;

  e->GetStiffnessMatrix(Ke);

  // ... same for number of DOF
  int Ne = e->GetNumberOfDegreesOfFreedom();

  // step over all rows in element matrix
  for ( int j = 0; j < Ne; j++ )
    {
    // step over all columns in element matrix
    for ( int k = 0; k < Ne; k++ )
      {
      // error checking. all GFN should be =>0 and <NGFN
      if ( e->GetDegreeOfFreedom(j) >= NGFN
           || e->GetDegreeOfFreedom(k) >= NGFN  )
        {
        throw FEMExceptionSolution(__FILE__, __LINE__, "FEMObject::AssembleElementMatrix()", "Illegal GFN!");
        }

      /**
       * Here we finaly update the corresponding element
       * in the master stiffness matrix. We first check if
       * element in Ke is zero, to prevent zeros from being
       * allocated in sparse matrix.
       */
      if ( Ke[j][k] != float(0.0) )
        {
        this->m_ls->AddMatrixValue(e->GetDegreeOfFreedom(j), e->GetDegreeOfFreedom(k), Ke[j][k]);
        }
      }
    }
}

/**
 * Assemble the master force vector
 */
template<unsigned int VDimension>
void FEMObject<VDimension>::AssembleF(int dim)
{
  // Vector that stores element nodal loads
  Element::VectorType Fe;

  // Type that stores IDs of fixed DOF together with the values to
  // which they were fixed.
  typedef std::map< Element::DegreeOfFreedomIDType, float > BCTermType;
  BCTermType bcterm;

  /* if no DOFs exist in a system, we have nothing to do */
  if ( NGFN <= 0 ) { return; }

  /* Initialize the master force vector */
  m_ls->InitializeVector();

  /**
   * Convert the external loads to the nodal loads and
   * add them to the master force vector F.
   */
   int numLoads = this->m_LoadContainer->Size();
  for ( int l = 0; l <numLoads; l++ )
    {
    /**
     * Store a temporary pointer to load object for later,
     * so that we don't have to access it via the iterator
     */
    Load::Pointer l0 = this->GetLoad(l);

    /**
     * Pass the vector to the solution to the Load object.
     */
    l0->SetSolution(m_ls);

    /**
     * Here we only handle Nodal loads
     */
    if ( LoadNode::Pointer l1 = dynamic_cast< LoadNode * >( &*l0 ) )
      {
      // yep, we have a nodal load

      // size of a force vector in load must match number of DOFs in node
      // changes made - kiran
      //if ( (l1->F.size() %
      // l1->m_element->GetNumberOfDegreesOfFreedomPerNode()) != 0 )
      if ( ( l1->GetForce().size() % l1->GetElement()->GetNumberOfDegreesOfFreedomPerNode() ) != 0 )
      // changes made - kiran
        {
        throw FEMExceptionSolution(__FILE__,
                                   __LINE__,
                                   "FEMObject::AssembleF()",
                                   "Illegal size of a force vector in LoadNode object!");
        }

      // we simply copy the load to the force vector
      // changes made - kiran
      //for(unsigned int d=0; d <
      // (l1->m_element->GetNumberOfDegreesOfFreedomPerNode()); d++)
      // changes made - kiran
      //      {
      //	  Element::DegreeOfFreedomIDType
      // dof=l1->m_element->GetNode(l1->m_pt)->GetDegreeOfFreedom(d);
      for ( unsigned int d = 0; d < ( l1->GetElement()->GetNumberOfDegreesOfFreedomPerNode() ); d++ )
        {
        Element::DegreeOfFreedomIDType dof = l1->GetElement()->GetNode( l1->GetNode() )->GetDegreeOfFreedom(d);
        // changes made - kiran
        // error checking
        if ( dof >= NGFN )
          {
          throw FEMExceptionSolution(__FILE__, __LINE__, "FEMObject::AssembleF()", "Illegal GFN!");
          }

        /**
         * If using the extra dim parameter, we can apply the force to
         * different isotropic dimension.
         *
         * FIXME: We assume that the implementation of force vector
         * inside the LoadNode class is correct for given number of
         * dimensions
         */
        // changes made - kiran
        //m_ls->AddVectorValue(dof ,
        // l1->F[d+l1->m_element->GetNumberOfDegreesOfFreedomPerNode()*dim]);
        m_ls->AddVectorValue(dof, l1->GetForce()
                             [d + l1->GetElement()->GetNumberOfDegreesOfFreedomPerNode() * dim]);
        // changes made - kiran
        }

      // that's all there is to DOF loads, go to next load in an array
      continue;
      }

    /**
     * Element loads...
     */
    if ( LoadElement::Pointer l1 = dynamic_cast< LoadElement * >( &*l0 ) )
      {
      // changes made - kiran
      // if ( !(l1->el.empty()) )
      if ( !( l1->GetElementArray().empty() ) )
      // changes made - kiran
        {
        /**
         * If array of element pointers is not empty,
         * we apply the load to all elements in that array
         */
        // changes made - kiran
        //for(LoadElement::ElementPointersVectorType::const_iterator
        // i=l1->el.begin(); i != l1->el.end(); i++)
        for ( LoadElement::ElementPointersVectorType::const_iterator i = l1->GetElementArray().begin();
              i != l1->GetElementArray().end(); i++ )
        // changes made - kiran
          {
          const Element *el0 = ( *i );
          // Call the Fe() function of the element that we are applying the load
          // to.
          // We pass a pointer to the load object as a paramater and a reference
          // to the nodal loads vector.
          el0->GetLoadVector(Element::LoadPointer(l1), Fe);
          unsigned int Ne = el0->GetNumberOfDegreesOfFreedom(); // ... element's
                                                                // number of DOF
          for ( unsigned int j = 0; j < Ne; j++ )               // step over all
                                                                // DOF
            {
            // error checking
            if ( el0->GetDegreeOfFreedom(j) >= NGFN )
              {
              throw FEMExceptionSolution(__FILE__, __LINE__, "FEMObject::AssembleF()", "Illegal GFN!");
              }

            // update the master force vector (take care of the correct
            // isotropic dimensions)
            m_ls->AddVectorValue( el0->GetDegreeOfFreedom(j), Fe(j + dim * Ne) );
            }
          }
        }
      else
        {
        /**
         * If the list of element pointers in load object is empty,
         * we apply the load to all elements in a system.
         */
         int numElements = this->m_ElementContainer->Size();
        for ( int e = 0; e < numElements; e++ ) //
                                                                          // step
                                                                          // over
                                                                          // all
                                                                          // elements
                                                                          // in
                                                                          // a
                                                                          // system
          {
          Element::Pointer el = this->GetElement(e);
          el->GetLoadVector(Element::LoadPointer(l1), Fe);     // ...
                                                                   // element's
                                                                   // force
                                                                   // vector
          unsigned int Ne = el->GetNumberOfDegreesOfFreedom(); // ...
                                                                   // element's
                                                                   // number of
                                                                   // DOF

          for ( unsigned int j = 0; j < Ne; j++ )  // step over all DOF
            {
            if ( el->GetDegreeOfFreedom(j) >= NGFN )
              {
              throw FEMExceptionSolution(__FILE__, __LINE__, "FEMObject::AssembleF()", "Illegal GFN!");
              }

            // update the master force vector (take care of the correct
            // isotropic dimensions)
            m_ls->AddVectorValue( el->GetDegreeOfFreedom(j), Fe(j + dim * Ne) );
            }
          }
        }

      // skip to next load in an array
      continue;
      }

    /**
     * Handle boundary conditions in form of MFC loads are handled next.
     */
    if ( LoadBCMFC::Pointer l1 = dynamic_cast< LoadBCMFC * >( &*l0 ) )
      {
      // changes made - kiran
      //m_ls->SetVectorValue(NGFN+l1->Index , l1->rhs[dim]);
      m_ls->SetVectorValue( NGFN + l1->GetIndex(), l1->GetRightHandSideTerm(dim) );
      // changes made - kiran

      // skip to next load in an array
      continue;
      }

    /**
     * Handle essential boundary conditions.
     */
    if ( LoadBC::Pointer l1 = dynamic_cast< LoadBC * >( &*l0 ) )
      {
      // Here we just store the values of fixed DOFs. We can't set it here,
      // because
      // it may be changed by other loads that are applied later.

      // changes made - kiran
      //bcterm[ l1->m_element->GetDegreeOfFreedom(l1->m_dof) ]=l1->m_value[dim];
      bcterm[l1->GetElement()->GetDegreeOfFreedom( l1->GetDegreeOfFreedom() )] =
        l1->GetValue()[dim];
      // changes made - kiran

      // skip to the next load in an array
      continue;
      }

    /**
     * If we got here, we were unable to handle that class of Load object.
     * We do nothing...
     */
    }  // for(LoadArray::iterator l ... )

  /**
   * Adjust the master force vector for essential boundary
   * conditions as required.
   */
  if ( m_ls->IsVectorInitialized(1) )
    {
    // Add the vector generated by ApplyBC to the solution vector
    const unsigned int totGFN = NGFN + NMFC;
    for ( unsigned int i = 0; i < totGFN; i++ )
      {
      m_ls->AddVectorValue( i, m_ls->GetVectorValue(i, 1) );
      }
    }

  // Set the fixed DOFs to proper values
  for ( BCTermType::iterator q = bcterm.begin(); q != bcterm.end(); q++ )
    {
    m_ls->SetVectorValue(q->first, q->second);
    }
}

/**
 * Decompose matrix using svd, qr, whatever ... if needed
 */


/**
 * Solve for the displacement vector u
 */
template<unsigned int VDimension>
void FEMObject<VDimension>::Solve()
{
  this->GenerateGFN();

  this->AssembleK();

  this->AssembleF();
  
  // Check if master stiffness matrix and master force vector were
  // properly initialized.
  if ( !m_ls->IsMatrixInitialized() )
    {
    throw FEMExceptionSolution(__FILE__, __LINE__, "FEMObject::Solve()", "Master stiffness matrix was not initialized!");
    }
  if ( !m_ls->IsVectorInitialized() )
    {
    throw FEMExceptionSolution(__FILE__, __LINE__, "FEMObject::Solve()", "Master force vector was not initialized!");
    }

  // Solve the system of linear equations
  m_ls->InitializeSolution();
  m_ls->Solve();
}

/**
 * Apply the boundary conditions to the system.
 */
template<unsigned int VDimension>
void FEMObject<VDimension>::ApplyBC(int dim, unsigned int matrix)
{
  // Vector with index 1 is used to store force correctios for BCs
  m_ls->DestroyVector(1);

  /* Step over all Loads */
  int numLoads = this->m_LoadContainer->Size();
  for ( int l = 0; l<numLoads; l++ )
    {
    /**
     * Store a temporary pointer to load object for later,
     * so that we don't have to access it via the iterator
     */
    Load::Pointer l0 = GetLoad(l);

    /**
     * Apply boundary conditions in form of MFC loads.
     *
     * We add the multi freedom constraints contribution to the master
     * stiffness matrix using the lagrange multipliers. Basically we only
     * change the last couple of rows and columns in K.
     */
    if ( LoadBCMFC::Pointer c = dynamic_cast< LoadBCMFC * >( &*l0 ) )
      {
      /* step over all DOFs in MFC */
      // changes made - kiran
      //     for(LoadBCMFC::LhsType::iterator q=c->lhs.begin();
      //       q != c->lhs.end();
      for ( LoadBCMFC::LhsType::iterator q = c->GetLeftHandSideArray().begin();
            q != c->GetLeftHandSideArray().end();
            // changes made - kiran
            q++ )
        {
        /* obtain the GFN of DOF that is in the MFC */
        Element::DegreeOfFreedomIDType gfn =
          q->m_element->GetDegreeOfFreedom(q->dof);

        /* error checking. all GFN should be =>0 and <NGFN */
        if ( gfn >= NGFN )
          {
          throw FEMExceptionSolution(__FILE__, __LINE__, "FEMObject::ApplyBC()", "Illegal GFN!");
          }

        /* set the proper values in matster stiffnes matrix */
        //changes made - kiran
        //this->m_ls->SetMatrixValue(gfn, NGFN+c->Index, q->value, matrix);
        //this->m_ls->SetMatrixValue(NGFN+c->Index, gfn, q->value, matrix);  //
        // this is a symetric matrix...
        this->m_ls->SetMatrixValue(gfn, NGFN + c->GetIndex(), q->value, matrix);
        this->m_ls->SetMatrixValue(NGFN + c->GetIndex(), gfn, q->value, matrix); //
                                                                                 // this
                                                                                 // is
                                                                                 // a
                                                                                 // symetric
                                                                                 // matrix...
        // changes made - kiran
        }

      // skip to next load in an array
      continue;
      }

    /**
     * Apply essential boundary conditions
     */
    if ( LoadBC::Pointer c = dynamic_cast< LoadBC * >( &*l0 ) )
      {
      // changes made - kiran
      /*Element::DegreeOfFreedomIDType fdof = c->m_element->GetDegreeOfFreedom(c->m_dof);
      float fixedvalue=c->m_value[dim];*/
      Element::DegreeOfFreedomIDType fdof = c->GetElement()->GetDegreeOfFreedom( c->GetDegreeOfFreedom() );
      float                          fixedvalue = c->GetValue()[dim];
      // changes made - kiran

      // Copy the corresponding row of the matrix to the vector that will
      // be later added to the master force vector.
      // NOTE: We need to copy the whole row first, and then clear it. This
      //       is much more efficient when using sparse matrix storage, than
      //       copying and clearing in one loop.

      // Get the column indices of the nonzero elements in an array.
      LinearSystemWrapper::ColumnArray cols;
      m_ls->GetColumnsOfNonZeroMatrixElementsInRow(fdof, cols, matrix);

      // Force vector needs updating only if DOF was not fixed to 0.0.
      if ( fixedvalue != 0.0 )
        {
        // Initialize the master force correction vector as required
        if ( !this->m_ls->IsVectorInitialized(1) )
          {
          this->m_ls->InitializeVector(1);
          }

        // Step over each nonzero matrix element in a row
        for ( LinearSystemWrapper::ColumnArray::iterator cc = cols.begin(); cc != cols.end(); cc++ )
          {
          // Get value from the stiffness matrix
          float d = this->m_ls->GetMatrixValue(fdof, *cc, matrix);

          // Store the appropriate value in bc correction vector (-K12*u2)
          //
          // See
          // http://titan.colorado.edu/courses.d/IFEM.d/IFEM.Ch04.d/IFEM.Ch04.pdf
          // chapter 4.1.3 (Matrix Forms of DBC Application Methods) for more
          // info.
          this->m_ls->AddVectorValue(*cc, -d * fixedvalue, 1);
          }
        }

      // Clear that row and column in master matrix
      for ( LinearSystemWrapper::ColumnArray::iterator cc = cols.begin(); cc != cols.end(); cc++ )
        {
        this->m_ls->SetMatrixValue(fdof, *cc, 0.0, matrix);
        this->m_ls->SetMatrixValue(*cc, fdof, 0.0, matrix); // this is a
                                                            // symetric matrix
        }
      this->m_ls->SetMatrixValue(fdof, fdof, 1.0, matrix); // Set the diagonal
                                                           // element to one

      // skip to next load in an array
      continue;
      }
    } // end for LoadArray::iterator l
}

template<unsigned int VDimension>
void FEMObject<VDimension>::AddNextElement(Element::Pointer e)
{
  ElementIdentifier size = this->m_ElementContainer->Size();
  this->m_ElementContainer->InsertElement(size, e);
}

template<unsigned int VDimension>
void FEMObject<VDimension>::InsertElement(Element::Pointer e, ElementIdentifier index)
{
    this->m_ElementContainer->InsertElement(index, e);
}

template<unsigned int VDimension>
void FEMObject<VDimension>::AddNextNode(Node::Pointer e)
{
  NodeIdentifier size = this->m_NodeContainer->Size();
  this->m_NodeContainer->InsertElement(size, e);
}

template<unsigned int VDimension>
void FEMObject<VDimension>::InsertNode(Node::Pointer e, NodeIdentifier index)
{
    this->m_NodeContainer->InsertElement(index, e);
}

template<unsigned int VDimension>
void FEMObject<VDimension>::AddNextMaterial(Material::Pointer e)
{
  MaterialIdentifier size = this->m_MaterialContainer->Size();
  this->m_MaterialContainer->InsertElement(size, e);
}

template<unsigned int VDimension>
void FEMObject<VDimension>::InsertMaterial(Material::Pointer e, MaterialIdentifier index)
{
    this->m_MaterialContainer->InsertElement(index, e);
}

template<unsigned int VDimension>
void FEMObject<VDimension>::AddNextLoad(Load::Pointer e)
{
  LoadIdentifier size = this->m_LoadContainer->Size();
  this->m_LoadContainer->InsertElement(size, e);
}

template<unsigned int VDimension>
void FEMObject<VDimension>::InsertLoad(Load::Pointer e, LoadIdentifier index)
{
    this->m_LoadContainer->InsertElement(index, e);
}

template<unsigned int VDimension>
Element::Pointer FEMObject<VDimension>::GetElement(ElementIdentifier index)
{
  return this->m_ElementContainer->GetElement(index);
}

template<unsigned int VDimension>
Element::Pointer FEMObject<VDimension>::GetElementWithGlobalNumber(ElementIdentifier index)
{
 int numElements = this->m_ElementContainer->Size();
 for (int i=0; i<numElements; i++)
 {
	if(this->m_ElementContainer->GetElement(i)->GetGlobalNumber() == index)
	{
		return	this->m_ElementContainer->GetElement(i);
	}
 }
 return NULL; 
}

template<unsigned int VDimension>
Node::Pointer FEMObject<VDimension>::GetNode(NodeIdentifier index)
{
  return this->m_NodeContainer->GetElement(index);
}

template<unsigned int VDimension>
Node::Pointer FEMObject<VDimension>::GetNodeWithGlobalNumber(NodeIdentifier index)
{
 int numNodes = this->m_NodeContainer->Size();
 for (int i=0; i<numNodes; i++)
 {
	if(this->m_NodeContainer->GetElement(i)->GetGlobalNumber() == index)
	{
		return	this->m_NodeContainer->GetElement(i);
	}
 }
 return NULL; 
}

template<unsigned int VDimension>
Load::Pointer FEMObject<VDimension>::GetLoad(LoadIdentifier index)
{
  return this->m_LoadContainer->GetElement(index);
}

template<unsigned int VDimension>
Load::Pointer FEMObject<VDimension>::GetLoadWithGlobalNumber(LoadIdentifier index)
{
 int numLoads = this->m_LoadContainer->Size();
 for (int i=0; i<numLoads; i++)
 {
	if(this->m_LoadContainer->GetElement(i)->GetGlobalNumber() == index)
	{
		return	this->m_LoadContainer->GetElement(i);
	}
 }
 return NULL; 
}

template<unsigned int VDimension>
Material::Pointer FEMObject<VDimension>::GetMaterial(MaterialIdentifier index)
{
  return this->m_MaterialContainer->GetElement(index);
}

template<unsigned int VDimension>
Material::Pointer FEMObject<VDimension>::GetMaterialWithGlobalNumber(MaterialIdentifier index)
{
 int numMaterials = this->m_MaterialContainer->Size();
 for (int i=0; i<numMaterials; i++)
 {
	if(this->m_MaterialContainer->GetElement(i)->GetGlobalNumber() == index)
	{
		return	this->m_MaterialContainer->GetElement(i);
	}
 }
 return NULL; 
}

template<unsigned int VDimension>
void FEMObject<VDimension>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
}
} // end namespace itk::fem
