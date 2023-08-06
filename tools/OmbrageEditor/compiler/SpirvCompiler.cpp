#include "SpirvCompiler.h"

SpirvCompiler::SpirvCompiler() {
    m_bindingCounter = 2;
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

void SpirvCompiler::SetShaderInputs(std::vector<HLTypes> inputs) {
    m_prog->SetShaderInputs(inputs);
}

void SpirvCompiler::RegisterConstant(std::shared_ptr<SpirvConstant> ctant) {
    std::cout << "Registered constant of type " << (int)ctant->type << std::endl;

    for(auto& c : m_ctant_table) {
        if(CtantCompare(c, ctant)) {
            m_ctant_overflow.push_back(ctant);

            auto wrap = m_ctant_wrap[c];
            m_ctant_wrap[ctant] = wrap;
            return;
        }
    }

    m_ctant_table.push_back(ctant);

    auto datawrap = std::make_shared<SpirvDataWrapperCtant>();
    datawrap->ctant = ctant;
    m_ctant_wrap[ctant] = datawrap;
}

std::shared_ptr<SpirvDataWrapperBase> SpirvCompiler::GetConstantW(std::shared_ptr<SpirvConstant> ctant) {
    if(m_ctant_wrap.find(ctant) != m_ctant_wrap.end()) {
        return m_ctant_wrap[ctant];
    } else {
        return {};
    }
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
    m_entry->funcSize = 0;
}

std::shared_ptr<SpirvFunction> SpirvCompiler::GetFunction(std::string funcName) {
    return m_funcTable[funcName];
}

void SpirvCompiler::AddOpToEntry(std::shared_ptr<SpirvOperation> op) {
    m_entry->funcBody.push_back(op);
}

void SpirvCompiler::AddFunCallToEntry(std::shared_ptr<SpOpFunCall> call) {
    call->id = m_prog->GetAvailableID();
    call->data_id = m_prog->GetAvailableID();
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

int SpirvCompiler::GetAvailableID() {
    return m_prog->GetAvailableID();
}

std::shared_ptr<SpirvDataWrapperBase> SpirvCompiler::GetInputVariableW(int idx) {
    RequireType(HLTypes::FLOATPTRI);

    auto wrap = std::make_shared<SpirvDataWrapperVar>();
    wrap->var = GetInputVariable(idx);
    wrap->stcl = StorageClass::Input;

    return wrap;
}

void SpirvCompiler::RegisterTexture(std::string texName) {
    if(m_texTable.find(texName) != m_texTable.end()) {
        return;
    }

    auto uniTex = std::make_shared<SpirvUniform>();
    uniTex->type = HLTypes::SAMPLED_IMAGE_PTRUC;
    uniTex->binding = m_bindingCounter;
    uniTex->id = GetAvailableID();
    uniTex->st_class = StorageClass::UniformConstant;

    m_texTable[texName] = uniTex;

    m_prog->RegisterVariable(uniTex);

    m_bindingCounter++;
}

std::shared_ptr<SpirvDataWrapperBase> SpirvCompiler::GetTextureW(std::string texName) {
    std::shared_ptr<SpirvUniform> uniform = m_texTable[texName];

    auto wrapper = std::make_shared<SpirvDataWrapperVar>();
    wrapper->var = uniform;

    return wrapper;
}

std::unordered_map<std::string, int> SpirvCompiler::GetTextureNames() {
    std::unordered_map<std::string, int> names;
    for(auto& t : m_texTable) {
        names[t.first] = t.second->binding;
    }

    return names;
}
