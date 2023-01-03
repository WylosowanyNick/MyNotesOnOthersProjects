#pragma once

// Makro i funkcje pozwalające na debugowanie funkcji OpenGLa
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
  x;\
  ASSERT(GLLogCall(#x, __FILE__, __LINE__))

namespace Utilities {

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

}
