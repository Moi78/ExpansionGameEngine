#include "SpirvData.h"
#include "SpirvOperation.h"
#include "SpirvConstant.h"

std::vector<uint32_t> SpirvVariable::DeclVariable(std::unordered_map<HLTypes, std::shared_ptr<SPVType>> realTypes) {
    uint32_t rID = realTypes[type]->m_id;

    SpirvOperation OpVariable{};
    OpVariable.LoadOp(59, 4);
    OpVariable.words = {
        rID,
        (uint32_t)id,
        (uint32_t)st_class
    };

    return OpVariable.GetData();
}

uint32_t SpirvDataWrapperCtant::GetReflectedID() {
    return ctant->id;
}

HLTypes SpirvDataWrapperCtant::GetReflectedType() {
    return ctant->type;
}

uint32_t SpirvDataWrapperVar::GetReflectedID() {
    return var->id;
}

HLTypes SpirvDataWrapperVar::GetReflectedType() {
    return var->type;
}
