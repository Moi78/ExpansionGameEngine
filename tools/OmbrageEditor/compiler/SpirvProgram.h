#ifndef EXPGE_SPIRVPROGRAM_H
#define EXPGE_SPIRVPROGRAM_H

#include <memory>
#include <string>
#include <unordered_map>
#include <iostream>
#include <cstring>

#include "SpirvFunction.h"
#include "SpirvTypes.h"
#include "SpirvData.h"

class SpirvProgram {
public:
    SpirvProgram() : m_IDcounter(1) {};
    virtual ~SpirvProgram() = default;

    void RegisterFunction(std::shared_ptr<SpirvFunction> func);
    void SetEntryPoint(std::unique_ptr<SpirvFunction> entry);

    std::shared_ptr<SpirvFunction> GetFunc(std::string funcName);
    std::shared_ptr<SPVType> GetType(HLTypes t);

    void SetShaderLayout(std::vector<HLTypes> layout);
    void SetShaderInputs(std::vector<HLTypes> inputs);

    void RegisterConstant(std::shared_ptr<SpirvConstant> ctant);
    void RegisterVariable(std::shared_ptr<SpirvVariable> var);

    std::shared_ptr<SpirvVariable> GetLayoutVariable(int idx);
    std::shared_ptr<SpirvVariable> GetInputVariable(int idx);

    std::vector<uint32_t>& GetProgramListing();
    void AssignFuncIDs();

    void CompileHeader();
    void CompileTypesFunctions();
    void Concat();

    int GetAvailableID();

private:
    void FunctionTypeLinker(std::shared_ptr<SpirvFunction>& func);

    std::vector<std::shared_ptr<SpirvFunction>> m_functions;
    std::shared_ptr<SpirvFunction> m_entryPoint;

    std::vector<uint32_t> m_progBin;
    std::vector<uint32_t> m_shaderBody;

    std::vector<std::shared_ptr<SpirvVariable>> m_shaderLayout;
    std::vector<std::shared_ptr<SpirvVariable>> m_shaderInputs;

    std::vector<std::shared_ptr<SpirvConstant>> m_ctants;
    std::vector<std::shared_ptr<SpirvVariable>> m_vars;

    std::unordered_map<HLTypes, std::shared_ptr<SPVType>> m_types;

    int m_IDcounter;
};

#endif //EXPGE_SPIRVPROGRAM_H
