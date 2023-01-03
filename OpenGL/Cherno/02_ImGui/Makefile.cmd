cl^
 /Fe"main"^
 /I .\inc /I .\src\vendor^
 /EHsc .\src\*.cpp .\src\vendor\glad.c .\src\vendor\stb_image\*.cpp .\src\vendor\imgui\*.cpp /Fo"./bin/"^
 /DGLEW_STATIC^
 /link /LIBPATH:.\dep^
 glfw3.lib glew32s.lib opengl32.lib user32.lib gdi32.lib shell32.lib msvcrt.lib

main.exe