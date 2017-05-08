#ifndef loadobj_h
#define loadobj_h
#include "EngineDLL.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#if defined(_WIN32)
#include <GL/glew.h>
#endif
#include <GL/gl.h>
#endif

	// How many error messages do you want before it stops?
#define NUM_DRAWMODEL_ERROR 8

	typedef struct
	{
		GLfloat* vertexArray;
		GLfloat* normalArray;
		GLfloat* texCoordArray;
		GLfloat* colorArray; // Rarely used
		GLuint* indexArray;
		int numVertices;
		int numIndices;

		// Space for saving VBO and VAO IDs
		GLuint vao; // VAO
		GLuint vb, ib, nb, tb; // VBOs
	} Model;

	// Basic model loading

	ENGINE_API Model* LoadModel(char* name); // Old version, single part OBJ only!
	ENGINE_API Model** LoadModel2(char* name); // Multi-part OBJ!

									// Extended, load model and upload to arrays!
									// DrawModel is for drawing such preloaded models.

	ENGINE_API void DrawModel(Model *m, GLuint program, char* vertexVariableName, char* normalVariableName, char* texCoordVariableName);
	ENGINE_API void DrawWireframeModel(Model *m, GLuint program, char* vertexVariableName, char* normalVariableName, char* texCoordVariableName);

	ENGINE_API Model* LoadModelPlus(char* name);
	ENGINE_API Model** LoadModel2Plus(char* name);

	// Utility functions that you may need if you want to modify the model.

	ENGINE_API void EnableModelForShader(Model *m, GLuint program, // NOT TESTED
		char* vertexVariableName,
		char* normalVariableName,
		char* texCoordVariableName);
	ENGINE_API Model* LoadDataToModel(
		GLfloat *vertices,
		GLfloat *normals,
		GLfloat *texCoords,
		GLfloat *colors,
		GLuint *indices,
		int numVert,
		int numInd);
	ENGINE_API void ReloadModelData(Model *m);

	ENGINE_API void CenterModel(Model *m);
	ENGINE_API void ScaleModel(Model *m, float sx, float sy, float sz);
	ENGINE_API void DisposeModel(Model *m);

#ifdef __cplusplus
}
#endif

#endif
