#version 450

float secret(float a, float b) {
	float c = a * b + (a + b);
	return a * b + c;
}

void main() {
	secret(0, 0);
}
