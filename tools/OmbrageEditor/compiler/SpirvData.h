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
};

struct SpirvConstant {
    int id;
    HLTypes type;

    virtual std::vector<uint32_t> DeclConstant(std::unordered_map<HLTypes, std::shared_ptr<SPVType>> realTypes, int& idcounter) = 0;
};

struct SpirvDataWrapperBase {
    virtual uint32_t GetReflectedID() = 0;
    virtual HLTypes GetReflectedType() = 0;
};

struct SpirvDataWrapperCtant : public SpirvDataWrapperBase {
    std::shared_ptr<SpirvConstant> ctant;

    uint32_t GetReflectedID() override;
    HLTypes GetReflectedType() override;
};

struct SpirvDataWrapperVar : public SpirvDataWrapperBase {
    std::shared_ptr<SpirvVariable> var;

    uint32_t GetReflectedID() override;
    HLTypes GetReflectedType() override;
};

#endif //EXPGE_SPIRVDATA_H
