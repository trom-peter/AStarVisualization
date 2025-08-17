#version 450 core

layout(location = 0) out vec4 f_color;

in vec3 v_position;
in vec3 v_normal;

uniform float u_amplitude;
uniform mat4 u_view;

struct DirectionalLight {
	vec4 diffuse;
	vec3 direction;
};

vec4 grass =        vec4(0.2, 0.4, 0.0, 1.0);  // Gras
vec4 hilly_grass =  vec4(0.3, 0.5, 0.0, 1.0);  // Hügelgras
vec4 hill =         vec4(0.5, 0.5, 0.0, 1.0);  // Hügel
vec4 mountain =     vec4(0.6, 0.3, 0.0, 1.0);  // Berg
vec4 mountain_top = vec4(0.6, 0.2, 0.1, 1.0);  // Bergspitze

DirectionalLight light = DirectionalLight(vec4(1.0), vec3(0.3, 1.0, 0.0));

vec4 topoPalette(float y) {

    if (y < 0.3) return grass;

    if (y < 0.45) return hilly_grass;

    if (y < 0.65) return hill;

    if (y < 0.85) return mountain;

    return mountain_top;
}

void main()
{
    float y = (v_position.y) / (u_amplitude);
    vec4 groundColor = topoPalette(y);
    
    mat3 viewRotation = mat3(u_view);
    vec3 viewLightDir = normalize(viewRotation * light.direction);
    
    vec3 L = normalize(viewLightDir); //light vector
	vec3 N = normalize(v_normal); //normal vector for this pixel
    
	float cosTheta = dot(L, N);
    
    vec3 shadedColor = light.diffuse.xyz * groundColor.xyz * max(cosTheta, 0.0);
    
    vec4 finalColor = vec4(shadedColor, 1.0);
    //vec4 finalColor = vec4(vec3(y), 1.0);
    //vec4 finalColor = vec4(groundColor);

	f_color = finalColor;
}