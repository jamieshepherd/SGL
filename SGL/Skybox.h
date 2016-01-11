#pragma once

#include <vector>
#include "Common.h"
#include "Shader.h"

class Skybox
{
public:
	GLuint TextureID;
	Skybox();
	Shader Shader();
private:
	GLfloat CubeVertices[];
	GLuint Load(std::vector<const GLchar*> faces);
};