#ifndef EXPGE_SPIRVFUNCTION_H
#define EXPGE_SPIRVFUNCTION_H

#include "SpirvTypes.h"

#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include <cassert>
#include <optional>

#define ID_PLACEHOLDER 0xFFFFFFFF

struct SpirvOperation {
    uint32_t op;
    std::vector<uint32_t> words;

    std::optional<uint32_t> result_id;

    void LoadOp(uint32_t opid, uint32_t opsize) { op = (opsize << 16) | opid; }

    std::vector<uint32_t> GetData();
};

struct SpirvFunction {
    HLTypes returnType;
    std::vector<HLTypes> argsType;
    std::vector<SpirvOperation> funcBody;

    uint32_t funcID;
    std::string funcName;

    int funcSize;

    std::vector<uint32_t> CompileFunction(std::unordered_map<HLTypes, std::shared_ptr<SPVType>> realTypes, int& nextID);
};

#endif //EXPGE_SPIRVFUNCTION_H
