#ifndef EXPGE_SPIRVDATA_H
#define EXPGE_SPIRVDATA_H

#include "SpirvTypes.h"

#include <cstring>
#include <unordered_map>

struct SpirvVariable {
    int id;

    HLTypes type;
    StorageClass st_class = StorageClass::Output;

    std::vector<uint32_t> DeclVariable(std::unordered_map<HLTypes, std::shared_ptr<SPVType>> realTypes);
    virtual std::vector<uint32_t> DecorateVariable() { return {}; };
};

struct SpirvUniform : public SpirvVariable {
    int binding;
    int descSet = 0;

    std::vector<uint32_t> DecorateVariable();
};

struct SpirvConstant {
    int id;
    HLTypes type;

    virtual std::vector<uint32_t> DeclConstant(std::unordered_map<HLTypes, std::shared_ptr<SPVType>> realTypes, int& idcounter) = 0;
};

struct SimpleCtant {
    HLTypes t;
    uint32_t data;
};

struct SpirvDataWrapperBase {
    virtual uint32_t GetReflectedID() = 0;
    virtual HLTypes GetReflectedType() = 0;

    virtual bool isConst() { return false; }
    virtual bool isVarStored() { return false; }

    StorageClass stcl = StorageClass::Output;
};

struct SpirvDataWrapperCtant : public SpirvDataWrapperBase {
    std::shared_ptr<SpirvConstant> ctant;

    uint32_t GetReflectedID() override;
    HLTypes GetReflectedType() override;

    bool isConst() override { return true; }
};

struct SpirvDataWrapperVar : public SpirvDataWrapperBase {
    std::shared_ptr<SpirvVariable> var;

    uint32_t GetReflectedID() override;
    HLTypes GetReflectedType() override;

    bool isVarStored() override { return true; }
};

#endif //EXPGE_SPIRVDATA_H
