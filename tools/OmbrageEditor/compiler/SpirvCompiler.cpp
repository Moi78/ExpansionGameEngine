#include "SpirvCompiler.h"

SpirvCompiler::SpirvCompiler() {

}

SpirvCompiler::~SpirvCompiler() {

}

bool SpirvCompiler::LoadDependency(std::string deps, std::string libRoot) {
    if(m_funcTable.find(deps) == m_funcTable.end()) {
        if(!std::filesystem::exists(libRoot + deps + ".func")) {
            std::cerr << "ERROR: Missing dependency " << libRoot + deps + ".func" << std::endl;
            return false;
        }

        std::cout << "Loading deps : " << deps << std::endl;

        FunctionDeserializer des{};
        auto func = des.Deserialize(libRoot + deps + ".func");

        m_funcTable[deps] = func;
        return true;
    } else {
        std::cout << "Already loaded " << deps << std::endl;
        return true;
    }
}

void SpirvCompiler::SetShaderLayout(std::vector<HLTypes> layout) {
    m_prog->SetShaderLayout(layout);
}

void SpirvCompiler::SetFunctionGraphForOutput(int idx, std::unique_ptr<FuncGraphElem> root) {
    m_callGraph[idx] = std::move(root);
}

void SpirvCompiler::RegisterConstant(std::shared_ptr<SpirvConstant> ctant) {
    std::cout << "Registered constant of type " << (int)ctant->type << std::endl;

    m_ctant_table.push_back(ctant);

    auto datawrap = std::make_shared<SpirvDataWrapperCtant>();
    datawrap->ctant = ctant;
    m_ctant_wrap.push_back(datawrap);
}

std::shared_ptr<SpirvDataWrapperBase> SpirvCompiler::GetConstantW(std::shared_ptr<SpirvConstant> ctant) {
    int i = 0;
    for(auto& c : m_ctant_table) {
        if(c == ctant) {
            return m_ctant_wrap[i];
        }

        i++;
    }

    return {};
}

void SpirvCompiler::MakeEntry() {
    if(m_entry) {
        m_entry.reset();
    }

    if(m_prog) {
        m_prog.reset();
    }

    m_prog = std::make_unique<SpirvProgram>();

    m_entry = std::make_unique<SpirvFunction>();
    m_entry->argsType = {};
    m_entry->returnType = HLTypes::VOID;
    m_entry->funcName = "main";
}

std::shared_ptr<SpirvFunction> SpirvCompiler::GetFunction(std::string funcName) {
    return m_funcTable[funcName];
}

void SpirvCompiler::AddOpToEntry(std::shared_ptr<SpirvOperation> op) {
    m_entry->funcBody.push_back(op);
}

void SpirvCompiler::AddFunCallToEntry(std::shared_ptr<SpOpFunCall> call) {
    call->id = m_prog->GetAvailableID();
    m_entry->funcBody.push_back(call);
}

std::vector<uint32_t> SpirvCompiler::CompileAll() {
    auto OpReturn = std::make_shared<SpirvOperation>();
    OpReturn->LoadOp(253, 1);

    m_entry->funcBody.push_back(OpReturn);

    m_prog->SetEntryPoint(std::move(m_entry));

    for(auto& f : m_funcTable) {
        m_prog->RegisterFunction(f.second);
    }

    for(auto& c : m_ctant_table) {
        m_prog->RegisterConstant(c);
    }

    m_prog->AssignFuncIDs();
    m_prog->CompileHeader();
    m_prog->CompileTypesFunctions();
    m_prog->Concat();

    return m_prog->GetProgramListing();
}
