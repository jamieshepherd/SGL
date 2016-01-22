#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

class Shader
{
public:
	GLuint Program;
	Shader();
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	void LoadShader(const GLchar* vertexPath, const GLchar* fragmentPath);
	void Use();
private:
	std::string vertexCode;
	std::string fragmentCode;
};