#version 450 core

layout(location = 0) out vec4 f_color;

in vec3 v_position;
in vec3 v_normal;

uniform float u_amplitude;

struct DirectionalLight {
	vec4 diffuse;
	vec3 direction;
};

// Colors for different height intervals
vec4 grass =        vec4(0.2, 0.4, 0.0, 1.0);
vec4 hilly_grass =  vec4(0.3, 0.5, 0.0, 1.0);
vec4 hill =         vec4(0.5, 0.5, 0.0, 1.0);
vec4 mountain =     vec4(0.6, 0.3, 0.0, 1.0);
vec4 mountain_top = vec4(0.6, 0.2, 0.1, 1.0);

DirectionalLight light = DirectionalLight(vec4(1.0), vec3(0.3, 1.0, 0.0));

// Hiking terrain color palette
vec4 topoPalette(float y) {

    if (y < 0.3) return grass;

    if (y < 0.45) return hilly_grass;

    if (y < 0.65) return hill;

    if (y < 0.85) return mountain;

    return mountain_top;
}

void main()
{
    float y = (v_position.y) / (u_amplitude); // Get relative height
    vec4 groundColor = topoPalette(y); // Get color for this height
    
    vec3 L = normalize(light.direction); // Light vector
	vec3 N = normalize(v_normal); // Normal vector for this surface
    
	float cosTheta = dot(L, N); // Angle between light direction and normal of this surface
    
    vec3 shadedColor = light.diffuse.xyz * groundColor.xyz * max(cosTheta, 0.0);
    
    vec4 finalColor = vec4(shadedColor, 1.0);

	f_color = finalColor;
}
