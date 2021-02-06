struct PixelOut {
	float2 UV : TEXCOORD0;
};

static float ssao_blur : SV_TARGET1;

Texture2D ssao_tex : register(t0);

SamplerState TexSampler {
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

void main(PixelOut inpt) {
	float2 texSize;
	ssao_tex.GetDimensions(texSize.x, texSize.y);
	
	float2 texelSize = 1.0 / texSize;
	float result = 0.0;
	for (int x = -2; x < 2; x++) {
		for (int y = -2; y < 2; y++) {
			float2 offset = float2(float(x), float(y)) * texelSize;
			result += ssao_tex.Sample(TexSampler, inpt.UV).r;
		}
	}

	ssao_blur = clamp(result / (4.0 * 4.0), 0.0, 1.0);
}