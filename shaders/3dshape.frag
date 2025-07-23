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
	//DirectionalLight light = DirectionalLight(vec4(1.0), vec4(1.0), vec4(1.0), vec3(1.0, 1.0, 1.0));
	//
	//mat3 viewRotation = mat3(u_view);
    //vec3 viewLightDir = normalize(viewRotation * light.direction);
	//
	//vec3 L = normalize(viewLightDir);
	//vec3 N = normalize(v_normal);
	//vec3 V = normalize(-u_view[3].xyz - v_position);
	//
	//vec3 R = normalize(reflect(-L, N));
	//float cosTheta = dot(L, N);
	//float cosPhi = dot(V, R);
	//
	//vec3 ambient = ((u_globalAmbient * u_material.ambient) + (light.ambient * u_material.ambient)).xyz;
	//vec3 diffuse = light.diffuse.xyz * u_material.diffuse.xyz * max(cosTheta, 0.0);
	//vec3 specular = light.specular.xyz * u_material.specular.xyz * pow(max(cosPhi, 0.0), u_material.shininess);

	//f_color = vec4((ambient + diffuse + specular), 1.0);
	//f_color = u_material.diffuse;
	f_color = vec4(u_color, 1.0);
}