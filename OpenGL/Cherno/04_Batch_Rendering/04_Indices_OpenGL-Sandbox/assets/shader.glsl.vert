#version 450 core

layout(location=0) in vec3 a_Position; // (x,y,z)
layout(location=1) in vec4 a_Color; // rgba
layout(location=2) in vec2 a_TexCoord; // (x,y)
layout(location=3) in float a_TexIndex; // float

uniform mat4 u_ViewProj, u_Transform;

out vec4 v_Color; // rgba
out vec2 v_TexCoord; // (x,y)
out float v_TexIndex; // float

void main()
{
   gl_Position = u_ViewProj * u_Transform * vec4(a_Position, 1.0);
   v_Color = a_Color;
   v_TexCoord = a_TexCoord;
   v_TexIndex = a_TexIndex;
}
