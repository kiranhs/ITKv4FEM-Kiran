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
 
#include "itkDataObject.h"

#include "itkFEMElementBase.h"
#include "itkFEMLinearSystemWrapper.h"
#include "itkFEMLinearSystemWrapperVNL.h"
#include "itkFEMLoadBase.h"
#include "itkFEMMaterialBase.h"
#include "itkVectorContainer.h"

#ifndef __itkFEMObject_h
#define __itkFEMObject_h
namespace itk
{
namespace fem
{
/** \class FEMObject
 * \brief Implements N-dimensional Finite element (FE) models including
 *   elements, materials, and loads.
 *
 * \par Overview
 * FEMObject was created to provide an object in ITK that specifies
 * the entire FE model. This model can then be passed to the itk::fem::Solver
 * to generate a solution for the model. The design for this class was modeled
 * after the itk::Mesh structure. Presently, no direct I/O support for 
 * the FEMObject exists. This must be done using the FEMSpatialObject.
 * The FEMObject simply serves as a storage container for the FE model.
 *
 * The FEMObject stores the FE problem using Vector Containers for
 *   1) Load
 *   2) Material
 *   3) Element
 *   4) Node
 *
 * \par Usage
 * The user can set the Vector Containers that define the Load,
 * Material, Element, and Nodes using the AddNext<Object> and
 * Insert<Object> methods. The user can also get the entire
 * VectorContainer using the Get<Object>Container(). For convience
 * methods are also provided to get any item in the vector containers
 * based on their index (Get<Object>) or their global number
 * (Get<Object>WithGlobalNumber). This class does not know anything
 * about the types of elements, materials, elements, or nodes. The 
 * problem presently can only be 2D or 3D.
 *
 * \ingroup FEM
 */
 
template <unsigned int VDimension = 3>
class ITK_EXPORT FEMObject : public DataObject
{
public:
	/** Standard class typedefs. */
	typedef FEMObject                  Self;
	typedef DataObject                Superclass;
	typedef SmartPointer<Self>        Pointer;
	typedef SmartPointer<const Self>  ConstPointer;

	/** Method for creation through the object factory. */
	itkNewMacro(Self);

	/** Standard part of every itk Object. */
	itkTypeMacro(FEMObject, DataObject);

	itkStaticConstMacro(FEMDimension, unsigned int, VDimension);
	itkStaticConstMacro(MaxDimensions, unsigned int, 3);

	typedef unsigned long ElementIdentifier;
	typedef unsigned long NodeIdentifier;
	typedef unsigned long LoadIdentifier;
	typedef unsigned long MaterialIdentifier;

	/** Vector containers for 1) Load, 2) Material, 3) Element and 4) Node. */
	typedef VectorContainer< LoadIdentifier, Load::Pointer >         LoadContainerType;
	typedef VectorContainer< MaterialIdentifier, Material::Pointer > MaterialContainerType;
	typedef VectorContainer< ElementIdentifier, Element::Pointer >   ElementContainerType;
	typedef VectorContainer< NodeIdentifier, Node::Pointer >         NodeContainerType;

	/** Create types that are pointers to each of the container types. */
	typedef typename ElementContainerType::Pointer             ElementContainerPointer;
	typedef typename ElementContainerType::ConstPointer        ElementContainerConstPointer;
	typedef typename NodeContainerType::Pointer                NodeContainerPointer;
	typedef typename NodeContainerType::ConstPointer           NodeContainerConstPointer;
	typedef typename LoadContainerType::Pointer                LoadContainerPointer;
	typedef typename LoadContainerType::ConstPointer           LoadContainerConstPointer;
	typedef typename MaterialContainerType::Pointer            MaterialContainerPointer;
	typedef typename MaterialContainerType::ConstPointer       MaterialContainerConstPointer;


	/** Create types that are iterators for each of the container types. */
	typedef typename
		ElementContainerType::ConstIterator         ElementContainerConstIterator;
	typedef typename
		ElementContainerType::Iterator              ElementContainerIterator;
	typedef typename
		NodeContainerType::ConstIterator            NodeContainerConstIterator;
	typedef typename
		NodeContainerType::Iterator                 NodeContainerIterator;
	typedef typename
		LoadContainerType::ConstIterator            LoadContainerConstIterator;
	typedef typename
		LoadContainerType::Iterator                 LoadContainerIterator;
	typedef typename
		MaterialContainerType::ConstIterator        MaterialContainerConstIterator;
	typedef typename
		MaterialContainerType::Iterator             MaterialContainerIterator;

	// Get methods to get the entire VectorContainers for Elements, Nodes, Loads, and Materials
	itkGetObjectMacro(ElementContainer, ElementContainerType);
	itkGetObjectMacro(NodeContainer, NodeContainerType);
	itkGetObjectMacro(LoadContainer, LoadContainerType);
	itkGetObjectMacro(MaterialContainer, MaterialContainerType);
	
	/** To access the solution. Solution obtained is the rsulting nodal displacements*/
	float GetSolution(unsigned int i, unsigned int which = 0)
	{
		return m_ls->GetSolutionValue(i, which);
	}

	/** Get the Degrees of Freedom for the FE model */
	unsigned int GetNumberOfDegreesOfFreedom(void)
	{
		return NGFN;
	}

	/** Get the Number of nodes in the FE mesh */
	unsigned int GetNumberOfNodes(void)
	{
		return m_NodeContainer->Size();
	}

    /** Get the Number of elements in the FE mesh */
	unsigned int GetNumberOfElements(void)
	{
		return m_ElementContainer->Size();
	}

    /** Get the Number of Loads in the FE problem */
	unsigned int GetNumberOfLoads(void)
	{
		return m_LoadContainer->Size();
	}

	/** Get the Number of Materials in the FE problem */
	unsigned int GetNumberOfMaterials(void)
	{
		return m_MaterialContainer->Size();
	}
	
  /**
   * Sets the LinearSystemWrapper object that will be used when solving
   * the master equation. If this function is not called, a default VNL linear
   * system representation will be used (class LinearSystemWrapperVNL).
   *
   * \param ls Pointer to an object of class which is derived from
   *           LinearSystemWrapper.
   *
   * \note Once the LinearSystemWrapper object is changed, it is used until
   *       the member function SetLinearSystemWrapper is called again. Since
   *       LinearSystemWrapper object was created outside the Solver class, it
   *       should also be destroyed outside. Solver class will not destroy it
   *       when the Solver object is destroyed.
   */
  void SetLinearSystemWrapper(LinearSystemWrapper::Pointer ls);

  /**
   * Gets the LinearSystemWrapper object.
   *
   * \sa SetLinearSystemWrapper
   */
  LinearSystemWrapper::Pointer GetLinearSystemWrapper() { return m_ls; }

  /**
   * Solve for the displacement vector u. May be overriden in derived classes.
   */
  virtual void Solve(void);

   /**
   * Add next element to the element array
   */
  void AddNextElement(Element::Pointer e);

  /**
  * Insert an element at the specified location
  */
  void InsertElement(Element::Pointer e, ElementIdentifier index);

  /**
  * Add next node to the node array
  */
  void AddNextNode(Node::Pointer e);

  /**
  * Insert a node at the specified index location
  */
  void InsertNode(Node::Pointer e, NodeIdentifier index);

  /**
   * Add next material data to the material array
   */
  void AddNextMaterial(Material::Pointer mat);

  /**
   * Insert material data at the specified index location
   */
  void InsertMaterial(Material::Pointer e, MaterialIdentifier index);

  /**
   * Add next load data to the load array
   */
  void AddNextLoad(Load::Pointer ld);

  /**
   * Insert material data at the specified index location
   */
  void InsertLoad(Load::Pointer ld, LoadIdentifier index);

  /**
   * Get the element at the specified index location
   */
  Element::Pointer GetElement(ElementIdentifier index);

  /**
   * Get the element at with the specified global number
   */
  Element::Pointer GetElementWithGlobalNumber(int globalNumber);

  /**
   * Get the node at the specified index location
   */
  Node::Pointer GetNode(NodeIdentifier index);

  /**
   * Get the Node at with the specified global number
   */
  Node::Pointer GetNodeWithGlobalNumber(int globalNumber);

  /**
   * Get the material data at the specified index location
   */
  Material::Pointer GetMaterial(MaterialIdentifier index);

  /**
   * Get the Material at with the specified global number
   */
  Material::Pointer GetMaterialWithGlobalNumber(int globalNumber);

  /**
   * Get the load data at the specified index location
   */
  Load::Pointer GetLoad(LoadIdentifier index);

  /**
   * Get the Load at with the specified global number
   */
  Load::Pointer GetLoadWithGlobalNumber(int globalNumber);

  /**
   * Clear the entire model and return to an initial state
   */
  void Clear();

  /**
   * Renumber the nodes global number based on their current order 
   * in the Node VectorContainer
   */
  void RenumberNodeContainer();

protected:
	/** Constructor for use by New() method. */
	FEMObject();
	~FEMObject();
	void PrintSelf(std::ostream& os, Indent indent) const;

 /**
   * Assign a global freedom numbers to each DOF in a system.
   * This must be done before any other solve function can be called.
   */
  void GenerateGFN(void);

  /**
   * Assemble the master stiffness matrix (also apply the MFCs to K)
   */
  void AssembleK(void);

  /**
   * This function is called before assembling the matrices. You can
   * override it in a derived class to account for special needs.
   *
   * \param N Size of the matrix.
   */
  virtual void InitializeMatrixForAssembly(unsigned int N);

  /**
   * This function is called after the assebly has been completed.
   * In this class it is only used to apply the BCs. You may however
   * use it to perform other stuff in derived solver classes.
   */
  virtual void FinalizeMatrixAfterAssembly(void)
  {
    // Apply the boundary conditions to the K matrix
    this->ApplyBC();
  }

  /**
   * Copy the element stiffness matrix into the correct position in the
   * master stiffess matrix. Since more complex Solver classes may need to
   * assemble many matrices and may also do some funky stuff to them, this
   * function is virtual and can be overriden in a derived solver class.
   */
  virtual void AssembleElementMatrix(Element::Pointer e);

  /**
   * Add the contribution of the landmark-containing elements to the
   * correct position in the master stiffess matrix. Since more
   * complex Solver classes may need to assemble many matrices and may
   * also do some funky stuff to them, this function is virtual and
   * can be overriden in a derived solver class.
   */
  virtual void AssembleLandmarkContribution(Element::Pointer e, float);

  /**
   * Apply the boundary conditions to the system.
   *
   * \note This function must be called after AssembleK().
   *
   * \param matrix Index of a matrix, to which the BCs should be
   *               applied (master stiffness matrix). Normally this
   *               is zero, but in derived classes many matrices may
   *               be used and this index must be specified.
   * \param dim This is a parameter that can be passed to the function and is
   *            normally used with isotropic elements to specify the
   *            dimension in which the DOF is fixed.
   */
  void ApplyBC(int dim = 0, unsigned int matrix = 0);

  /**
   * Assemble the master force vector.
   *
   * \param dim This is a parameter that can be passed to the function and is
   *            normally used with isotropic elements to specify the
   *            dimension for which the master force vector should be assembled.
   */
  void AssembleF(int dim = 0);

   /**
   * Performs any initialization needed for LinearSystemWrapper
   * object i.e. sets the maximum number of matrices and vectors.
   */
  virtual void InitializeLinearSystemWrapper(void);

   /**
   * Number of global degrees of freedom in a system
   */
  unsigned int NGFN;

  /**
   * Number of multi freedom constraints in a system.
   * This member is set in a AssembleK function.
   */
  unsigned int NMFC;

  /** Pointer to LinearSystemWrapper object. */
  LinearSystemWrapper::Pointer m_ls;

  ElementContainerPointer   m_ElementContainer;
  NodeContainerPointer      m_NodeContainer;
  LoadContainerPointer      m_LoadContainer;
  MaterialContainerPointer  m_MaterialContainer;

	 /**
   * LinearSystemWrapperVNL object that is used by default in Solver class.
   */
  LinearSystemWrapperVNL m_lsVNL;

private:
	FEMObject(const Self&); //purposely not implemented
	void operator=(const Self&); //purposely not implemented

}; // End Class: FEMObject

}
}

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkFEMObject.txx"
#endif

#endif // #ifndef __itkFEMObject_h
