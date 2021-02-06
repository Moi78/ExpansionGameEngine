struct PixelOut {
    float2 UV : TEXCOORD;
};

Texture2D gShaded : register(t0);

cbuffer Data : register(b0)  {
    bool horizontal;
    int threshold;
};

SamplerState TexSampler {
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

float weight[5] = {0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216};

static float3 bloom : SV_TARGET0;

void main(PixelOut inpt) {
    float3 result = gShaded.Sample(TexSampler, inpt.UV) * weight[0];

    float2 texSize;
    gShaded.GetDimensions(texSize.x, texSize.y);
    float2 tex_offset = 1.0 / texSize;

    if(horizontal) {
        for(int i = 0; i < 5; i++) {
            result += clamp(gShaded.Sample(TexSampler, inpt.UV + float2(tex_offset.x * i, 0.0)).rgb, 0, 1);
            result += clamp(gShaded.Sample(TexSampler, inpt.UV - float2(tex_offset.x * i, 0.0)).rgb, 0, 1);
        }
        bloom = clamp(result, 0, 1);
    } else {
        for(int i = 0; i < 5; i++) {
            result += clamp(gShaded.Sample(TexSampler, inpt.UV + float2(tex_offset.y * i, 0.0)).rgb, 0, 1);
            result += clamp(gShaded.Sample(TexSampler, inpt.UV - float2(tex_offset.y * i, 0.0)).rgb, 0, 1);
        }
        bloom = clamp(result, 0, 1);
    }
}