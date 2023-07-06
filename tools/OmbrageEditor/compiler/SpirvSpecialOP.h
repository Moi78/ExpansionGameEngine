#ifndef EXPGE_SPIRVSPECIALOP_H
#define EXPGE_SPIRVSPECIALOP_H

#include "SpirvFunction.h"
#include "SpirvData.h"
#include "SpirvOperation.h"

struct SpOpFunCall : public SpirvOperation {
    SpOpFunCall(std::shared_ptr<SpirvFunction> fn, std::vector<std::shared_ptr<SpirvDataWrapperBase>> arg, bool nStore = false);

    std::shared_ptr<SpirvFunction> func;
    std::vector<std::shared_ptr<SpirvDataWrapperBase>> args;
    std::shared_ptr<SpirvVariable> target;

    int id;
    bool noStore;

    void PreCompile(std::unordered_map<HLTypes, std::shared_ptr<SPVType>> realTypes) override;
};

struct SpirvDataWrapperFunRet : public SpirvDataWrapperBase {
    uint32_t GetReflectedID() override;
    HLTypes GetReflectedType() override;

    std::shared_ptr<SpOpFunCall> fcall;
};

struct SpOpStoreCtant : public SpirvOperation {
    SpOpStoreCtant(std::shared_ptr<SpirvVariable> targ, std::shared_ptr<SpirvConstant> ct);

    std::shared_ptr<SpirvVariable> target;
    std::shared_ptr<SpirvConstant> ctant;

    void PreCompile(std::unordered_map<HLTypes, std::shared_ptr<SPVType>> realTypes) override;

};

#endif //EXPGE_SPIRVSPECIALOP_H
