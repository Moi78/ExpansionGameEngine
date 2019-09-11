#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
//in vec3 DBG_color;

uniform vec3 BaseColor;

uniform float AmbientStrength;
uniform vec3 AmbientColor;

uniform vec3 LightPos;
uniform float LightBrightness;

void main()
{
    vec3 ambient = AmbientStrength * AmbientColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(1.0, 1.0, 1.0);
/*
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * vec3(1.0, 1.0, 1.0);
*/
    vec3 result = BaseColor * (ambient + diffuse);

    FragColor = vec4(result, 1.0);
}