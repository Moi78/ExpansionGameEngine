#ifndef EXPGE_FUNCTIONGRAPH_H
#define EXPGE_FUNCTIONGRAPH_H

#include <string>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <optional>
#include <memory>

#include "SpirvOperation.h"
#include "SpirvSpecialOP.h"
#include "SpirvFunction.h"
#include "SpirvData.h"
#include "SpirvTypes.h"
#include "SpirvProgram.h"

enum FuncGraphElemType {
    FUNCTION,
    CONST
};

struct FuncGraphElem {
    FuncGraphElemType t;

    std::vector<std::unique_ptr<FuncGraphElem>> parents;
};

struct HighLevelFunCall : public FuncGraphElem {
    HighLevelFunCall();

    std::string func;
    std::vector<std::shared_ptr<SpirvDataWrapperBase>> args;
};

struct HighLevelCtant : public FuncGraphElem {
    HighLevelCtant();

    std::shared_ptr<SpirvDataWrapperBase> ctant;
};

#endif //EXPGE_FUNCTIONGRAPH_H
