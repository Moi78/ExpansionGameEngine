#version 450
layout (location = 0) out vec2 moments;

void main() {
    float depth = gl_FragCoord.z / gl_FragCoord.w;
    depth = depth * 0.5 + 0.5;

    moments.x = depth;
    moments.y = pow(depth, 2);

    float dx = dFdx(depth);
    float dy = dFdy(depth);
    moments.y += 0.25*(dx*dx+dy*dy) ;
}