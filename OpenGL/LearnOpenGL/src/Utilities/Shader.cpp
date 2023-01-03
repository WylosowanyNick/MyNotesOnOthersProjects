#include "Shader.h"

#include <GL/glew.h>

#include <iostream>
#include <fstream>
#include <sstream>

namespace Utilities {

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
  std::string vertexCode, fragmentCode;
  std::ifstream vShaderFile, fShaderFile;

// Getting shaders' source code
  // Opening files
  vShaderFile.open(vertexPath);
  fShaderFile.open(fragmentPath);

  // Reading file's buffer contents into streams
	std::stringstream vShaderStream, fShaderStream;
  vShaderStream << vShaderFile.rdbuf();
  fShaderStream << fShaderFile.rdbuf();

  // Closing file handlers
  vShaderFile.close();
  fShaderFile.close();

  // Convert streams into strings
  vertexCode = vShaderStream.str();
  fragmentCode = fShaderStream.str();

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

// Compiling shaders
  unsigned int vertex, fragment;
  int success;
  char infoLog[512];

  // Vertex shader
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vShaderCode, NULL);
  glCompileShader(vertex);

  glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
  if(!success) // print compile errors if any
  {
    glGetShaderInfoLog(vertex, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
      infoLog << std::endl;
  }
 
  // Fragment shader
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fShaderCode, NULL);
  glCompileShader(fragment);

  glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
  if(!success) // print compile errors if any
  {
    glGetShaderInfoLog(fragment, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" <<
      infoLog << std::endl;
  }

// Shader program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);

	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if(!success) // print linking errors if any
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" <<
			infoLog << std::endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::Use()
{
	glUseProgram(ID);
}

void Shader::SetBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::Set4Floats(const std::string& name, float v0, float v1, float v2, float v3) const
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), v0, v1, v2, v3);
}

}
