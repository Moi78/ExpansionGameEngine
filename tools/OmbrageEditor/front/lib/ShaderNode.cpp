#include "ShaderNode.h"

ShaderNode::ShaderNode(uint32_t id) : Node(id) {
    m_io = {
            { NodePinMode::INPUT, NodePinTypes::VEC3, "Color", 0 },
            { NodePinMode::INPUT, NodePinTypes::VEC3, "Normal", 1 },
            { NodePinMode::INPUT, NodePinTypes::VEC3, "Pos", 2 },
            { NodePinMode::INPUT, NodePinTypes::FLOAT, "Metallic", 3 },
            { NodePinMode::INPUT, NodePinTypes::FLOAT, "Roughness", 4 },
            { NodePinMode::INPUT, NodePinTypes::FLOAT, "AO", 5 },
            { NodePinMode::INPUT, NodePinTypes::VEC4, "Sheen", 6 }
    };

    m_nodeName = "Shader Node";
    m_color[1] = 190.0f;
}

std::vector<std::shared_ptr<SpirvOperation>>
ShaderNode::GetStoreOpForNode(std::shared_ptr<SpirvCompiler> compiler, int idx,
                              std::shared_ptr<SpirvDataWrapperBase> toStore) {
    std::vector<std::shared_ptr<SpirvOperation>> ops;

    compiler->RequireType(HLTypes::FLOATPTRO);

    switch (idx) {
    case 0: case 1: case 2:
        ops = StoreVec3ToVec4(compiler, idx, toStore);
        break;
    case 3: case 4: case 5:
        ops = StoreFloatToVec4(compiler, 3, idx - 3, toStore);
        break;
    case 6:
        ops = StoreVec4ToVec4(compiler, 4, toStore);
        break;
    }

    return ops;
}

std::vector<std::shared_ptr<SpirvOperation>>
ShaderNode::StoreVec3ToVec4(std::shared_ptr<SpirvCompiler> compiler, int layoutidx,
                            std::shared_ptr<SpirvDataWrapperBase> toStore) {
    std::shared_ptr<SPVIntConstant> c0 = std::make_shared<SPVIntConstant>();
    c0->data = 0;
    c0->type = HLTypes::INT;

    std::shared_ptr<SPVIntConstant> c1 = std::make_shared<SPVIntConstant>();
    c1->data = 1;
    c1->type = HLTypes::INT;

    std::shared_ptr<SPVIntConstant> c2 = std::make_shared<SPVIntConstant>();
    c2->data = 2;
    c2->type = HLTypes::INT;

    std::vector<std::shared_ptr<SPVIntConstant>> ct = {c0, c1, c2};

    compiler->RegisterConstant(c0);
    compiler->RegisterConstant(c1);
    compiler->RegisterConstant(c2);

    std::vector<std::shared_ptr<SpirvOperation>> ops;

    auto var = compiler->GetLayoutVariable(layoutidx);

    for(int i = 0; i < 3; i++) {
        auto achain = std::make_shared<SpOpAccessChain>(var, compiler->GetConstantW(ct[i]), compiler->GetAvailableID());
        auto achainptr = std::make_shared<SpirvDataWrapperAccessChain>();
        achainptr->ptr = achain;

        auto opExtract = std::make_shared<SpOpCompExtract>(toStore, i, compiler->GetAvailableID());

        auto extracted = std::make_shared<SpirvDataWrapperCompExtracted>();
        extracted->ptr = opExtract;

        auto store = std::make_shared<SpOpStoreCtant>(achainptr, extracted);

        ops.push_back(achain);
        ops.push_back(opExtract);
        ops.push_back(store);
    }

    return ops;
}

std::vector<std::shared_ptr<SpirvOperation>>
ShaderNode::StoreFloatToVec4(std::shared_ptr<SpirvCompiler> compiler, int layoutidx, int vecidx, std::shared_ptr<SpirvDataWrapperBase> toStore) {
    std::shared_ptr<SPVIntConstant> idx = std::make_shared<SPVIntConstant>();
    idx->data = vecidx;
    idx->data = vecidx;
    idx->type = HLTypes::INT;
    compiler->RegisterConstant(idx);

    std::vector<std::shared_ptr<SpirvOperation>> ops;

    auto var = compiler->GetLayoutVariable(layoutidx);

    auto achain = std::make_shared<SpOpAccessChain>(var, compiler->GetConstantW(idx), compiler->GetAvailableID());
    auto achainptr = std::make_shared<SpirvDataWrapperAccessChain>();
    achainptr->ptr = achain;

    auto store = std::make_shared<SpOpStoreCtant>(achainptr, toStore);

    ops.push_back(achain);
    ops.push_back(store);

    return ops;
}

std::vector<std::shared_ptr<SpirvOperation>>
ShaderNode::StoreVec4ToVec4(std::shared_ptr<SpirvCompiler> compiler, int layoutidx,
                            std::shared_ptr<SpirvDataWrapperBase> toStore) {
    auto var = compiler->GetLayoutVariable(layoutidx);
    auto var_wrap = std::make_shared<SpirvDataWrapperVar>();
    var_wrap->var = var;

    auto opstorectant = std::make_shared<SpOpStoreCtant>(var_wrap, toStore);

    return {opstorectant};
}

std::shared_ptr<SpirvDataWrapperBase> ShaderNode::GetDefault(std::shared_ptr<SpirvCompiler> compiler, int idx) {
    switch (idx) {
    case 0: case 1: case 2:
            return GetVec30(compiler);
            break;
    case 3: case 4: case 5:
            return GetFloat0(compiler);
            break;
    case 6:
            return GetVec40(compiler);
            break;
    }

    return {};
}

std::shared_ptr<SpirvDataWrapperBase> ShaderNode::GetVec30(std::shared_ptr<SpirvCompiler> compiler) {
    auto ct = std::make_shared<SPVVecConstant<3>>();
    ct->data = {0, 0, 0};
    ct->type = HLTypes::VECTOR3;

    compiler->RegisterConstant(ct);

    return compiler->GetConstantW(ct);
}

std::shared_ptr<SpirvDataWrapperBase> ShaderNode::GetVec40(std::shared_ptr<SpirvCompiler> compiler) {
    auto ct = std::make_shared<SPVVecConstant<4>>();
    ct->data = {0, 0, 0, 0};
    ct->type = HLTypes::VECTOR4;

    compiler->RegisterConstant(ct);

    return compiler->GetConstantW(ct);
}

std::shared_ptr<SpirvDataWrapperBase> ShaderNode::GetFloat0(std::shared_ptr<SpirvCompiler> compiler) {
    auto ct = std::make_shared<SPVFloatConstant>();
    ct->data = 0;
    ct->type = HLTypes::FLOAT;

    compiler->RegisterConstant(ct);

    return compiler->GetConstantW(ct);
}
