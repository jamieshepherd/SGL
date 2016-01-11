#pragma once

#include "Common.h"
#include "Shader.h"

#include "assimp/types.h"

using namespace std;

struct Vertex {
	// Position
	glm::vec3 Position;
	// Normal
	glm::vec3 Normal;
	// TexCoords
	glm::vec2 TexCoords;
};

struct Texture {
	GLuint id;
	string type;
	aiString path;
};

class Mesh
{
public:
	Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures);
	void Draw(Shader shader);

	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;
private:
	void setupMesh();

	GLuint VAO, VBO, EBO;
};