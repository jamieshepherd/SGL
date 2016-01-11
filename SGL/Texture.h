#pragma once

#include <vector>
#include "Common.h"

class Texture
{
public:
	GLuint TextureID;

	Texture(GLchar* path);
	GLuint Get();
};