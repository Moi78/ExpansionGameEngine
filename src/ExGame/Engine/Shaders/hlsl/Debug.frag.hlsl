struct PixelOut {
    float4 pos : SV_POSITION;
};

float4 main(PixelOut inpt) : SV_TARGET {
    return float4(1.0, 1.0, 1.0, 1.0);
}