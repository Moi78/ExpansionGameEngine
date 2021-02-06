struct VertInput {
    float3 pos : POSITION;
};

static float3 fpos : SV_POSITION;

void main(VertInput intp) {
    fpos = intp.pos;
}