#include "SpirvProgram.h"

std::shared_ptr<SPVType> SpirvProgram::GetType(HLTypes t) {
    // Do type exists
    if(m_types.find(t) != m_types.end()) {
        return m_types[t];
    }

    std::shared_ptr<SPVType> newType;
    HLTypes test = t & 0xF;
    switch(t & 0xF) {
        case HLTypes::VOID:
            newType = std::make_shared<TVoid>(m_IDcounter);
            break;
        case HLTypes::FLOAT:
            newType = std::make_shared<TFloat>(m_IDcounter);
            break;
        case HLTypes::INT:
            newType = std::make_shared<TInt>(m_IDcounter);
            break;
        case HLTypes::VECTOR2:
            newType = std::make_shared<TVector<2>>(m_IDcounter, GetType(HLTypes::FLOAT));
            break;
        case HLTypes::VECTOR3:
            newType = std::make_shared<TVector<3>>(m_IDcounter, GetType(HLTypes::FLOAT));
            break;
        case HLTypes::VECTOR4:
            newType = std::make_shared<TVector<4>>(m_IDcounter, GetType(HLTypes::FLOAT));
            break;
    }

    if((uint32_t)(t & FLAG_PTR_FUNCTION)) {
        newType->m_isPointer = true;
        newType->m_ptrAttr = StorageClass::Function;
    } else if ((uint32_t)(t & FLAG_PTR_OUTPUT)) {
        newType->m_isPointer = true;
        newType->m_ptrAttr = StorageClass::Output;
    }

    std::cout << "TYPE " << (int)t << " IS " << m_IDcounter << std::endl;

    m_types[t] = newType;
    m_IDcounter++;

    return newType;
}

std::shared_ptr<SpirvFunction> SpirvProgram::GetFunc(std::string funcName) {
    for(auto& f : m_functions) {
        if(f->funcName == funcName) {
            return f;
        }
    }

    return {};
}

std::vector<uint32_t>& SpirvProgram::GetProgramListing() {
    return m_progBin;
}

void SpirvProgram::RegisterFunction(std::unique_ptr<SpirvFunction> func) {
    m_functions.push_back(std::move(func));
}

void SpirvProgram::SetShaderLayout(std::vector<HLTypes> layout) {
    for(auto& t : layout) {
        auto t_res = GetType(t);

        m_layoutIDs.push_back(m_IDcounter);
        m_IDcounter++;
    }
}

void SpirvProgram::CompileHeader() {
    assert(m_entryPoint.use_count() != 0 && "An entry point must be defined");

    std::vector<SpirvOperation> allOp;

    SpirvOperation OpCap{};
    OpCap.LoadOp(17, 2);
    OpCap.words = { 0x01 }; // Capability : Shader
    allOp.emplace_back(OpCap);

    // Memory Model Logical Simple
    SpirvOperation OpMemModel{};
    OpMemModel.LoadOp(14, 3);
    OpMemModel.words = { 0x00, 0x00 };
    allOp.emplace_back(OpMemModel);

    // Entry point declaration as Fragment
    char name[4] = {'m', 'n', '\0', '\0'};
    uint32_t toint = 0;
    memcpy(&toint, name, 4);

    SpirvOperation OpEntryP{};
    OpEntryP.LoadOp(15, 4 + m_layoutIDs.size());
    OpEntryP.words = {
           0x04,
           (uint32_t)m_entryPoint->funcID,
           toint
    };

    for(auto& lid : m_layoutIDs) {
        OpEntryP.words.push_back(lid);
    }
    allOp.emplace_back(OpEntryP);

    // Exec Mode OriginUpperLeft
    SpirvOperation OpExecMode{};
    OpExecMode.LoadOp(16, 3);
    OpExecMode.words = {
            m_entryPoint->funcID,
            0x07
    };
    allOp.emplace_back(OpExecMode);

    // Handling interface decoration
    for(int i = 0; i < m_layoutIDs.size(); i++) {
        // OpDecorate ID Location i
        SpirvOperation opdec{};
        opdec.LoadOp(71, 3 + 1);
        opdec.words = {
                (uint32_t)m_layoutIDs[i],
                30,
                (uint32_t)i
        };

        allOp.push_back(opdec);
    }

    // Making header raw binary

    m_progBin.clear();
    m_progBin.push_back(0x07230203); // Magic number
    m_progBin.push_back(0x00010600); // Version number
    m_progBin.push_back(0x00);
    m_progBin.push_back(m_IDcounter + 1); // Bound
    m_progBin.push_back(0x00); // Bound

    std::cout << "Bound : " << m_IDcounter << std::endl;

    for(auto& op : allOp) {
        auto opdata = op.GetData();
        m_progBin.insert(m_progBin.end(), opdata.begin(), opdata.end());
    }

}

void SpirvProgram::CompileTypesFunctions() {
    FunctionTypeLinker(m_entryPoint);

    auto entryFunc = m_entryPoint->CompileFunction(m_types, m_IDcounter);

    std::vector<uint32_t> funcs;
    for(auto& f : m_functions) {
        FunctionTypeLinker(f);

        auto compiled = f->CompileFunction(m_types, m_IDcounter);

        funcs.insert(funcs.end(), compiled.begin(), compiled.end());
    }

    // Primary types
    for(auto& t : m_types) {
        std::vector<uint32_t> tdecl;
        if(!t.second->m_isPointer) {
            tdecl = t.second->GetTypeDecl();
            m_shaderBody.insert(m_shaderBody.end(), tdecl.begin(), tdecl.end());
        }
    }

    // Pointer types
    for(auto& t : m_types) {
        std::vector<uint32_t> tdecl;
        if(t.second->m_isPointer) {
            tdecl = t.second->GetTypePtrDecl(GetType(t.first & 0xFF));
            m_shaderBody.insert(m_shaderBody.end(), tdecl.begin(), tdecl.end());
        }
    }

    m_shaderBody.insert(m_shaderBody.end(), entryFunc.begin(), entryFunc.end());
    m_shaderBody.insert(m_shaderBody.end(), funcs.begin(), funcs.end());
}

void SpirvProgram::FunctionTypeLinker(std::shared_ptr<SpirvFunction> &func) {
    auto rtype = GetType(func->returnType);
    for(auto type : func->argsType) {
        auto argType = GetType(type);
    }
}

void SpirvProgram::AssignFuncIDs() {
    m_entryPoint->funcID = m_IDcounter;
    m_IDcounter += m_entryPoint->funcSize + 4;

    for(auto& f : m_functions) {
        f->funcID = m_IDcounter;

        m_IDcounter += f->funcSize + 4 + f->argsType.size();
    }
}

void SpirvProgram::SetEntryPoint(std::unique_ptr<SpirvFunction> entry) {
    m_entryPoint = std::move(entry);
}

void SpirvProgram::Concat() {
    m_progBin.insert(m_progBin.end(), m_shaderBody.begin(), m_shaderBody.end());
}
