#include "SpirvConstant.h"

std::vector<uint32_t> SPVFloatConstant::DeclConstant(std::unordered_map<HLTypes, std::shared_ptr<SPVType>> realTypes, int& idcounter) {
    uint32_t converted = 0;
    memcpy(&converted, &data, sizeof(float));

    uint32_t tID = realTypes[HLTypes::FLOAT]->m_id;

    SpirvOperation OpConstant{};
    OpConstant.LoadOp(43, 4);
    OpConstant.words = {
            tID,
            (uint32_t)id,
            converted
    };

    return OpConstant.GetData();
}

std::vector<uint32_t> SPVIntConstant::DeclConstant(std::unordered_map<HLTypes, std::shared_ptr<SPVType>> realTypes, int& idcounter) {
    uint32_t converted = data;

    uint32_t tID = realTypes[HLTypes::INT]->m_id;

    SpirvOperation OpConstant{};
    OpConstant.LoadOp(43, 4);
    OpConstant.words = {
            tID,
            (uint32_t)id,
            converted
    };

    return OpConstant.GetData();
}

bool CtantCompare(std::shared_ptr<SpirvConstant> a, std::shared_ptr<SpirvConstant> b) {
    bool isSamePtr = a == b;

    bool isSameValue = false;

    bool isSameType = a->type == b->type;
    if(isSameType) {
        if(a->type == HLTypes::FLOAT) {
            auto castedA = std::reinterpret_pointer_cast<SPVFloatConstant>(a);
            auto castedB = std::reinterpret_pointer_cast<SPVFloatConstant>(b);

            isSameValue = castedA->data == castedB->data;
        } else if(a->type == HLTypes::INT) {
            auto castedA = std::reinterpret_pointer_cast<SPVIntConstant>(a);
            auto castedB = std::reinterpret_pointer_cast<SPVIntConstant>(b);

            isSameValue = castedA->data == castedB->data;
        } else if(a->type == HLTypes::VECTOR2) {
            auto castedA = std::reinterpret_pointer_cast<SPVVecConstant<2>>(a);
            auto castedB = std::reinterpret_pointer_cast<SPVVecConstant<2>>(b);

            isSameValue = castedA->data == castedB->data;
        } else if(a->type == HLTypes::VECTOR3) {
            auto castedA = std::reinterpret_pointer_cast<SPVVecConstant<3>>(a);
            auto castedB = std::reinterpret_pointer_cast<SPVVecConstant<3>>(b);

            isSameValue = castedA->data == castedB->data;
        } else if(a->type == HLTypes::VECTOR4) {
            auto castedA = std::reinterpret_pointer_cast<SPVVecConstant<3>>(a);
            auto castedB = std::reinterpret_pointer_cast<SPVVecConstant<3>>(b);

            isSameValue = castedA->data == castedB->data;
        }
    }

    return isSamePtr || isSameValue;
}
