#include "SpirvProgram.h"
#include "SpirvOperation.h"

std::shared_ptr<SPVType> SpirvProgram::GetType(HLTypes t) {
    // Do type exists
    if(m_types.find(t) != m_types.end()) {
        return m_types[t];
    }

    std::shared_ptr<SPVType> newType;
    HLTypes test = t & 0xF;
    switch(t & 0xFF) {
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

    m_IDcounter++;

    if((uint32_t)(t & FLAG_PTR_FUNCTION)) {
        newType->m_isPointer = true;
        newType->m_ptrAttr = StorageClass::Function;

        auto deptype = GetType(t & 0xFF);
    } else if ((uint32_t)(t & FLAG_PTR_OUTPUT)) {
        newType->m_isPointer = true;
        newType->m_ptrAttr = StorageClass::Output;

        auto deptype = GetType(t & 0xFF);
    }

    std::cout << "TYPE " << (int)t << " IS " << newType->m_id << std::endl;

    m_types[t] = newType;

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

void SpirvProgram::RegisterFunction(std::shared_ptr<SpirvFunction> func) {
    m_functions.push_back(func);
}

void SpirvProgram::RegisterConstant(std::shared_ptr<SpirvConstant> ctant) {
    ctant->id = m_IDcounter;
    m_IDcounter++;

    auto gettype = GetType(ctant->type);

    std::cout << "CTANT IS " << ctant->id << std::endl;

    m_ctants.push_back(ctant);
}

void SpirvProgram::RegisterVariable(std::shared_ptr<SpirvVariable> var) {
    var->id = m_IDcounter;
    m_IDcounter++;

    auto gettype = GetType(var->type);

    m_vars.push_back(var);
}

void SpirvProgram::SetShaderLayout(std::vector<HLTypes> layout) {
    for(auto& t : layout) {
        auto t_res = GetType(t);

        auto var = std::make_shared<SpirvVariable>();
        var->id = m_IDcounter;
        var->type = t;

        m_shaderLayout.push_back(var);
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
    OpEntryP.LoadOp(15, 4 + m_shaderLayout.size());
    OpEntryP.words = {
           0x04,
           (uint32_t)m_entryPoint->funcID,
           toint
    };

    for(auto& lid : m_shaderLayout) {
        OpEntryP.words.push_back(lid->id);
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
    for(int i = 0; i < m_shaderLayout.size(); i++) {
        // OpDecorate ID Location i
        SpirvOperation opdec{};
        opdec.LoadOp(71, 3 + 1);
        opdec.words = {
                (uint32_t)m_shaderLayout[i]->id,
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

    // Primary monotypes
    for(auto& t : m_types) {
        std::vector<uint32_t> tdecl;
        if(!t.second->m_isPointer && (t.first == HLTypes::FLOAT || t.first == HLTypes::INT || t.first == HLTypes::VOID)) {
            tdecl = t.second->GetTypeDecl();
            m_shaderBody.insert(m_shaderBody.end(), tdecl.begin(), tdecl.end());
        }
    }

    // Primary multitypes
    for(auto& t : m_types) {
        std::vector<uint32_t> tdecl;
        if(!(t.second->m_isPointer) && ((uint32_t)(t.first & FLAG_IS_VECTOR))) {
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

    // Vars & Ctants
    for(auto& c : m_ctants) {
        std::vector<uint32_t> cdecl = c->DeclConstant(m_types);
        m_shaderBody.insert(m_shaderBody.end(), cdecl.begin(), cdecl.end());
    }

    for(auto& v : m_vars) {
        std::vector<uint32_t> cdecl = v->DeclVariable(m_types);
        m_shaderBody.insert(m_shaderBody.end(), cdecl.begin(), cdecl.end());
    }


    // Shader layout
    for(auto& var : m_shaderLayout) {
        std::vector<uint32_t> vardecl = var->DeclVariable(m_types);
        m_shaderBody.insert(m_shaderBody.end(), vardecl.begin(), vardecl.end());
    }

    m_shaderBody.insert(m_shaderBody.end(), funcs.begin(), funcs.end());
    m_shaderBody.insert(m_shaderBody.end(), entryFunc.begin(), entryFunc.end());
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

std::shared_ptr<SpirvVariable> SpirvProgram::GetLayoutVariable(int idx) {
    return m_shaderLayout[idx];
}

int SpirvProgram::GetAvailableID() {
    int id = m_IDcounter;
    m_IDcounter++;

    return id;
}