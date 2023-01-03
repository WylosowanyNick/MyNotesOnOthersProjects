#version 450 core

layout(location=0) in vec4 a_Position; // (x,y,z)
layout(location=1) in vec4 a_Color; // rgba

uniform mat4 u_ViewProj, u_Transform;

out vec4 v_Color; // rgba

void main()
{
   gl_Position = u_ViewProj * u_Transform * a_Position;
   v_Color = a_Color;
}
