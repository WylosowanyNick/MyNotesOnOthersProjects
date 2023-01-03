#version 330 core
layout(location=0) in vec4 a_Position;

out vec4 o_Position;

void main()
{
   gl_Position = a_Position;
}
