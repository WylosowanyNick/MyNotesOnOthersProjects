#include <GL/glew.h>
#include <iostream>
 
namespace Utilities {

void GLClearError()
{
  while(glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
  while(GLenum error = glGetError())
  {
    std::cout << "[OpenGl Error] (" << error << "): " << function <<
    " " << file << ": " << line << '\n';

    return false;
  }
  return true;
}

}
