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

std::vector<uint32_t> SpirvUniform::DecorateVariable() {
    SpirvOperation opdec_bding{};
    opdec_bding.LoadOp(71, 3 + 1);
    opdec_bding.words = {
            (uint32_t)id,
            33,
            (uint32_t)binding
    };

    SpirvOperation opdec_dset{};
    opdec_dset.LoadOp(71, 3 + 1);
    opdec_dset.words = {
            (uint32_t)id,
            34,
            (uint32_t)descSet
    };

    auto bin_bding = opdec_bding.GetData();
    auto bin_dset = opdec_dset.GetData();

    bin_bding.insert(bin_bding.end(), bin_dset.begin(), bin_dset.end());
    return bin_bding;
}
