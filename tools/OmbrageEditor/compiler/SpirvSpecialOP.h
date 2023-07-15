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
    SpOpStoreCtant(std::shared_ptr<SpirvDataWrapperBase> targ, std::shared_ptr<SpirvDataWrapperBase> ct);

    std::shared_ptr<SpirvDataWrapperBase> target;
    std::shared_ptr<SpirvDataWrapperBase> ctant;

    void PreCompile(std::unordered_map<HLTypes, std::shared_ptr<SPVType>> realTypes) override;
};

struct SpOpAccessChain : public SpirvOperation {
    SpOpAccessChain(std::shared_ptr<SpirvVariable> target, std::shared_ptr<SpirvDataWrapperBase> idx, uint32_t rid);

    std::shared_ptr<SpirvVariable> target;
    std::shared_ptr<SpirvDataWrapperBase> vecIdx;
    uint32_t resID;

    StorageClass stcl = StorageClass::Output;

    void PreCompile(std::unordered_map<HLTypes, std::shared_ptr<SPVType>> realTypes) override;
};

struct SpirvDataWrapperAccessChain : public SpirvDataWrapperBase {
    uint32_t GetReflectedID() override;
    HLTypes GetReflectedType() override;

    std::shared_ptr<SpOpAccessChain> ptr;
};

struct SpOpCompExtract : public SpirvOperation {
    SpOpCompExtract(std::shared_ptr<SpirvDataWrapperBase> src, int idx, uint32_t rid);

    std::shared_ptr<SpirvDataWrapperBase> source;
    int vecIdx;
    uint32_t resId;

    void PreCompile(std::unordered_map<HLTypes, std::shared_ptr<SPVType>> realTypes) override;
};

struct SpirvDataWrapperCompExtracted : public SpirvDataWrapperBase {
    uint32_t GetReflectedID() override;
    HLTypes GetReflectedType() override;

    std::shared_ptr<SpOpCompExtract> ptr;
};

#endif //EXPGE_SPIRVSPECIALOP_H
