#version 410 core
layout (location = 1) out vec3 ssr_color;

in vec2 UVcoords;

uniform sampler2D gPos;
uniform sampler2D gNorm;
uniform sampler2D ShadedImg;
uniform sampler2D gMetRoughAO;
uniform sampler2D Depth;

uniform mat4 projection;
uniform mat4 view;
uniform vec3 camPos;

float maxDist = 50;
float resolution = 0.3;
float steps = 1;
float thickness = 0.5;

void main() {
    if(texture(gMetRoughAO, UVcoords).x > 0) {
        vec3 norm = normalize(texture(gNorm, UVcoords).xyz);
        vec2 UVsize = textureSize(ShadedImg, 0);

        //Get reflective ray direction
        vec3 FragPos = texture(gPos, UVcoords).xyz;
        vec3 viewDir = normalize(camPos - FragPos);

        vec3 rayDir = FragPos + reflect(viewDir, norm);

        //Trace ray
        vec4 rayStartPos = projection * vec4(FragPos, 0.0);

        vec3 rayStartUV = rayStartPos.xyz / rayStartPos.w;
        rayStartUV.xy = rayStartPos.xy * 0.5 + 0.5;
        rayStartUV.xy *= UVsize;

        vec4 rayEndPos = vec4(rayDir, 0.0);

        vec4 rayUVend = rayEndPos * projection;
        rayUVend.xyz /= rayUVend.w;
        rayUVend.xy = rayUVend.xy * 0.5 + 0.5;
        rayUVend.xy *= UVcoords;

        //Find intersections
        float lCamRay = distance(FragPos, rayEndPos.xyz);
        float lDepth = texture(Depth, rayUVend.xy).r;

        while (lCamRay > lDepth) {
            if(lCamRay > maxDist) {
                break;
            }

            rayEndPos += steps;
            
            vec4 rayUVend = rayEndPos * projection;
            rayUVend.xyz /= rayUVend.w;
            rayUVend.xy = rayUVend.xy * 0.5 + 0.5;
            rayUVend.xy *= UVcoords;
        }

        ssr_color = texture(ShadedImg, rayUVend.xy).rgb;
    } else {
        ssr_color = vec3(0.0, 0.0, 0.0);
    }
}