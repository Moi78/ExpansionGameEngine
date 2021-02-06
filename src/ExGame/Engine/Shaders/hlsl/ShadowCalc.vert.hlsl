struct VertInput {
    float3 pos : POSITION;
    float2 UV : TEXCOORD;
};

struct PixelOut {
    float2 UV : TEXCOORD;
};

float3 pos : SV_POSITION;

PixelOut main(VertInput inpt) {
    PixelOut vsOut;
    vsOut.UV = inpt.UV;

    pos = inpt.pos;

    return vsOut;
}