#version 450 core

layout(location = 0) out vec4 f_color;

in vec3 v_normal;
in vec3 v_position;

uniform vec3 u_color;

void main()
{
	f_color = vec4(u_color, 1.0);
}