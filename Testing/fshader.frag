#version 330 core

out vec4 FragColor;

uniform vec3 BaseColor;

uniform float AmbientStrength;
uniform vec3 AmbientColor;

void main()
{
    vec3 ambient = AmbientStrength * AmbientColor;
    vec3 result = BaseColor * ambient;

    FragColor = vec4(result, 1.0);
}