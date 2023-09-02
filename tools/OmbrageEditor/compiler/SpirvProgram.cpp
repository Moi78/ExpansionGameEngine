#include "SpirvProgram.h"
#include "SpirvOperation.h"

std::vector<uint32_t> StringToUint32(std::string str) {
    std::vector<uint32_t> res(str.size() / 4);
    memcpy(res.data(), str.data(), str.size());

    res.push_back(0x00);

    return res;
}

std::shared_ptr<SPVType> SpirvProgram::GetType(HLTypes t) {
    // Do type exists
    if(m_types.find(t) != m_types.end()) {
        return m_types[t];
    }

    std::shared_ptr<SPVType> newType;
    HLTypes test = t & 0xFF;
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
        case HLTypes::IMAGE:
            newType = std::make_shared<TImage>(m_IDcounter, GetType(HLTypes::FLOAT));
            break;
        case HLTypes::SAMPLED_IMAGE:
            newType = std::make_shared<TSampledImage>(m_IDcounter, GetType(HLTypes::IMAGE));
            break;
        case HLTypes::MAT3:
            newType = std::make_shared<TMat3>(m_IDcounter, GetType(HLTypes::VECTOR3));
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
    } else if((uint32_t)(t & FLAG_PTR_INPUT)) {
        newType->m_isPointer = true;
        newType->m_ptrAttr = StorageClass::Input;

        auto deptype = GetType(t & 0xFF);
    } else if((uint32_t)(t & FLAG_PTR_UNIFORM_CTANT)) {
        newType->m_isPointer = true;
        newType->m_ptrAttr = StorageClass::UniformConstant;

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

void SpirvProgram::SetShaderInputs(std::vector<HLTypes> inputs) {
    for(auto& t : inputs) {
        auto t_res = GetType(t);

        auto var = std::make_shared<SpirvVariable>();
        var->id = m_IDcounter;
        var->type = t;
        var->st_class = StorageClass::Input;

        m_shaderInputs.push_back(var);
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

    std::vector<uint32_t> str = StringToUint32("GLSL.std.450");

    SpirvOperation OpExtImp{};
    OpExtImp.LoadOp(11, 2 + str.size());
    OpExtImp.words = {(uint32_t)m_glsl_ext};

    OpExtImp.words.insert(OpExtImp.words.end(), str.begin(), str.end());
    allOp.emplace_back(OpExtImp);

    // Memory Model Logical Simple
    SpirvOperation OpMemModel{};
    OpMemModel.LoadOp(14, 3);
    OpMemModel.words = { 0x00, 0x00 };
    allOp.emplace_back(OpMemModel);

    // Entry point declaration as Fragment
    char name[5] = {'m', 'a', 'i', 'n', '\0'};
    uint32_t toint[2];
    memcpy(&toint, name, 5);

    SpirvOperation OpEntryP{};
    OpEntryP.LoadOp(15, 5 + m_shaderLayout.size() + m_shaderInputs.size());
    OpEntryP.words = {
           0x04,
           (uint32_t)m_entryPoint->funcID,
           toint[0],
           toint[1]
    };

    for(auto& lid : m_shaderLayout) {
        OpEntryP.words.push_back(lid->id);
    }

    for(auto& inp_id : m_shaderInputs) {
        OpEntryP.words.push_back(inp_id->id);
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

    for(int i = 0; i < m_shaderInputs.size(); i++) {
        // OpDecorate ID Location i
        SpirvOperation opdec{};
        opdec.LoadOp(71, 3 + 1);
        opdec.words = {
                (uint32_t)m_shaderInputs[i]->id,
                30,
                (uint32_t)i
        };

        allOp.push_back(opdec);
    }

    for(auto& v : m_vars) {
        std::vector<uint32_t> vdecorate = v->DecorateVariable();

        if(!vdecorate.empty()) {
            SpirvOperation deco{};
            deco.op = vdecorate[0];
            deco.words.insert(deco.words.begin(), vdecorate.begin() + 1, vdecorate.end());

            allOp.push_back(deco);
        }
    }

    // Making header raw binary

    m_progBin.clear();
    m_progBin.push_back(0x07230203); // Magic number
    m_progBin.push_back(0x00010600); // Version number
    m_progBin.push_back(0x00);
    m_progBin.push_back(0); // Bound SET LATER
    m_progBin.push_back(0x00); // Bound

    for(auto& op : allOp) {
        auto opdata = op.GetData();
        m_progBin.insert(m_progBin.end(), opdata.begin(), opdata.end());
    }

}

void SpirvProgram::CompileTypesFunctions() {
    std::vector<uint32_t> funcs;
    std::vector<uint32_t> funcsTypeDecls;

    for(auto& f : m_functions) {
        FunctionTypeLinker(f);

        auto ftypedecl = f->GetTypeDecl(m_types, m_IDcounter);
        auto compiled = f->CompileFunction(m_types, m_glsl_ext, m_IDcounter);

        funcs.insert(funcs.end(), compiled.begin(), compiled.end());
        funcsTypeDecls.insert(funcsTypeDecls.end(), ftypedecl.begin(), ftypedecl.end());
    }

    FunctionTypeLinker(m_entryPoint);

    auto entryType = m_entryPoint->GetTypeDecl(m_types, m_IDcounter);
    funcsTypeDecls.insert(funcsTypeDecls.end(), entryType.begin(), entryType.end());

    auto entryFunc = m_entryPoint->CompileFunction(m_types, m_glsl_ext, m_IDcounter);

    // Primary monotypes
    for(auto& t : m_types) {
        std::vector<uint32_t> tdecl;
        if(
                !t.second->m_isPointer &&
                (t.first == HLTypes::FLOAT ||
                t.first == HLTypes::INT ||
                t.first == HLTypes::VOID)
        ) {
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

    for(auto& t : m_types) {
        std::vector<uint32_t> tdecl;
        if(t.first == HLTypes::IMAGE || t.first == HLTypes::SAMPLED_IMAGE || (t.first == HLTypes::MAT3)) {
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

    // Func types
    m_shaderBody.insert(m_shaderBody.end(), funcsTypeDecls.begin(), funcsTypeDecls.end());

    // Vars & Ctants
    for(auto& c : m_ctants) {
        std::vector<uint32_t> cdecl = c->DeclConstant(m_types, m_IDcounter);
        m_shaderBody.insert(m_shaderBody.end(), cdecl.begin(), cdecl.end());
    }

    for(auto& v : m_vars) {
        std::vector<uint32_t> vdecl = v->DeclVariable(m_types);
        m_shaderBody.insert(m_shaderBody.end(), vdecl.begin(), vdecl.end());
    }


    // Shader layout
    for(auto& var : m_shaderLayout) {
        std::vector<uint32_t> vardecl = var->DeclVariable(m_types);
        m_shaderBody.insert(m_shaderBody.end(), vardecl.begin(), vardecl.end());
    }

    // Shader inputs
    for(auto& var : m_shaderInputs) {
        std::vector<uint32_t> vardecl = var->DeclVariable(m_types);
        m_shaderBody.insert(m_shaderBody.end(), vardecl.begin(), vardecl.end());
    }

    std::cout << "Bound : " << m_IDcounter << std::endl;
    m_progBin[3] = m_IDcounter + 1;

    m_shaderBody.insert(m_shaderBody.end(), entryFunc.begin(), entryFunc.end());
    m_shaderBody.insert(m_shaderBody.end(), funcs.begin(), funcs.end());
}

void SpirvProgram::FunctionTypeLinker(std::shared_ptr<SpirvFunction> &func) {
    auto rtype = GetType(func->returnType);
    auto rtype_ptr = GetType(func->returnType | FLAG_PTR_FUNCTION);
    auto floatptr = GetType(HLTypes::FLOAT | FLAG_PTR_FUNCTION);

    for(auto& type : func->argsType) {
        auto argType = GetType(type);
        auto argPtr_t = GetType(type | FLAG_PTR_FUNCTION);
    }

    for(auto& type : func->typeDeps) {
        auto depType = GetType(type);
    }
}

void SpirvProgram::AssignFuncIDs() {
    m_glsl_ext = m_IDcounter;
    m_IDcounter++;

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

std::shared_ptr<SpirvVariable> SpirvProgram::GetInputVariable(int idx) {
    return m_shaderInputs[idx];
}
