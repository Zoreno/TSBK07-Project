#pragma once

#include <string>

#include <rapidxml/rapidxml.hpp>

#include <vector>

namespace collada
{

	typedef rapidxml::xml_document<> Document;
	typedef rapidxml::xml_node<> Node;
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
		unsigned int count;
		std::string ID;
		std::vector<float> floats;
	};

	/**
	 * @brief Array of names
	 */
	struct NameArray
	{
		unsigned int count;
		std::string ID;
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
		std::string name;
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
		std::string source;
		unsigned int count;
		unsigned int stride;
		std::vector<Param> params;
	};

	/**
	 * @brief Technique Common Node
	 * 
	 * Defines the purpose of a Source array
	 */
	struct Technique_Common
	{
		Accessor accessor;
	};

	/**
	 * @brief Source Node
	 * 
	 * Defines a source data node. A source node could contain information about vertex positions or normals etc.
	 */
	struct Source
	{
		std::string ID;
		std::string name;
		FloatArray floatArray;
		NameArray nameArray;
		Technique_Common techniqueCommon;
	};

	struct Input
	{
		std::string semantic;
		std::string source;
		std::string offset;
		std::string set;
	};

	struct Vertices
	{
		std::string ID;
		std::vector<Input> inputs;
	};

	struct P
	{
		unsigned int count;
		std::vector<unsigned short> indices;
	};

	struct Triangles
	{
		unsigned int count;
		std::string material;
		std::vector<Input> inputs;
		P p;
	};

	struct VCount
	{
		unsigned int count;
		std::vector<unsigned short> indices;
	};

	struct PolyList
	{
		unsigned int count;
		std::string name;
		std::string material;
		std::vector<Input> inputs;
		VCount vcount;
		P p;
	};

	struct Mesh
	{
		std::vector<Source> sources;
		Vertices vertices;
		std::vector<Triangles> triangles;
		std::vector<PolyList> polyLists;
	};

	struct Geometry
	{
		std::string ID;
		std::string name;
		Mesh mesh;
	};

	struct GeometryLibrary
	{
		std::vector<Geometry> geometries;
	};

	//=========================================================================
	// Images Library
	//=========================================================================

	struct InitFrom
	{
		std::string fileName;
	};

	struct Image
	{
		InitFrom initFrom;
	};

	struct ImagesLibrary
	{
		std::vector<Image> images;
	};

	//=========================================================================
	// Collada Struct
	//=========================================================================

	struct COLLADA
	{
		GeometryLibrary geometryLibrary;
		ImagesLibrary imagesLibrary;
	};

	void loadFloatArray(Node* floatArrayNode, FloatArray* floatArray);

	void loadNameArray(Node* nameArrayNode, NameArray* nameArray);

	void loadParam(Node* paramNode, Param* param);

	void loadAccessor(Node* accessorNode, Accessor* accessor);

	void loadTechnique_Common(Node* techniqueCommonNode, Technique_Common* techniqueCommon);

	void loadSource(Node* sourceNode, Source* source);

	void loadInput(Node* inputNode, Input* input);

	void loadVertices(Node* verticesNode, Vertices* vertices);

	void loadP(Node* pNode, P* p);

	void loadTriangles(Node* trianglesNode, Triangles* triangles);

	void loadVCount(Node* vCountNode, VCount* vCount);

	void loadPolyList(Node* polyListNode, PolyList* polyList);

	void loadMesh(Node* meshNode, Mesh* mesh);

	void loadGeometry(Node* geometryNode, Geometry* geometry);

	void loadGeometryLibrary(Node* geometryLibraryNode, GeometryLibrary* geometryLibrary);

	void loadCOLLADA(const char* fileName, COLLADA* collada);
}