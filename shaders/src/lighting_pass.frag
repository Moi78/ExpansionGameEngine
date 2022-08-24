#version 450
layout (location = 0) out vec4 oColor;

layout (location = 0) in vec2 UVcoords;

layout (binding = 0) uniform sampler2D Color;
layout (binding = 1) uniform sampler2D Norm;
layout (binding = 2) uniform sampler2D FragPos;

struct DirLight {
    vec3 dir;
    float brightness;
    vec4 color;
};

#define MAX_DIR_LIGHTS 10
layout (binding = 3) uniform DIR_LIGHTS {
    DirLight dlights[MAX_DIR_LIGHTS];
};

void main() {
    vec3 light = vec3(0.0, 0.0, 0.0);
    for(int i = 0; i < MAX_DIR_LIGHTS; i++) {
        vec3 n = normalize(texture(Norm, UVcoords).xyz);
        vec3 normDir = normalize(-dlights[i].dir);

        float lightness = max(dot(n, normDir), 0.0);

        vec3 c = texture(Color, UVcoords).xyz;
        light += lightness * c * dlights[i].color.rgb * dlights[i].brightness;
    }

    light += 0.1 * texture(Color, UVcoords).xyz;

    oColor = vec4(light, 1.0);
}
