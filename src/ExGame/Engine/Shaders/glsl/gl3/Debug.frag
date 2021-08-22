#version 450 core
uniform vec3 debugColor = vec3(1.0, 1.0, 1.0);

out vec3 FragPos;

void main() {
	FragPos = debugColor;
}