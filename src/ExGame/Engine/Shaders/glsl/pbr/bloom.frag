#version 450 core
layout (location = 0) out vec3 bloom;

in vec2 UVcoords;

uniform sampler2D gShaded;
uniform bool horizontal;
uniform int threshold = 0;

//From https://github.com/Jam3/glsl-fast-gaussian-blur/blob/master/13.glsl
vec4 blur13(sampler2D image, vec2 uv, vec2 resolution, vec2 direction) {
  vec4 color = vec4(0.0);
  vec2 off1 = vec2(1.411764705882353) * direction;
  vec2 off2 = vec2(3.2941176470588234) * direction;
  vec2 off3 = vec2(5.176470588235294) * direction;
  color += (texture2D(image, uv) - threshold) * 0.1964825501511404;
  color += texture2D(image, uv + (off1 / resolution)) * 0.2969069646728344;
  color += texture2D(image, uv - (off1 / resolution)) * 0.2969069646728344;
  color += texture2D(image, uv + (off2 / resolution)) * 0.09447039785044732;
  color += texture2D(image, uv - (off2 / resolution)) * 0.09447039785044732;
  color += texture2D(image, uv + (off3 / resolution)) * 0.010381362401148057;
  color += texture2D(image, uv - (off3 / resolution)) * 0.010381362401148057;
  return color;
}

void main() {
    if(horizontal) {
        bloom = clamp(blur13(gShaded, UVcoords, textureSize(gShaded, 0), vec2(1.0, 0.0)).rgb - threshold, 0, 1);
    } else {
        bloom = clamp(blur13(gShaded, UVcoords, textureSize(gShaded, 0), vec2(0.0, 1.0)).rgb - threshold, 0, 1);
    }
}