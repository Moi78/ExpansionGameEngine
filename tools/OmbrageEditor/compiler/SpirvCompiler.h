#ifndef EXPGE_SPIRVCOMPILER_H
#define EXPGE_SPIRVCOMPILER_H

#include <string>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <optional>
#include <filesystem>

#include "SpirvOperation.h"
#include "SpirvSpecialOP.h"
#include "SpirvFunction.h"
#include "SpirvData.h"
#include "SpirvTypes.h"
#include "SpirvProgram.h"
#include "FunctionGraph.h"
#include "FunctionDeserializer.h"

class SpirvCompiler {
public:
    SpirvCompiler();
    ~SpirvCompiler();

    bool LoadDependency(std::string deps, std::string libRoot);
    void MakeEntry();

    std::vector<uint32_t> CompileAll();

    void AddFunCallToEntry(std::shared_ptr<SpOpFunCall> call);
    void AddOpToEntry(std::shared_ptr<SpirvOperation> op);

    void SetShaderLayout(std::vector<HLTypes> layout);
    void SetFunctionGraphForOutput(int idx, std::unique_ptr<FuncGraphElem> root);

    void RegisterConstant(std::shared_ptr<SpirvConstant> ctant);
    std::shared_ptr<SpirvDataWrapperBase> GetConstantW(std::shared_ptr<SpirvConstant> ctant);

    std::shared_ptr<SpirvFunction> GetFunction(std::string funcName);
    std::shared_ptr<SpirvVariable> GetLayoutVariable(int idx) { return m_prog->GetLayoutVariable(idx); }
private:
    std::unordered_map<std::string, std::shared_ptr<SpirvFunction>> m_funcTable;
    std::unique_ptr<SpirvFunction> m_entry;
    std::unique_ptr<SpirvProgram> m_prog;

    std::vector<std::shared_ptr<SpirvConstant>> m_ctant_table;
    std::vector<std::shared_ptr<SpirvDataWrapperCtant>> m_ctant_wrap;

    std::unordered_map<int, std::shared_ptr<FuncGraphElem>> m_callGraph;
};


#endif //EXPGE_SPIRVCOMPILER_H
