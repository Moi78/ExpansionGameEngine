#version 450

layout(location = 0) out vec4 _2;
layout(location = 1) out vec4 _3;
layout(location = 1) in vec3 _8;
layout(location = 2) out vec4 _4;
layout(location = 2) in vec3 _9;
layout(location = 3) out vec4 _5;
layout(location = 4) out vec4 _6;
layout(location = 0) in vec2 _7;

layout(binding = 0) uniform sampler2D _10;

void main()
{
    vec4 _42 = texture(_10, vec2(0.0));
}
