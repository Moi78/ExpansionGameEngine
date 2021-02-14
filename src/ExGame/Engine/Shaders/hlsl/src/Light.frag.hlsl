struct PixelOut {
    float4 pos : SV_POSITION;
    float2 UV : TEXCOORD;
};

Texture2D ShadowPass : register(t0);
Texture2D gPos : register(t1);
Texture2D gAlbedo : register(t2);
Texture2D gSpec : register(t3);
Texture2D gNormal : register(t4);

cbuffer LightData : register(b1) {
    float AmbientStrength;
    float AmbientColor;
}

cbuffer PointLights : register(b2) {
    float3 LightPos[243];
    float LightBrightness[243];
    float3 LightColor[243];
    float LightRadius[243];
    int nbrPointLight;
}

cbuffer DirLights : register(b3) {
    float3 DirLightDir[10];
    float3 DirLightColor[10];
    float DirLightBrightness[10];
    int nbrDirLight;
}

cbuffer Data : register(b4) {
    float3 CamPos;
}

SamplerState TexSampler {
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

static float3 norm;
static float3 viewDir;
static float SpecExp;
static float Specular;
static float3 FragPos;

float3 CalcDirLight(int index) {
    float3 dir = normalize(float3(-DirLightDir[index]));

    float diff = max(0.0, dot(norm, dir));
    float3 diffuse = (diff * DirLightBrightness[index] * DirLightColor[index]);


    float3 d_specular = float3(0.0, 0.0, 0.0);
    
    float3 reflectDir = reflect(-dir, norm);
    float spec = pow(max(0.0, dot(viewDir, reflectDir)), SpecExp);
    d_specular = spec * DirLightColor[index] * DirLightBrightness[index] * Specular;

    return diffuse + d_specular;
}

float3 CalcPointLight(int index) {
    float dist = length(LightPos[index] - FragPos);
    if(dist < LightRadius[index]) {
        float3 LightDir = normalize(LightPos[index] - FragPos);
        float attenuation = (1.0 / (1.0 + 0.7 * (dist * dist)));

        float diffuse = LightBrightness[index] * max(0.0, dot(norm, LightDir));

        float3 specular = float3(0.0, 0.0, 0.0);
        float3 reflectDir = reflect(-LightDir, norm);

        float3 hwDir = normalize(LightDir + viewDir);
        float spec = pow(max(0.0, dot(norm, hwDir)), SpecExp);

        specular = (spec * LightColor[index] * Specular);

        return attenuation * (LightColor[index] * (diffuse + specular));
    } else {
        return 0.0;
    }
}

float4 main(PixelOut inpt) : SV_TARGET0 {
    norm = normalize(gNormal.Sample(TexSampler, inpt.UV));
    FragPos = gPos.Sample(TexSampler, inpt.UV);

    float3 Diffuse = gAlbedo.Sample(TexSampler, inpt.UV).rgb;
    Specular = gAlbedo.Sample(TexSampler, inpt.UV).a;
    SpecExp = gSpec.Sample(TexSampler, inpt.UV).r;

    float3 diffSpec = float3(0.0, 0.0, 0.0);
    float shadow = ShadowPass.Sample(TexSampler, inpt.UV).r;

    for(int i = 0; i < nbrDirLight; i++) {
        diffSpec += CalcDirLight(i) * shadow;
    }

    for(int i = 0; i < nbrPointLight; i++) {
        diffSpec += CalcPointLight(i);
    }

    float3 ambient = AmbientColor * AmbientStrength;
    float3 result = (diffSpec + ambient);

	float4 gamma = float4(1.0 / 2.2, 1.0 / 2.2, 1.0 / 2.2, 1.0 / 2.2);
    float4 render = clamp(pow(float4(Diffuse, 1.0) * float4(result, 1.0), gamma), 0.0, 1.0);
    render.a = 1.0;

    return render;
}