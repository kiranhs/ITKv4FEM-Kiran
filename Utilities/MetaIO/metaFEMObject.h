/*============================================================================
  MetaIO
  Copyright 2000-2010 Insight Software Consortium

  Distributed under the OSI-approved BSD License (the "License");
  see accompanying file Copyright.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the License for more information.
============================================================================*/
#include "metaTypes.h"

#ifndef ITKMetaIO_METAFEMOBJECT_H
#define ITKMetaIO_METAFEMOBJECT_H

#include "metaUtils.h"
#include "metaObject.h"

#ifdef _MSC_VER
#pragma warning ( disable: 4251 )
#endif

#include <list>
#include <vector>

#if (METAIO_USE_NAMESPACE)
namespace METAIO_NAMESPACE {
#endif

// helper classes to store the information read in from FEM mesh
/** Define a fem node */
class METAIO_EXPORT FEMObjectNode
{
public:

  FEMObjectNode(int dim);
  ~FEMObjectNode();
  
  unsigned int m_Dim;
  float* m_X;
  int m_GN;	// global number used in FEM namespace
};


/** Define a mesh cell 
 *  a cell contains a list of Ids corresponding to the list 
 *  of points */
class METAIO_EXPORT FEMObjectElement
{
public:

  FEMObjectElement(int dim);
  ~FEMObjectElement();
  
  int m_GN;
  char m_ElementName[256]; // class name
  unsigned int m_Dim;
  unsigned int m_NumNodes;
  unsigned int m_MaterialGN;
  int* m_NodesId;
};

/** Define a mesh cell 
 *  a cell contains a list of Ids corresponding to the list 
 *  of points */
class METAIO_EXPORT FEMObjectMaterial
{
public:

  FEMObjectMaterial()
  {
  }
  ~FEMObjectMaterial()
  {
  }
  
  int m_GN;
  char m_MaterialName[256]; // material name
  double E;
  double A;
  double I;
  double nu;
  double h;
  double RhoC;
};

 class METAIO_EXPORT FEMObjectMFCTerm
  {
public:
    /**
     * Pointer to element, which holds the DOF that is affected by MFC
     */
    unsigned int m_ElementGN;

    /**
     * DOF number within the Element object
     */
    unsigned int m_DOF;

    /**
     * Value with which this displacement is multiplied on the lhs of MFC equation
     */
    float m_Value;

    /**
     * Constructor for easy object creation.
     */
    FEMObjectMFCTerm(unsigned int element_, unsigned int dof_,
            float value_):m_ElementGN(element_), m_DOF(dof_), m_Value(value_) {}
  };

class METAIO_EXPORT FEMObjectLoad
{
public:

	FEMObjectLoad();
	~FEMObjectLoad();

	int m_GN;
	char m_LoadName[256]; // load name
	int m_ElementGN;
	int m_Dim;
	METAIO_STL::vector<float> m_ForceVector;
	int m_DOF;
	int m_NodeNumber;
	int m_NumRHS;
	METAIO_STL::vector<float> m_RHS;
	int m_NumLHS;
	METAIO_STL::vector<FEMObjectMFCTerm*> m_LHS;
	int m_NumElements;
	METAIO_STL::vector<int> m_Elements;
	METAIO_STL::vector<METAIO_STL::vector<float>> m_ForceMatrix;
	int m_EdgeNumber;
};

class METAIO_EXPORT MetaFEMObject : public MetaObject
  {
  public:


    MetaFEMObject(void);

    MetaFEMObject(const char *_headerName);

    MetaFEMObject(const MetaFEMObject *_femobject);

    MetaFEMObject(unsigned int dim);

    ~MetaFEMObject(void);

    void PrintInfo(void) const;

    void CopyInfo(const MetaObject * _object);

    /** Clear the MetaFEMObject */
    void  Clear(void);

	/** List of valid class name types from FEM namespace*/
	typedef METAIO_STL::list<std::string> ClassNameListType;

	/** List of Node, Element, Material and Load*/
	typedef METAIO_STL::list<FEMObjectNode*> NodeListType;
	typedef METAIO_STL::list<FEMObjectElement*>  ElementListType;
	typedef METAIO_STL::list<FEMObjectMaterial*>  MaterialListType;
	typedef METAIO_STL::list<FEMObjectLoad*>  LoadListType;

	/** Access methods*/
	NodeListType & GetNodeList(void) {return m_NodeList;}
	ElementListType & GetElementList(void) {return m_ElementList;}
	MaterialListType & GetMaterialList(void) {return m_MaterialList;}
	LoadListType & GetLoadList(void) {return m_LoadList;}

  protected:

    void  M_Destroy(void);

    void  M_SetupReadFields(void);

    void  M_SetupWriteFields(void);

    bool  M_Read(void);

    bool  M_Write(void);

	/** For reading and writing in node details */
	bool  M_Read_Node();

	void M_Write_Node(FEMObjectNode *Node);

	void M_Write_Material(FEMObjectMaterial *Material);

	void M_Write_Element(FEMObjectElement *Element);

	void M_Write_Load(FEMObjectLoad *Load);

	/** For reading in element details. The input is the name of the element from FEM namespace */
	bool  M_Read_Element(std::string element_name);

	/** For reading in element details. The input is the name of the element from FEM namespace */
	bool  M_Read_Material(std::string material_name);

	/** For reading in element details. The input is the name of the element from FEM namespace */
	bool  M_Read_Load(std::string load_name);

	/** Read in only the keywords that are in the  'ClassNameListType' list container*/
	bool IsClassNamePresent(std::string c_string);

	/** Global number is common for all entity lists of FEM*/
	int ReadGlobalNumber();

	void  SkipWhiteSpace();

	/** Based on the element name get the number of nodes and the dimension*/
	int* GetElementDimensionAndNumberOfNodes(std::string c_string, int info[2]); 

	// variables
	static const std::string whitespaces;

	ClassNameListType m_ClassNameList;
    ElementListType m_ElementList;
	NodeListType m_NodeList;
	MaterialListType m_MaterialList;
	LoadListType m_LoadList;

	char      m_ElementDataFileName[255];

	// to keep track of the type element created
	enum{
		NODE			= 0,
		MATERIAL        = 1,
		ELEMENT         = 2,
		LOAD			= 3
	};

  };

#if (METAIO_USE_NAMESPACE)
};
#endif


#endif
