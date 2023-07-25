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
#include "SpirvConstant.h"

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
    void SetShaderInputs(std::vector<HLTypes> inputs);

    void RegisterConstant(std::shared_ptr<SpirvConstant> ctant);
    std::shared_ptr<SpirvDataWrapperBase> GetConstantW(std::shared_ptr<SpirvConstant> ctant);

    std::shared_ptr<SpirvFunction> GetFunction(std::string funcName);
    std::shared_ptr<SpirvVariable> GetLayoutVariable(int idx) { return m_prog->GetLayoutVariable(idx); }

    std::shared_ptr<SpirvDataWrapperBase> GetInputVariableW(int idx);
    std::shared_ptr<SpirvVariable> GetInputVariable(int idx) { return m_prog->GetInputVariable(idx); }

    int GetAvailableID();
    void RequireType(HLTypes t) { m_prog->GetType(t); }
private:
    std::unordered_map<std::string, std::shared_ptr<SpirvFunction>> m_funcTable;
    std::unique_ptr<SpirvFunction> m_entry;
    std::unique_ptr<SpirvProgram> m_prog;

    std::vector<std::shared_ptr<SpirvConstant>> m_ctant_table;
    std::vector<std::shared_ptr<SpirvConstant>> m_ctant_overflow;
    std::unordered_map<std::shared_ptr<SpirvConstant>, std::shared_ptr<SpirvDataWrapperCtant>> m_ctant_wrap;
};


#endif //EXPGE_SPIRVCOMPILER_H
