#pragma once

#include "loadobj.h"
#include <glm/glm.hpp>

#include "ShaderProgram.h"

class TGA;

class TerrainModel
{
public:
	explicit TerrainModel(const char* filePath);
	~TerrainModel();

	void draw(ShaderProgram& shader) const;

	float getHeight(float x, float z) const;
private:
	static glm::vec3 getVector(int x1, int z1, int x2, int z2, const TGA& tex);
	void generate(const char* filePath);

	float getGridHeight(int x, int z) const;

	int width;
	int height;

	Model* model;


};
