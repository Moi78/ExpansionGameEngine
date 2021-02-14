struct VertInput {
    float3 pos : POSITION;
    float2 UV : TEXCOORD;
};

struct PixelOut {
    float4 pos : SV_POSITION;
    float2 UV : TEXCOORD;
};

PixelOut main(VertInput inpt) {
    PixelOut vsOut;

	vsOut.pos = float4(inpt.pos, 1.0);
    vsOut.UV = inpt.UV;

    return vsOut;
}