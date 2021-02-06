struct PixelOut {
	float2 UV : TEXCOORD;
};

static float4 LightPass : SV_Target0;

Texture2D ShadowPass : register(t0);
Texture2D gPos : register(t1);
Texture2D gNormal : register(t2);
Texture2D gAlbedo : register(t3);
Texture2D gSpec : register(t4);
Texture2D gMetRoughAO : register(t5);
Texture2D gEmissive : register(t6);
Texture2D ssao : register(t7);

cbuffer Ambient : register(b1) {
	float AmbientStrength;
	float3 AmbientColor;
};

cbuffer PointLights : register(b2) {
	float3 LightPos[243];
	float LightBrightness[243];
	float3 LightColor[243];
	float LightRadius[243];
	int nbrPointLight;
};

cbuffer DirLights : register(b3) {
	float3 DirLightDir[10];
	float DirLightBrightness[10];
	float3 DirLightColor[10];
	int nbrDirLight;
};

cbuffer Data : register(b4) {
	float3 CamPos;
};

SamplerState TexSampler {
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

float PI = 3.14159265359;

static float3 norm;
static float3 FragPos;

static float3 Diffuse;

static float4 metrao;
static float rness;
static float metllc;
static float AO;

static float3 viewDir;
static float3 F0;

float3 FresnelSchlick(float cosTheta, float3 F0) {
	return F0 + (1.0 - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}

float DistributionGGX(float3 N, float3 H, float roughness) {
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH * NdotH;
	
	float num = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	
	return num / denom;
}

float GeomSchlickGGX(float NdotV, float roughness) {
	float r = (roughness + 1.0);
	float k = (r * r) / 8.0;
	
	float num = NdotV;
	float denom = NdotV * (1.0 - k) + k;
	
	return num / denom;
}

float GeomSmith(float3 N, float3 V, float3 L, float roughness) {
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx2 = GeomSchlickGGX(NdotV, roughness);
	float ggx1 = GeomSchlickGGX(NdotL, roughness);
	
	return ggx1 * ggx2;
}

float3 CalcDirLight(int index) {
	float dist = 1000000.0;
	
	float3 ColPow = DirLightColor[index] * DirLightBrightness[index];
	
	float3 L = normalize(float3(-DirLightDir[index]));
	float3 H = normalize(viewDir + L);
	
	float3 radiance = ColPow;
	
	float NDF = DistributionGGX(norm, H, rness);
	float G = GeomSmith(norm, viewDir, L, rness);
	float3 F = FresnelSchlick(max(dot(H, viewDir), 0.0), F0);
	
	float3 kS = F;
	float3 kD = float3(1.0, 1.0, 1.0) - kS;
	kD *= 1.0 - metllc;
	
	float3 numerator = NDF * G * F;
	float denominator = 4.0 * max(dot(norm, viewDir), 0.0) * max(dot(norm, L), 0.0) + 0.001;
	float3 specular = numerator / denominator;
	
	float NdotL = max(dot(norm, L), 0.0);
	
	return (kD * Diffuse / PI + specular) * radiance * NdotL;
}

float3 CalcPointLight(int index) {
	float dist = length(LightPos[index] - FragPos);
	
	if (dist < LightRadius[index]) {
		float3 ColPow = LightColor[index] * LightBrightness[index];
		
		float3 L = normalize(LightPos[index] - FragPos);
		float3 H = normalize(viewDir + L);
		
		float attenuation = 1.0 / (dist * dist);
		float3 radiance = ColPow * attenuation;
		
		float NDF = DistributionGGX(norm, H, rness);
		float G = GeomSmith(norm, viewDir, L, rness);
		float3 F = FresnelSchlick(max(dot(H, viewDir), 0.0), F0);
		
		float3 kS = F;
		float3 kD = float3(1.0, 1.0, 1.0) - kS;
		kD *= 1.0 - metllc;
		
		float3 num = NDF * G * F;
		float denom = 4.0 * max(dot(norm, viewDir), 0.0) * max(dot(norm, L), 0.0) + 0.001;
		float3 specular = num / denom;
		
		float NdotL = max(dot(norm, L), 0.0);
		return (kD * Diffuse / PI + specular) * radiance * NdotL;
	}
	else {
		return float3(0.0, 0.0, 0.0);
	}
}

void main(PixelOut inpt) {
	//Defining constants
	norm = normalize(gNormal.Sample(TexSampler, inpt.UV).rgb);
	FragPos = gPos.Sample(TexSampler, inpt.UV).rgb;
	
	Diffuse = gAlbedo.Sample(TexSampler, inpt.UV).rgb;
	
	metrao = gMetRoughAO.Sample(TexSampler, inpt.UV);
	rness = metrao.g;
	metllc = metrao.r;
	AO = metrao.b;
	
	viewDir = normalize(CamPos - FragPos);
	F0 = lerp(float3(0.04, 0.04, 0.04), Diffuse, metllc);
	
	//Computing light
	float3 diffSpec = float3(0.0, 0.0, 0.0);
	float shadow = ShadowPass.Sample(TexSampler, inpt.UV);
	
	for (int i = 0; i < nbrDirLight; i++) {
		diffSpec += max(CalcDirLight(i), 0.0) * shadow;
	}
	
	for (int i = 0; i < nbrPointLight; i++) {
		diffSpec += max(CalcPointLight(i), 0.0) * shadow;
	}

	float SSAO = ssao.Sample(TexSampler, inpt.UV).r;
	float3 ambient = (AmbientColor * AmbientStrength) * Diffuse * AO;
	float3 result = (diffSpec + ambient) * SSAO;
	
	//Gamma
	result = result / (result + float3(1.0, 1.0, 1.0));
	result = pow(result, float3(1.0 / 2.2, 1.0 / 2.2, 1.0 / 2.2));
	
	//Emissive
	result += gEmissive.Sample(TexSampler, inpt.UV).rgb;
	
	LightPass = float4(result, 1.0);
}