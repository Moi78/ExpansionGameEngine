#ifndef EXPGE_SPIRVFUNCTION_H
#define EXPGE_SPIRVFUNCTION_H

#include "SpirvTypes.h"
#include "SpirvData.h"
#include "SpirvOperation.h"

#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include <cassert>
#include <optional>

#define ID_PLACEHOLDER 0xFFFFFFFF

struct SpirvFunction {
    HLTypes returnType;
    std::vector<HLTypes> argsType;

    std::vector<std::shared_ptr<SpirvOperation>> funcBody;
    //std::vector<std::shared_ptr<SpirvVariable>> funcVars;

    uint32_t funcID;
    std::string funcName;

    int funcSize;

    std::vector<uint32_t> CompileFunction(std::unordered_map<HLTypes, std::shared_ptr<SPVType>> realTypes, int& nextID);
    std::vector<uint32_t> GetTypeDecl(std::unordered_map<HLTypes, std::shared_ptr<SPVType>> realTypes, int& nextID);
};

#endif //EXPGE_SPIRVFUNCTION_H
