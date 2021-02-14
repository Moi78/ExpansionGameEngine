struct PixelOut {
	float2 UV : TEXCOORD0;
};

static float4 ssao : SV_TARGET0;

Texture2D gPos : register(t0);
Texture2D gNorm : register(t1);
Texture2D noise : register(t2);

cbuffer Data : register(b1) {
	float3 samples[64];

	float4x4 projection;
	float4x4 view;

	int scr_w;
	int scr_h;
};

SamplerState TexSampler {
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

const float radius = 0.5;
const float bias = 0.05;

static float2 noiseScale = float2(scr_w / 4.0, scr_h / 4.0);

void main(PixelOut inpt) {
	float3 FragPos = float4(mul(view, gPos.Sample(TexSampler, inpt.UV))).xyz;
	float3 norm = gNorm.Sample(TexSampler, inpt.UV).xyz;
	float3 randomVec = normalize(noise.Sample(TexSampler, inpt.UV * noiseScale));
	
	float3 tangent = normalize(randomVec - norm * dot(randomVec, norm));
	float3 bitangent = cross(norm, tangent);
	float3x3 TBN = float3x3(tangent, bitangent, norm);
	
	float occlusion = 0.0;
	for (int i = 0; i < 64; i++) {
		float3 samplePos = mul(TBN, samples[i]);
		samplePos = samplePos * radius + FragPos;
		
		float4 offset = float4(samplePos, 1.0);
		offset = mul(projection, offset);
		offset.xy /= offset.w;
		offset.xy = offset.xy * 0.5 + 0.5;
		
		float sampleDepth = float4(mul(view, gPos.Sample(TexSampler, offset.xy))).z;
		
		float rangeCheck = smoothstep(0.0, 1.0, radius / abs(FragPos.z - sampleDepth));
		occlusion += (sampleDepth >= samplePos.z + bias ? 1.0 : 0.0) * rangeCheck;
	}

	occlusion = 1.0 - (occlusion / 64.0);
	ssao = occlusion;
}