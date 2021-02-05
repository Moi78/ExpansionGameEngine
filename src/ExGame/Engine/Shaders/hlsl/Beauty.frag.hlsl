cbuffer PS_CONSTANT_BUFFER : register(b1) {
    Texture2D lightpass;
    Texture2D GUIscreen;
    Texture2D bloom;
    Texture2D SSR;
};

SamplerState TexSampler {
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

struct PixelIn {
    float4 pos : SV_POSITION;
    float2 UV : TEXCOORD;
};

float4 main(PixelIn input) : SV_TARGET {
    float4 gui = GUIscreen.Sample(TexSampler, input.UV);
    float4 light = clamp(
        lightpass.Sample(TexSampler, input.UV) + bloom.Sample(TexSampler, input.UV),
        0.0, 1.0);

    return lerp(light, gui, gui.a);
}