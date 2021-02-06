struct VertInput {
    float3 pos : POSITION;
};

struct PixelOut {
    float4 pos : SV_POSITION;
};

cbuffer CAMERA_DATA : register(b0) {
    float4x4 projection;
    float4x4 view;
    float4x4 model;
};

PixelOut main(VertInput inpt) {
    PixelOut vsOut;

    vsOut.pos = mul(float4(inpt.pos, 1.0), model);
    vsOut.pos = mul(vsOut.pos, view);
    vsOut.pos = mul(vsOut.pos, projection);

    return vsOut;
}