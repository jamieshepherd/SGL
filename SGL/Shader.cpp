/*
--------------------------------------------------------------------
Shader.cpp
--------------------------------------------------------------------
A helper class which loads a vertex shader and a fragment shader.
--------------------------------------------------------------------
*/

#include "Shader.h"

Shader::Shader()
{
}

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	this->LoadShader(vertexPath, fragmentPath);
}

void Shader::LoadShader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	std::ifstream vertexFile;
	std::ifstream fragmentFile;
	vertexFile.exceptions(std::ifstream::badbit);
	fragmentFile.exceptions(std::ifstream::badbit);

	// Load and stream the shaders to a string
	try	{
		vertexFile.open(vertexPath);
		fragmentFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vertexFile.rdbuf();
		fShaderStream << fragmentFile.rdbuf();
		vertexFile.close();
		fragmentFile.close();
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	} catch (std::ifstream::failure e) {
		std::cout << "ERROR: Could not read shader files\n" << std::endl;
	}
	const GLchar*  vShaderCode = vertexCode.c_str();
	const GLchar * fShaderCode = fragmentCode.c_str();

	GLuint vertex, fragment;
	GLint success;
	GLchar infoLog[512];

	// Vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);

	// Show errors if not successful
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR: Vertex shader compilation failed\n" << infoLog << std::endl;
	}

	// Fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);

	// Show errors if not successful
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR: Fragment shader compilation failed\n" << infoLog << std::endl;
	}

	// Shader Program
	this->Program = glCreateProgram();
	glAttachShader(this->Program, vertex);
	glAttachShader(this->Program, fragment);
	glLinkProgram(this->Program);

	// Show errors if not successful
	glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
		std::cout << "ERROR: Program linking failed\n" << infoLog << std::endl;
	}

	// Delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

// Use the current shader
void Shader::Use()
{
	glUseProgram(this->Program);
}