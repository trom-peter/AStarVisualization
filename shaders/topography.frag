#version 450 core

layout(location = 0) out vec4 f_color;

in vec3 v_position;
in vec3 v_normal;

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

uniform float u_amplitude;
uniform mat4 u_view;
uniform vec4 u_globalAmbient;

Material grass =        Material(vec4(0.0), vec4(vec3(0.2, 0.4, 0.0), 0.0), vec4(0.0), 0.0);  // Gras
Material hilly_grass =  Material(vec4(0.0), vec4(vec3(0.3, 0.5, 0.0), 0.0), vec4(0.0), 0.0);  // Hügelgras
Material hill =         Material(vec4(0.0), vec4(vec3(0.5, 0.5, 0.0), 0.0), vec4(0.0), 0.0);  // Hügel
Material mountain =     Material(vec4(0.0), vec4(vec3(0.6, 0.3, 0.0), 0.0), vec4(0.0), 0.0);  // Berg
Material mountain_top = Material(vec4(0.0), vec4(vec3(0.6, 0.2, 0.1), 0.0), vec4(0.0), 5.0);  // Schnee


vec3 palette(float t) {
    vec3 A = vec3(-3.1, 0.5, 0.2); 
    vec3 B = vec3(0.5, 0.5, 0.27); 
    vec3 C = vec3(0.52, 0.5, 1.1); 
    vec3 D = vec3(0.5, -0.6, 0.7);
    return A + B * cos(2.0 * 3.1415 * (C * t + D));
}

Material topoPalette_smooth(float y) {
    float t;

    if (y < 0.3) return grass;

    if (y < 0.45) {
        t = smoothstep(0.3, 0.45, y);
        return Material(mix(grass.ambient, hilly_grass.ambient, t), mix(grass.diffuse, hilly_grass.diffuse, t), 
                        mix(grass.specular, hilly_grass.specular, t), mix(grass.shininess, hilly_grass.shininess, t));
    }
    if (y < 0.65) {
        t = smoothstep(0.45, 0.65, y);
        return Material(mix(hilly_grass.ambient, hill.ambient, t), mix(hilly_grass.diffuse, hill.diffuse, t), 
                        mix(hilly_grass.specular, hill.specular, t), mix(hilly_grass.shininess, hill.shininess, t));
    }
    if (y < 0.85) {
        t = smoothstep(0.65, 0.85, y);
        return Material(mix(hill.ambient, mountain.ambient, t), mix(hill.diffuse, mountain.diffuse, t), 
                        mix(hill.specular, mountain.specular, t), mix(hill.shininess, mountain.shininess, t));
    }

    t = smoothstep(0.85, 0.95, y);
    return Material(mix(mountain.ambient, mountain_top.ambient, t), mix(mountain.diffuse, mountain_top.diffuse, t), 
                    mix(mountain.specular, mountain_top.specular, t), mix(mountain.shininess, mountain_top.shininess, t));
}

Material topoPalette(float y) {

    if (y < 0.3) return grass;

    if (y < 0.45) return hilly_grass;

    if (y < 0.65) return hill;

    if (y < 0.85) return mountain;

    return mountain_top;
}


void main()
{
    float y = (v_position.y) / (u_amplitude);
    Material groundMat = topoPalette_smooth(y);

	DirectionalLight light = DirectionalLight(vec4(1.0), vec4(1.0), vec4(1.0), vec3(0.0, 1.0, 0.0));

    mat3 viewRotation = mat3(u_view);
    vec3 viewLightDir = normalize(viewRotation * light.direction);

    vec3 L = normalize(viewLightDir);
	vec3 N = normalize(v_normal);
	vec3 V = normalize(-u_view[3].xyz - v_position); // -position reicht, weil kamera immer im ursprung ist

    vec3 R = normalize(reflect(-L, N));
	float cosTheta = dot(L, N);
	float cosPhi = dot(V, R);

    vec3 ambient = ((u_globalAmbient * groundMat.ambient) + (light.ambient * groundMat.ambient)).xyz;
    vec3 diffuse = light.diffuse.xyz * groundMat.diffuse.xyz * max(cosTheta, 0.2);
	vec3 specular = light.specular.xyz * groundMat.specular.xyz * pow(max(cosPhi, 0.0), groundMat.shininess);

    vec3 finalColor = vec3(ambient + diffuse + specular);
    //vec3 finalColor = vec3(y);

	f_color = vec4(finalColor, 1.0);
}