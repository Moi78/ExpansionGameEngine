#version 450 core
#extension GL_ARB_bindless_texture : enable
layout (location = 0) out vec3 blurred;

in vec2 UVcoords;

#ifndef GL_ARB_bindless_texture
	uniform sampler2D baseImage;
#else
	layout(std430, binding = 9) buffer BINDLESS_SFX {
		sampler2D sfx_passes[4];
	};
#endif

layout(std430, binding = 10) buffer BLUR_STATE {
	vec3 dir;
	int index;
	int threshold;
};

//From https://github.com/Jam3/glsl-fast-gaussian-blur/blob/master/13.glsl
vec4 blur13(sampler2D image, vec2 uv, vec2 resolution, vec2 direction) {
  vec4 color = vec4(0.0);
  vec2 off1 = vec2(1.411764705882353) * direction;
  vec2 off2 = vec2(3.2941176470588234) * direction;
  vec2 off3 = vec2(5.176470588235294) * direction;
  color += texture2D(image, uv) * 0.1964825501511404;
  color += texture2D(image, uv + (off1 / resolution)) * 0.2969069646728344;
  color += texture2D(image, uv - (off1 / resolution)) * 0.2969069646728344;
  color += texture2D(image, uv + (off2 / resolution)) * 0.09447039785044732;
  color += texture2D(image, uv - (off2 / resolution)) * 0.09447039785044732;
  color += texture2D(image, uv + (off3 / resolution)) * 0.010381362401148057;
  color += texture2D(image, uv - (off3 / resolution)) * 0.010381362401148057;
  return color;
}

void main() {
	#ifndef GL_ARB_bindless_texture
		blurred = blur13(baseImage, UVcoords, textureSize(baseImage, 0), dir.xy).rgb;
	#else
		blurred = blur13(sfx_passes[index], UVcoords, textureSize(sfx_passes[0], 0), dir.xy).rgb;
	#endif
}