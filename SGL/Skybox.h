#pragma once

#include <vector>
#include "Common.h"
#include "Shader.h"

class Skybox
{
public:
	GLuint TextureID;
	GLuint skyboxVAO, skyboxVBO;
	GLuint cubemapTexture;
	Shader *shader;

	Skybox(char* a);
	void Draw();
private:
	GLfloat CubeVertices[];
	GLuint Load(std::vector<const GLchar*> faces);
};