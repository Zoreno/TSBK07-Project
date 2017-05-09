/**
 * @file	Collada.h
 * @Author	Joakim Bertils
 * @date	2017-05-09
 * @brief	COLLADA file parser
 */

#pragma once

#include <string>

#include <rapidxml/rapidxml.hpp>

#include <vector>

namespace collada
{

	/**
	 * @brief Type definition of a XML document
	 */
	typedef rapidxml::xml_document<> Document;

	/**
	 * @brief Type definition of a XML node.
	 */
	typedef rapidxml::xml_node<> Node;

	/**
	 * @brief Type definition of a XML attribute.
	 */
	typedef rapidxml::xml_attribute<> Attrib;

	//=========================================================================
	// Geometry Library
	//=========================================================================

	/**
	 * @brief Array of Floats Node
	 * 
	 * Example:
	 * <float_array id="values" count="6"> 0.3 0.5 0.7 0.2 0.4 0.6 </float_array>
	 */
	struct FloatArray
	{
		/**
		 * @brief Number of floatis in array
		 */
		unsigned int count;

		/**
		 * @brief Array ID.
		 */
		std::string ID;

		/**
		 * @brief Array of floats.
		 */
		std::vector<float> floats;
	};

	/**
	 * @brief Array of names
	 */
	struct NameArray
	{
		/**
		 * @brief Number of names.
		 */
		unsigned int count;

		/**
		 * @brief Array ID.
		 */
		std::string ID;

		/**
		 * @brief Array of names.
		 */
		std::vector<std::string> names;
	};

	/**
	 * @brief Param Node.
	 * 
	 * Example:
	 * <param name="X" type="float"/>
	 */
	struct Param
	{
		/**
		 * @brief Name of param
		 */
		std::string name;

		/**
		 * @brief Param type.
		 */
		std::string type;
	};

	/**
	 * @brief Accessor node.
	 * 
	 * Specifies what kind of data the array contains and how it should be accessed.
	 * 
	 * Example:
	 * <accessor count="8" source="#Cube1-0-Pos-array" stride="3">
	 *		<param name="X" type="float"/>
	 *		<param name="Y" type="float"/>
	 *		<param name="Z" type="float"/>
	 * </accessor>
	 */
	struct Accessor
	{
		/**
		 * @brief Source array name.
		 */
		std::string source;

		/**
		 * @brief Number of elements
		 */
		unsigned int count;

		/**
		 * @brief Offset between elements.
		 */
		unsigned int stride;

		/**
		 * @brief Parameter description list.
		 */
		std::vector<Param> params;
	};

	/**
	 * @brief Technique Common Node
	 * 
	 * Defines the purpose of a Source array
	 */
	struct Technique_Common
	{
		/**
		 * @brief Accessor description.
		 */
		Accessor accessor;
	};

	/**
	 * @brief Source Node
	 * 
	 * Defines a source data node. A source node could contain information about vertex positions or normals etc.
	 */
	struct Source
	{
		/**
		 * @brief ID of data node.
		 */
		std::string ID;

		/**
		 * @brief Name of data node.
		 */
		std::string name;

		/**
		 * @brief Array of floats
		 */
		FloatArray floatArray;

		/**
		 * @brief Array of names.
		 */
		NameArray nameArray;

		/**
		 * @brief Tecnique for accessing elements.
		 */
		Technique_Common techniqueCommon;
	};

	/**
	 * @brief Input struct
	 */
	struct Input
	{
		/**
		 * @brief Input semantic.
		 */
		std::string semantic;

		/**
		 * @brief Input source location
		 */
		std::string source;

		/**
		 * @brief Offset into list of indices.
		 */
		std::string offset;

		/**
		 * @brief Containing set.
		 */
		std::string set;
	};

	/**
	 * @brief Describes attributes of vertices.
	 */
	struct Vertices
	{
		/**
		 * @brief ID of vertex.
		 */
		std::string ID;

		/**
		 * @brief List of inputs.
		 */
		std::vector<Input> inputs;
	};

	/**
	 * @brief Primitive index descriptor
	 */
	struct P
	{
		/**
		 * @brief Number of indices.
		 */
		unsigned int count;

		/**
		 * @brief List of indices.
		 */
		std::vector<unsigned short> indices;
	};

	/**
	 * @brief Triangles node.
	 */
	struct Triangles
	{
		/**
		 * @brief Number of triangles
		 */
		unsigned int count;

		/**
		 * @brief Material symbol
		 */
		std::string material;

		/**
		 * @brief Input list
		 */
		std::vector<Input> inputs;
		
		/**
		 * @brief Indices list
		 */
		P p;
	};

	/**
	 * @brief Vertex count list.
	 */
	struct VCount
	{
		/**
		 * @brief Number of vertices.
		 */
		unsigned int count;

		/**
		 * @brief Indices
		 */
		std::vector<unsigned short> indices;
	};

	/**
	 * @brief Polygon list
	 */
	struct PolyList
	{
		/**
		 * @brief Number of polygon lists.
		 */
		unsigned int count;

		/**
		 * @brief Name of element.
		 */
		std::string name;

		/**
		 * @brief Material symbol.
		 */
		std::string material;

		/**
		 * @brief List of inputs.
		 */
		std::vector<Input> inputs;
		
		/**
		 * @brief List of vertex counts for polygons.
		 */
		VCount vcount;
		
		/**
		 * @brief Specifies vertex attributes for a polylist.
		 */
		P p;
	};

	/**
	 * @brief Describes geometric meshes.
	 */
	struct Mesh
	{
		/**
		 * @brief Mesh sources.
		 */
		std::vector<Source> sources;

		/**
		 * @brief Mesh vertex attributes.
		 */
		Vertices vertices;

		/**
		 * @brief List of triangles.
		 */
		std::vector<Triangles> triangles;
		
		/**
		 * @brief List of polygon lists.
		 */
		std::vector<PolyList> polyLists;
	};

	/**
	 * @brief Describes visual shape of objects.
	 */
	struct Geometry
	{
		/**
		 * @brief ID of geometry element.
		 */
		std::string ID;

		/**
		 * @brief Name of element.
		 */
		std::string name;

		/**
		 * @brief The geometry's mesh
		 */
		Mesh mesh;
	};

	/**
	 * @brief Geometry library object
	 */
	struct GeometryLibrary
	{
		/**
		 * @brief List of all geometries in file.
		 */
		std::vector<Geometry> geometries;
	};

	//=========================================================================
	// Images Library
	//=========================================================================

	/**
	 * @brief Image Initializing source.
	 */
	struct InitFrom
	{
		/**
		 * @brief File name of image.
		 */
		std::string fileName;
	};

	/**
	 * @brief Image object
	 */
	struct Image
	{
		/**
		 * @brief Initializing source.
		 */
		InitFrom initFrom;
	};

	/**
	 * @brief Image library object
	 */
	struct ImagesLibrary
	{
		/**
		 * @brief List of images.
		 */
		std::vector<Image> images;
	};

	//=========================================================================
	// Collada Struct
	//=========================================================================

	/**
	 * @brief Collada file object.
	 */
	struct COLLADA
	{
		/**
		 * @brief Geometry Library
		 */
		GeometryLibrary geometryLibrary;

		/**
		 * @brief Images library
		 */
		ImagesLibrary imagesLibrary;
	};

	/**
	 * @brief Loads a float array
	 * @param floatArrayNode Source
	 * @param floatArray Destination
	 */
	void loadFloatArray(Node* floatArrayNode, FloatArray* floatArray);

	/**
	 * @brief Loads a name array
	 * @param nameArrayNode Source
	 * @param nameArray Destination
	 */
	void loadNameArray(Node* nameArrayNode, NameArray* nameArray);

	/**
	 * @brief Loads a param node
	 * @param paramNode Source
	 * @param param Destination
	 */
	void loadParam(Node* paramNode, Param* param);

	/**
	 * @brief Loads an accessor node.
	 * @param accessorNode Source
	 * @param accessor Destination
	 */
	void loadAccessor(Node* accessorNode, Accessor* accessor);

	/**
	 * @brief Loads a technique_common node
	 * @param techniqueCommonNode Source
	 * @param techniqueCommon Destination
	 */
	void loadTechnique_Common(Node* techniqueCommonNode, Technique_Common* techniqueCommon);

	/**
	 * @brief Loads a source node
	 * @param sourceNode Source
	 * @param source Destination
	 */
	void loadSource(Node* sourceNode, Source* source);

	/**
	 * @brief Loads an input node.
	 * @param inputNode Source
	 * @param input Destination
	 */
	void loadInput(Node* inputNode, Input* input);

	/**
	 * @brief Loads a vertices node
	 * @param verticesNode Source
	 * @param vertices Destination
	 */
	void loadVertices(Node* verticesNode, Vertices* vertices);

	/**
	 * @brief Loads a <p>-node
	 * @param pNode Source
	 * @param p Destination
	 */
	void loadP(Node* pNode, P* p);

	/**
	 * @brief Loads a triangles node
	 * @param trianglesNode Source
	 * @param triangles Destination
	 */
	void loadTriangles(Node* trianglesNode, Triangles* triangles);

	/**
	 * @brief Loads a vcount node.
	 * @param vCountNode Source
	 * @param vCount Destination
	 */
	void loadVCount(Node* vCountNode, VCount* vCount);

	/**
	 * @brief Loads a PolyList Node
	 * @param polyListNode Source
	 * @param polyList Destination
	 */
	void loadPolyList(Node* polyListNode, PolyList* polyList);

	/**
	 * @brief Loads a mesh node
	 * @param meshNode Source
	 * @param mesh Destination
	 */
	void loadMesh(Node* meshNode, Mesh* mesh);

	/**
	 * @brief Loads a geometry node
	 * @param geometryNode Source
	 * @param geometry Destination
	 */
	void loadGeometry(Node* geometryNode, Geometry* geometry);

	/**
	 * @brief Loads a geometry library node
	 * @param geometryLibraryNode Source
	 * @param geometryLibrary Destination
	 */
	void loadGeometryLibrary(Node* geometryLibraryNode, GeometryLibrary* geometryLibrary);

	/**
	 * @brief Loads a COLLADA file
	 * @param fileName File path
	 * @param collada Struct to load info to.
	 */
	void loadCOLLADA(const char* fileName, COLLADA* collada);
}