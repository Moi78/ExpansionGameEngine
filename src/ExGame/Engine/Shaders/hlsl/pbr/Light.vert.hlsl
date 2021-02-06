struct VertInput {
	float3 pos : POSITION;
	float2 UV : TEXCOORD;
};

struct PixelOut {
	float2 UV : TEXCOORD;
};

static float3 pos : SV_POSITION;

PixelOut main(VertInput inpt) {
	pos = inpt.pos;
	
	PixelOut vsOut;
	vsOut.UV = inpt.UV;
	
	return vsOut;
}