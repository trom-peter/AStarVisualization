#version 450 core

layout(location = 0) out vec4 f_color;

in vec3 v_normal;
in vec3 v_position;

struct Material {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

struct DirectionalLight {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec3 direction;
};

uniform mat4 u_view;
uniform vec4 u_globalAmbient;
uniform vec3 u_color;

void main()
{
	f_color = vec4(u_color, 1.0);
}