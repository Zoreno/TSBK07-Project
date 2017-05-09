/**
 * @file	TerrainModel.h
 * @Author	Joakim Bertils
 * @date	2017-05-09
 * @brief	Terrain Model
 */

#pragma once

#include "loadobj.h"
#include <glm/glm.hpp>

#include "ShaderProgram.h"

class TGA;

/**
 * @brief Terrain model class
 */
class TerrainModel
{
public:

	/**
	 * @brief Constructor
	 * @param filePath Path to terrain file.
	 */
	explicit TerrainModel(const char* filePath);

	/**
	 * @brief Destructor.
	 */
	~TerrainModel();

	/**
	 * @brief Draws the model.
	 * @param shader Shader to use.
	 */
	void draw(ShaderProgram& shader) const;

	/**
	 * @brief Gets the height at given position
	 * @param x X-coordinate
	 * @param z Z-coordinate
	 * @return Height at (x,z)
	 */
	float getHeight(float x, float z) const;
private:

	/**
	 * @brief Returns the 3D vector between (x1, y1, z1) and (x2, y2, z2) where y1 and y2 are sampled from file.
	 * @param x1 First x-coordinate
	 * @param z1 First z-coordinate
	 * @param x2 Second x-coordinate
	 * @param z2 Second y-coordinate
	 * @param tex Texture to sample
	 * @return Vector.
	 */
	static glm::vec3 getVector(int x1, int z1, int x2, int z2, const TGA& tex);
	
	/**
	 * @brief Generates the terrain from file
	 * @param filePath Path to file.
	 */
	void generate(const char* filePath);

	/**
	 * @brief Gets the height in the integer grid.
	 * @param x X-coordinate
	 * @param z Z-coordinate
	 * @return Height at grid.
	 */
	float getGridHeight(int x, int z) const;

	/**
	 * @brief Width of terrain
	 */
	int width;
	
	/**
	 * @brief Height of terrain
	 */
	int height;

	/**
	 * @brief Terrain Model
	 */
	Model* model;
};
