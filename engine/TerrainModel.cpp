#include "TerrainModel.h"
#include "TGA.h"
#include <list>
#include <iostream>
#include "Utils.h"

TerrainModel::TerrainModel(const char* filePath)
{
	generate(filePath);
}

TerrainModel::~TerrainModel()
{
	DisposeModel(model);
}

void TerrainModel::draw(ShaderProgram& shader) const
{
	DrawModel(model, shader.getShaderProgramHandle(), "vertex_position", "vertex_normal", "vertex_texture_coordinates");
}

float TerrainModel::getHeight(float x, float z) const
{
	int x_int = static_cast<int>(x);
	float x_rem = x - x_int;

	int z_int = static_cast<int>(z);
	float z_rem = z - z_int;

	if (x_rem + z_rem < 1.f)
	{
		float c = getGridHeight(x_int, z_int);

		float a = getGridHeight(x_int + 1, z_int) - c;

		float b = getGridHeight(x_int, z_int + 1) - c;

		return a*x_rem + b*z_rem + c;
	}

	x_int += 1;
	z_int += 1;

	x_rem = -(1 - x_rem);
	z_rem = -(1 - z_rem);

	float c = getGridHeight(x_int, z_int);

	float a = c - getGridHeight(x_int - 1, z_int);

	float b = c - getGridHeight(x_int, z_int - 1);

	return a*x_rem + b*z_rem + c;
}

glm::vec3 TerrainModel::getVector(int x1, int z1, int x2, int z2, const TGA& tex)
{
	glm::vec3 vec;

	vec.x = x2 - x1;
	vec.z = z2 - z1;

	vec.y = tex.getPixels()[(x2 + z2 * tex.getWidth()) * ((tex.hasAlpha() ? 32 : 24) / 8)] / 10.0 -
		tex.getPixels()[(x1 + z1 * tex.getWidth()) * ((tex.hasAlpha() ? 32 : 24) / 8)] / 10.0;

	return vec;
}

void TerrainModel::generate(const char* filePath)
{
	TGA textureFile(filePath);

	width = textureFile.getWidth();
	height = textureFile.getHeight();

	int vertexCount = width * height;
	int triangleCount = (width - 1) * (height - 1) * 2;

	int x, z;

	GLfloat *vertexArray = static_cast<GLfloat*>(malloc(sizeof(GLfloat) * 3 * vertexCount));
	GLfloat *normalArray = static_cast<GLfloat*>(malloc(sizeof(GLfloat) * 3 * vertexCount));
	GLfloat *texCoordArray = static_cast<GLfloat*>(malloc(sizeof(GLfloat) * 2 * vertexCount));
	GLuint *indexArray = static_cast<GLuint*>(malloc(sizeof(GLuint) * triangleCount * 3));

	int bpp = textureFile.hasAlpha() ? 32 : 24;

	for (x = 0; x < width; ++x)
	{
		for (z = 0; z < height; ++z)
		{
			vertexArray[(x + z * width) * 3 + 0] = x / 1.0;
			vertexArray[(x + z * width) * 3 + 1] = textureFile.getPixels()[(x + z * width) * (bpp / 8)] / 10.0;
			vertexArray[(x + z * width) * 3 + 2] = z / 1.0;

			std::list<int> indexList = { 1,2,3,4,5,6 };

			if (x == 0)
			{
				indexList.remove(1);
				indexList.remove(5);
				indexList.remove(6);
			}
			else if (x == width - 1)
			{
				indexList.remove(2);
				indexList.remove(3);
				indexList.remove(4);
			}

			if (z == 0)
			{
				indexList.remove(1);
				indexList.remove(2);
				indexList.remove(3);
			}
			else if (z == height - 1)
			{
				indexList.remove(4);
				indexList.remove(5);
				indexList.remove(6);
			}

			std::vector<glm::vec3> normals;

			for (auto& it : indexList)
			{
				switch (it)
				{
				case 1:
					normals.push_back(2.f*glm::normalize(glm::cross(getVector(x, z, x, z - 1, textureFile), getVector(x, z, x - 1, z, textureFile))));
					break;
				case 2:
					normals.push_back(glm::normalize(glm::cross(getVector(x, z, x + 1, z - 1, textureFile), getVector(x, z, x, z - 1, textureFile))));
					break;
				case 3:
					normals.push_back(glm::normalize(glm::cross(getVector(x, z, x + 1, z, textureFile), getVector(x, z, x + 1, z - 1, textureFile))));
					break;
				case 4:
					normals.push_back(2.f*glm::normalize(glm::cross(getVector(x, z, x, z + 1, textureFile), getVector(x, z, x + 1, z, textureFile))));
					break;
				case 5:
					normals.push_back(glm::normalize(glm::cross(getVector(x, z, x - 1, z, textureFile), getVector(x, z, x - 1, z + 1, textureFile))));
					break;
				case 6:
					normals.push_back(glm::normalize(glm::cross(getVector(x, z, x - 1, z + 1, textureFile), getVector(x, z, x, z + 1, textureFile))));
					break;
				default: // Should not happen
					break;
				}
			}

			glm::vec3 normal{};

			for (auto& it : normals)
			{
				normal += it;
			}

			normal = glm::normalize(normal);

			normalArray[(x + z * width) * 3 + 0] = normal.x;
			normalArray[(x + z * width) * 3 + 1] = normal.y;
			normalArray[(x + z * width) * 3 + 2] = normal.z;
			// Texture coordinates. You may want to scale them.
			texCoordArray[(x + z * width) * 2 + 0] = x;
			texCoordArray[(x + z * width) * 2 + 1] = z;
		}
	}

	for (x = 0; x < width - 1; ++x)
		for (z = 0; z < height - 1; ++z)
		{
			// Triangle 1
			indexArray[(x + z * (width - 1)) * 6 + 0] = x + z * width;
			indexArray[(x + z * (width - 1)) * 6 + 1] = x + (z + 1) * width;
			indexArray[(x + z * (width - 1)) * 6 + 2] = x + 1 + z * width;
			// Triangle 2
			indexArray[(x + z * (width - 1)) * 6 + 3] = x + 1 + z * width;
			indexArray[(x + z * (width - 1)) * 6 + 4] = x + (z + 1) * width;
			indexArray[(x + z * (width - 1)) * 6 + 5] = x + 1 + (z + 1) * width;
		}

	// End of terrain generation

	// Create Model and upload to GPU:

	model = LoadDataToModel(
		vertexArray,
		normalArray,
		texCoordArray,
		NULL,
		indexArray,
		vertexCount,
		triangleCount * 3);
}

float TerrainModel::getGridHeight(int x, int z) const
{
	return model->vertexArray[(x + width*z) * 3 + 1];
}
