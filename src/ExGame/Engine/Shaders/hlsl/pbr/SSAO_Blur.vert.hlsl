struct VertInput {
	float3 pos : POSITION;
	float2 UV : TEXCOORD0;
};

struct PixelOut {
	float2 UV : TEXCOORD0;
};

static float4 pos : SV_Position;

PixelOut main(VertInput inpt) {
	pos = float4(inpt.pos, 1.0);
	
	PixelOut vsOut;
	vsOut.UV = inpt.UV;
	
	return vsOut;
}