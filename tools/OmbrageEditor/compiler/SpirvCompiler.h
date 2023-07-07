#ifndef EXPGE_SPIRVCOMPILER_H
#define EXPGE_SPIRVCOMPILER_H

#include <string>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <optional>

#include "SpirvOperation.h"
#include "SpirvSpecialOP.h"
#include "SpirvFunction.h"
#include "SpirvData.h"
#include "SpirvTypes.h"
#include "SpirvProgram.h"
#include "FunctionGraph.h"

class SpirvCompiler {
public:
    SpirvCompiler();
    ~SpirvCompiler();

    bool LoadDependency(std::string deps);

    void SetShaderLayout(std::vector<HLTypes> layout);
    void SetFunctionGraphForOutput(int idx, std::unique_ptr<FuncGraphElem> root);

    void RegisterConstant(std::shared_ptr<SpirvConstant> ctant);
    std::shared_ptr<SpirvDataWrapperBase> GetConstantW(std::shared_ptr<SpirvConstant> ctant);

private:
    std::unordered_map<std::string, std::shared_ptr<SpirvFunction>> m_funcTable;

    std::vector<std::shared_ptr<SpirvConstant>> m_ctant_table;
    std::vector<std::shared_ptr<SpirvDataWrapperCtant>> m_ctant_wrap;

    std::unordered_map<int, std::unique_ptr<FuncGraphElem>> m_callGraph;

    std::vector<HLTypes> m_shaderLayout;
};


#endif //EXPGE_SPIRVCOMPILER_H
