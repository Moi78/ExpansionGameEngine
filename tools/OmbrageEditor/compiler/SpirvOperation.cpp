#include "SpirvOperation.h"

std::vector<uint32_t> SpirvOperation::GetData() {
    std::vector<uint32_t> vec;

    for(auto& pop : preOp) {
        auto dat = pop->GetData();
        vec.insert(vec.end(), dat.begin(), dat.end());
    }

    vec.push_back(op);
    vec.insert(vec.end(), words.begin(), words.end());

    return vec;
}