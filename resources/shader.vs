#version 400 core

layout (location = 0) in vec3 aPos;

uniform mat4 uni_model;
uniform mat4 uni_view;
uniform mat4 uni_projection;

void main()
{
	gl_Position = uni_projection * uni_view * uni_model * vec4(aPos, 1.0);
}