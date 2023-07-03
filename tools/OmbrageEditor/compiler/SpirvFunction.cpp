#include <iostream>
#include "SpirvFunction.h"

std::vector<uint32_t> SpirvOperation::GetData() {
    std::vector<uint32_t> vec(words);
    vec.insert(vec.begin(), op);

    return vec;
}

std::vector<uint32_t> SpirvFunction::CompileFunction(std::unordered_map<HLTypes, std::shared_ptr<SPVType>> realTypes, int& nextID) {
    // Type check
    assert(realTypes.find(returnType) != realTypes.end() && "This type was not constructed");
    for(auto& t : argsType) { assert(realTypes.find(t) != realTypes.end() && "This type was not constructed"); }

    uint32_t rtypeID = realTypes[returnType]->m_id;

    std::cout << "FUNC ID : " << funcID << std::endl;

    SpirvOperation OpFunctionType{};
    OpFunctionType.LoadOp(33, 3 + argsType.size());
    OpFunctionType.words = {
            (uint32_t)(funcID + 1),                         // Type function ID
            (uint32_t)rtypeID                               // Return type ID
    };

    // Params type IDs
    for(auto& t : argsType) {
        uint32_t tp = realTypes[t]->m_id;
        OpFunctionType.words.push_back(tp);
    }

    SpirvOperation OpFunction{};
    OpFunction.LoadOp(54, 5);
    OpFunction.words = {
            (uint32_t)rtypeID,                              // Return Type ID
            (uint32_t)funcID,                               // Function ID
            0,                                              // NONE
            (uint32_t)(funcID + 1)                          // Type function ID
    };

    nextID += 2; // function id + type id

    // Treating parameters

    int paramOffset = nextID + 1; // Param 0 is at paramOffset + 0, Param 1 is at paramOffset + 1, etc...
    // Every other labels will be at paramOffset + numberOfParams + x

    std::vector<SpirvOperation> params;
    for(auto& t : argsType) {
        SpirvOperation OpFunctionParam{};
        OpFunctionParam.LoadOp(55, 3);
        OpFunctionParam.words = {
                (uint32_t)realTypes[t]->m_id,               // Param type ID
                uint32_t (nextID + 1)                       // Param ID
        };

        nextID++;
    }

    SpirvOperation OpLabel{};
    OpLabel.LoadOp(248, 2);
    OpLabel.words = { (uint32_t)(nextID + 1) };

    nextID++;

    // Replacing placeholder with real param id
    std::vector<SpirvOperation> offsetedListing;
    for(auto& op : funcBody) {
        if(op.result_id.has_value()) {
            for(auto& inst : op.words) {
                if(inst == ID_PLACEHOLDER) {
                    inst = paramOffset + op.result_id.value();
                }
            }
        } else {
            offsetedListing.push_back(op);
        }
    }

    SpirvOperation OpFunctionEnd{};
    OpFunctionEnd.LoadOp(56, 1);

    nextID += funcSize;

    std::vector<uint32_t> funcData;

    auto optype = OpFunctionType.GetData();
    funcData.insert(funcData.end(), optype.begin(), optype.end());

    auto opfunc = OpFunction.GetData();
    funcData.insert(funcData.end(), opfunc.begin(), opfunc.end());

    for(auto& p : params) {
        auto opparam = p.GetData();
        funcData.insert(funcData.end(), opparam.begin(), opparam.end());
    }

    auto oplabel = OpLabel.GetData();
    funcData.insert(funcData.end(), oplabel.begin(), oplabel.end());

    for(auto& i : offsetedListing) {
        auto data = i.GetData();
        funcData.insert(funcData.end(), data.begin(), data.end());
    }

    auto opend = OpFunctionEnd.GetData();
    funcData.insert(funcData.end(), opend.begin(), opend.end());

    return funcData;
}