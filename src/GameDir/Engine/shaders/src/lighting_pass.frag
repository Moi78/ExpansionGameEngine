#version 450
layout (location = 0) out vec4 oColor;

layout (location = 0) in vec2 UVcoords;

layout (binding = 10) uniform sampler2D Color;
layout (binding = 11) uniform sampler2D Norm;
layout (binding = 12) uniform sampler2D FragPos;

layout (binding = 13) uniform sampler2D MetRoughAO;
layout (binding = 14) uniform sampler2D SheenProp;

layout (binding = 20) uniform sampler2D dDepth[10];

struct DirLight {
    vec3 dir;
    float brightness;
    vec3 color;
    bool isShadowCaster;
};

struct PointLight {
    vec3 pos;
    float brightness;
    vec3 color;
    float radius;
};

#define PI 3.1415926538

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

layout (binding = 6) uniform CAMERA_INFO {
    vec4 camPos;
    vec4 camDir;
};

layout (binding = 7) uniform LIGHT_MAT {
    mat4 lmats[10];
};

layout (binding = 8) uniform NOISE {
    vec2 noise[16];
};

// MET ROUGH AO
float roughness = texture(MetRoughAO, UVcoords).y;
float metallic = texture(MetRoughAO, UVcoords).x;
float ao = texture(MetRoughAO, UVcoords).z;

// SHEEN
float sheen = texture(SheenProp, UVcoords).x;
float sheenTint = texture(SheenProp, UVcoords).y;

vec3 n = normalize(texture(Norm, UVcoords).xyz);
vec3 fragPos = texture(FragPos, UVcoords).xyz;
vec3 v = camPos.xyz - fragPos;

vec3 c = texture(Color, UVcoords).xyz;

float CalcShadow(int idx, vec3 fPos) {
    vec4 fPos_lightSpace = vec4(fPos, 1.0) * lmats[idx];
    vec3 projCoord = fPos_lightSpace.xyz / fPos_lightSpace.w;

    projCoord = projCoord * 0.5 + 0.5;

    vec2 texelSize = 1.0 / textureSize(dDepth[idx], 0);
    float currentDepth = projCoord.z;
    float bias = 0.0025;

    float center = texture(dDepth[idx], projCoord.xy).r;
    center = center * 0.5 + 0.5;

    const float filterSize = 1.0;
    float shadow = 0.0;
    for(float x = -filterSize; x <= filterSize; x++) {
        for(float y = -filterSize; y <= filterSize; y++) {
            float depthVal = texture(dDepth[idx], projCoord.xy + (noise[int(x) * 3 + int(y)] * texelSize)).r;
            depthVal = depthVal * 0.5 + 0.5;

            if(depthVal > currentDepth - bias) {
                shadow += 1.0;
            }
        }
    }
    shadow /= 9.0;
    return shadow;
}

float GGXDistrib(vec3 halfway, float alpha) {
    float a2 = alpha*alpha;
    float nDh2 = pow(max(dot(n, halfway), 0.0), 2.0);

    return (a2) / (PI * pow(nDh2*(a2 - 1.0) + 1.0, 2.0));
}

float ShadowFac(float NdotV, float k) {
    float denom = NdotV * (1.0 - k) + k;

    return NdotV / denom;
}

vec3 Fresnel(vec3 h, vec3 F0) {
    return F0 + (1.0 - F0) * pow(clamp(1.0 - dot(h, v), 0.0, 1.0), 5.0);
}

vec3 Sheen(vec3 h, vec3 F0) {
    return F0 + (1.0 - F0) * pow(clamp(1.0 - dot(h, n), 0.0, 1.0), 5.0);
}

vec3 ComputeRadianceDir(vec3 color, float brightness) {
    return color * brightness;
}

vec3 ComputeRadiancePoint(vec3 pos, vec3 color, float brightness) {
    float distance = length(pos - fragPos);
    float att = 1.0 / (distance * distance);

    return color * att * brightness;
}

vec3 ComputeLight(vec3 l, vec3 radiance) {
    vec3 diffuse = vec3(0.0, 0.0, 0.0);
    vec3 h = normalize(l + v);

    float spec = GGXDistrib(h, roughness);

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, c, metallic);

    vec3 fresnel = Fresnel(h, F0);

    float r_trans = (roughness + roughness) / 8.0;
    float geom = ShadowFac(max(dot(n, v), 0.0), r_trans) * ShadowFac(max(dot(n, l), 0.0), r_trans);

    vec3 num = spec * geom * fresnel;
    float denom = 4.0 * max(dot(n, v), 0.0) * max(dot(n, l), 0.0) + 0.0001;

    vec3 specular = num / denom;

    vec3 kS = fresnel;

    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;

    float NdL = max(dot(n, l), 0.0);

    vec3 Lo = (kD * c / PI + specular) * radiance * NdL;
    return Lo;
}

void main() {
    vec3 light = vec3(0.0, 0.0, 0.0);

    int sCasterCount = 0;
    for(int i = 0; i < nDLights; i++) {
        vec3 radiance = ComputeRadianceDir(dlights[i].color.rgb, dlights[i].brightness);

        if(dlights[i].isShadowCaster) {
            float factor = CalcShadow(sCasterCount, fragPos);
            light += ComputeLight(normalize(vec3(-dlights[i].dir)), radiance) * factor;

            sCasterCount++;
        }
    }

    for(int i = 0; i < nPLights; i++) {
        float dist = distance(plights[i].pos, fragPos);
        if(dist <= plights[i].radius) {
            vec3 dir = normalize(plights[i].pos - fragPos);

            vec3 radiance = ComputeRadiancePoint(plights[i].pos, plights[i].color, plights[i].brightness);
            light += ComputeLight(dir, radiance);
        }
    }

    light += 0.03 * c * ao;
    light += mix(vec3(1.0), c, sheenTint) * sheen * clamp(Sheen(normalize(-camDir.xyz + v).xyz, vec3(0)), 0.0, 1.0);

    light = light / (light + vec3(1.0));
    oColor = vec4(pow(light, vec3(1.0 / 2.2)), 1.0);
}
