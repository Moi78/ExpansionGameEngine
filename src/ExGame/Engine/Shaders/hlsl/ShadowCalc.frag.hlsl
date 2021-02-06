float3 ShadowColor : SV_TARGET0;

Texture2D ShadowMaps[10] : register(t0);

cbuffer ShadowData : register(b0) {
    float4x4 lspaceMat[10];
    int nbrDirLights;

    Texture2D gPos;
};

struct PixelOut {
    float2 UV : TEXCOORD;
};

SamplerState TexSampler {
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

void main(PixelOut inpt) {
    float finalShadow = 0.0;

    for(int i = 0; i < nbrDirLights; i++) {
        float4 fpls = mul(float4(gPos.Sample(TexSampler, inpt.UV)), lspaceMat[i]);

        float3 projCoord = fpls.xyz / fpls.w;
        projCoord = projCoord * 0.5 + 0.5;

        if(projCoord.z > 1.0) {
            ShadowColor = float3(0.0);
        }

        float closestDepth = ShadowMaps[i].Sample(TexSampler, inpt.UV).r;
        float currentDepth = projCoord.z;

        float bias = 0.0015;
        
        float shadow = 0.0;

        float2 textureSize;
        ShadowMaps[i].GetDimensions(textureSize.x, textureSize.y);

        float2 texelSize = 1.0 / textureSize;
        for(float x = -1; x <= 1; x += 1) {
            for(float y = -1; y <= 1; y += 1) {
                float PCFdepth = ShadowMaps[i].Sample(TexSampler, projCoord.xy + float2(x, y));
                shadow += currentDepth - bias > PCFdepth ? 1.0 : 0.0;
            }
        }

        finalShadow += clamp(shadow / 9, 0, 1);
    }
}