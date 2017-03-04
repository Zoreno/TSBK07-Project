#include "Collada.h"

#include <rapidxml/rapidxml.hpp>
#include "Utils.h"

#include <iostream>
#include <sstream>

using namespace rapidxml;

namespace collada
{
	void loadFloatArray(Node* floatArrayNode, FloatArray* floatArray)
	{
		floatArray->ID = floatArrayNode->first_attribute("id")->value();
		floatArray->count = atoi(floatArrayNode->first_attribute("count")->value());
		char* data = floatArrayNode->value();
		std::stringstream ss{ data };

		for(unsigned int i{0}; i < floatArray->count; ++i)
		{
			float f;
			ss >> f;
			floatArray->floats.push_back(f);
		}
	}

	void loadNameArray(Node* nameArrayNode, NameArray* nameArray)
	{
		nameArray->ID = nameArrayNode->first_attribute("id")->value();
		nameArray->count = atoi(nameArrayNode->first_attribute("count")->value());
		char* data = nameArrayNode->value();
		std::stringstream ss{ data };

		for (unsigned int i{ 0 }; i < nameArray->count; ++i)
		{
			std::string f;
			ss >> f;
			nameArray->names.push_back(f);
		}
	}

	void loadParam(Node* paramNode, Param* param)
	{
		param->name = paramNode->first_attribute("name")->value();
		param->type = paramNode->first_attribute("type")->value();
	}

	void loadAccessor(Node* accessorNode, Accessor* accessor)
	{
		accessor->source = accessorNode->first_attribute("source")->value();
		accessor->count = atoi(accessorNode->first_attribute("count")->value());
		accessor->stride = atoi(accessorNode->first_attribute("stride")->value());

		Node* paramNode = accessorNode->first_node("param");

		while(paramNode != nullptr)
		{
			Param param;
			loadParam(paramNode, &param);
			accessor->params.push_back(param);
			paramNode = paramNode->next_sibling("param");
		}

	}

	void loadTechnique_Common(Node* techniqueCommonNode, Technique_Common* techniqueCommon)
	{
		loadAccessor(techniqueCommonNode->first_node("accessor"), &techniqueCommon->accessor);
	}

	void loadSource(Node* sourceNode, Source* source)
	{
		source->ID = sourceNode->first_attribute("id")->value();

		Attrib* nameAttrib = sourceNode->first_attribute("name");
		if(nameAttrib != nullptr)
		{
			source->name = nameAttrib->value();
		}

		Node* floatArrayNode = sourceNode->first_node("float_array");
		if(floatArrayNode != nullptr)
		{
			loadFloatArray(floatArrayNode, &source->floatArray);
		}

		Node* nameArrayNode = sourceNode->first_node("NAME_array");
		if (nameArrayNode != nullptr)
		{
			loadNameArray(nameArrayNode, &source->nameArray);
		}

		loadTechnique_Common(sourceNode->first_node("technique_common"), &source->techniqueCommon);
	}

	void loadInput(Node* inputNode, Input* input)
	{
		Attrib* semanticAttrib = inputNode->first_attribute("semantic");
		if(semanticAttrib != nullptr)
		{
			input->semantic = semanticAttrib->value();
		}

		Attrib* sourceAttrib = inputNode->first_attribute("source");
		if (sourceAttrib != nullptr)
		{
			input->source = sourceAttrib->value();
		}

		Attrib* offsetAttrib = inputNode->first_attribute("offset");
		if (offsetAttrib != nullptr)
		{
			input->offset = offsetAttrib->value();
		}

		Attrib* setAttrib = inputNode->first_attribute("set");
		if (setAttrib != nullptr)
		{
			input->set = setAttrib->value();
		}
	}

	void loadVertices(Node* verticesNode, Vertices* vertices)
	{
		vertices->ID = verticesNode->first_attribute("id")->value();

		Node* inputNode = verticesNode->first_node("input");

		while(inputNode != nullptr)
		{
			Input input;
			loadInput(inputNode, &input);
			vertices->inputs.push_back(input);
			inputNode = inputNode->next_sibling("inputNode");
		}
	}

	void loadP(Node* pNode, P* p)
	{
		char* data = pNode->value();
		std::stringstream ss{ data };

		unsigned int count{0};

		while(ss)
		{
			unsigned short f;
			ss >> f;
			p->indices.push_back(f);
			++count;
		}

		p->count = count;
	}

	void loadTriangles(Node* trianglesNode, Triangles* triangles)
	{
		triangles->count = atoi(trianglesNode->first_attribute("count")->value());
		triangles->material = trianglesNode->first_attribute("material")->value();

		Node* inputNode = trianglesNode->first_node("input");

		while (inputNode != nullptr)
		{
			Input input;
			loadInput(inputNode, &input);
			triangles->inputs.push_back(input);
			inputNode = inputNode->next_sibling("inputNode");
		}

		loadP(trianglesNode->first_node("p"), &triangles->p);
	}

	void loadVCount(Node* vCountNode, VCount* vCount)
	{
		char* data = vCountNode->value();
		std::stringstream ss{ data };

		unsigned int count{ 0 };

		while (ss)
		{
			unsigned short f;
			ss >> f;
			vCount->indices.push_back(f);
			++count;
		}

		vCount->count = count;
	}

	void loadPolyList(Node* polyListNode, PolyList* polyList)
	{
		polyList->count = atoi(polyListNode->first_attribute("count")->value());

		Attrib* nameAttrib = polyListNode->first_attribute("name");
		if (nameAttrib != nullptr)
		{
			polyList->name = nameAttrib->value();
		}
		polyList->material = polyListNode->first_attribute("material")->value();

		Node* inputNode = polyListNode->first_node("input");

		while (inputNode != nullptr)
		{
			Input input;
			loadInput(inputNode, &input);
			polyList->inputs.push_back(input);
			inputNode = inputNode->next_sibling("inputNode");
		}

		loadVCount(polyListNode->first_node("vcount"), &polyList->vcount);

		loadP(polyListNode->first_node("p"), &polyList->p);
	}

	void loadMesh(Node* meshNode, Mesh* mesh)
	{
		Node* sourceNode = meshNode->first_node("source");
		while(sourceNode != nullptr)
		{
			Source source;
			loadSource(sourceNode, &source);
			mesh->sources.push_back(source);
			sourceNode = sourceNode->next_sibling("source");
		}

		loadVertices(meshNode->first_node("vertices"), &mesh->vertices);

		Node* trianglesNode = meshNode->first_node("triangles");
		while (trianglesNode != nullptr)
		{
			Triangles triangles;
			loadTriangles(trianglesNode, &triangles);
			mesh->triangles.push_back(triangles);
			trianglesNode = trianglesNode->next_sibling("triangles");
		}

		Node* polyListNode = meshNode->first_node("polylist");
		while(polyListNode != nullptr)
		{
			PolyList polyList;
			loadPolyList(polyListNode, &polyList);
			mesh->polyLists.push_back(polyList);
			polyListNode = polyListNode->next_sibling("polylist");
		}
	}

	void loadGeometry(Node* geometryNode, Geometry* geometry)
	{
		geometry->ID = geometryNode->first_attribute("id")->value();
		geometry->name = geometryNode->first_attribute("name")->value();

		Node* meshNode = geometryNode->first_node("mesh");

		loadMesh(meshNode, &geometry->mesh);
	}

	void loadGeometryLibrary(Node* geometryLibraryNode, GeometryLibrary* geometryLibrary)
	{
		// Get the first geometry node
		Node* geometryNode = geometryLibraryNode->first_node("geometry");

		// Loop until we have traversed all geometry nodes.
		while (geometryNode != nullptr)
		{
			Geometry geometry;
			loadGeometry(geometryNode, &geometry);
			geometryLibrary->geometries.push_back(geometry);
			geometryNode = geometryNode->next_sibling("geometry");

		}
	}

	void loadCOLLADA(const char* fileName, COLLADA* collada)
	{
		// Read in source file.
		std::string source = getStringFromFile(fileName);

		// Create a document
		Document doc;

		// Parse the source into the document
		doc.parse<0>(const_cast<char*>(source.c_str()));

		// Find first COLLADA node.
		Node* collada_node = doc.first_node("COLLADA");

		// Find the first library geometries node.
		Node* library_geometries_node = collada_node->first_node("library_geometries");

		// Load the geometry library information.
		loadGeometryLibrary(library_geometries_node, &collada->geometryLibrary);
	}
}
