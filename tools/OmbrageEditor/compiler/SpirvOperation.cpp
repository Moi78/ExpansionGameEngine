#include "SpirvOperation.h"

std::vector<uint32_t> SpirvOperation::GetData() {
    std::vector<uint32_t> vec(words);
    vec.insert(vec.begin(), op);

    return vec;
}