cl^
 /c /I .\inc /I .\src\vendor^
 /EHsc .\src\main.cpp /Fo"./bin/" 

cl^
 /Fe"main"^
 /EHsc .\bin\*.obj^
 /DGLEW_STATIC^
 /link /LIBPATH:.\dep^
 glfw3.lib glew32s.lib opengl32.lib user32.lib gdi32.lib shell32.lib msvcrt.lib

main.exe