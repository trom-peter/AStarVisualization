#version 450 core

// Vertex attributes in model space
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;

out vec3 v_normal;
out vec3 v_position;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0);  // Vertex position in clip space
    v_normal = (inverse(transpose(u_model)) * vec4(a_normal, 1.0)).xyz;     // Vertex normal in world space
    v_position = (u_model * vec4(a_position, 1.0)).xyz;                     // Vertex position in world space
}
