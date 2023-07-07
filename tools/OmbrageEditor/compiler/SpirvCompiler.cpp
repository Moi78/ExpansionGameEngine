#include "SpirvCompiler.h"

SpirvCompiler::SpirvCompiler() {

}

SpirvCompiler::~SpirvCompiler() {

}

bool SpirvCompiler::LoadDependency(std::string deps) {
    if(m_funcTable.find(deps) != m_funcTable.end()) {
        std::cout << "Loading deps : " << deps << std::endl;
        m_funcTable[deps] = {};
        return true;
    } else {
        std::cout << "Already loaded " << deps << std::endl;
        return true;
    }
}

void SpirvCompiler::SetShaderLayout(std::vector<HLTypes> layout) {
    m_shaderLayout = std::move(layout);
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
