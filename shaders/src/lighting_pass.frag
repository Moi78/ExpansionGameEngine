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

struct PointLight {
    vec3 pos;
    float brightness;
    vec3 color;
    float radius;
};

#define MAX_DIR_LIGHTS 10
layout (binding = 3) uniform DIR_LIGHTS {
    DirLight dlights[MAX_DIR_LIGHTS];
};

#define MAX_POINT_LIGHTS 300
layout (binding = 4) uniform POINT_LIGHTS {
    PointLight plights[MAX_POINT_LIGHTS];
};

layout (binding = 5) uniform CASTER_COUNT {
    int nDLights;
    int nPLights;
};

vec3 n = normalize(texture(Norm, UVcoords).xyz);
vec3 c = texture(Color, UVcoords).xyz;
vec3 fragPos = texture(FragPos, UVcoords).xyz;

void main() {
    vec3 light = vec3(0.0, 0.0, 0.0);
    for(int i = 0; i < nDLights; i++) {
        vec3 normDir = normalize(-dlights[i].dir);

        float lightness = max(dot(n, normDir), 0.0);

        light += lightness * c * dlights[i].color.rgb * dlights[i].brightness;
    }

    for(int i = 0; i < nPLights; i++) {
        float dist = distance(plights[i].pos, fragPos);
        if(dist <= plights[i].radius) {
            vec3 lightDir = plights[i].pos - fragPos;

            float lightness = max(dot(n, lightDir), 0.0);
            float attenuation = 1.0 / (dist * dist);

            light += lightness * c * plights[i].color.rgb * plights[i].brightness * attenuation;
        }
    }

    light += 0.1 * c;

    oColor = vec4(light, 1.0);
}
