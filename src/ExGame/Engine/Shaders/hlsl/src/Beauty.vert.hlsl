struct VertexInput {
    float3 pos : POSITION;
    float2 UV : TEXCOORD;
};

struct PixelIn {
    float4 pos : SV_POSITION;
    float2 UV : TEXCOORD;
};

cbuffer VS_CONSTANT_BUFFER : register(b0) {
    float4x4 model;
    float4x4 view;
    float4x4 proj;
};

PixelIn main(VertexInput IN) {
    PixelIn vsOut;

    vsOut.pos = mul(float4(IN.pos, 1.0), model);
    vsOut.pos = mul(vsOut.pos, view);
    vsOut.pos = mul(vsOut.pos, proj);

    return vsOut;
}