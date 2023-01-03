#pragma once

#include <GL/glew.h>
#include <iostream>

namespace Utilities {
 
class Shader
{
public:
  // the program ID
  unsigned int ID;

  // constructor reads and builds the shader
  Shader(const char* vertexPath, const char* fragmentPath);

  // use/activate the shader
  void Use();

  // utility uniform functions
  void SetBool(const std::string& name, bool value) const;
  void SetInt(const std::string& name, int value) const;
  void SetFloat(const std::string& name, float value) const;

  void Set4Floats(const std::string& name, float v1, float v2, float v3, float v4) const;
};

}
