#include "SpirvSpecialOP.h"
#include "SpirvOperation.h"

SpOpFunCall::SpOpFunCall(std::shared_ptr<SpirvFunction> fn, std::vector<std::shared_ptr<SpirvDataWrapperBase>> arg, bool nStore)
: SpirvOperation(), func(fn), args(arg), noStore(nStore)
{
    LoadOp(57, 4 + fn->argsType.size());
}

void SpOpFunCall::PreCompile(std::unordered_map<HLTypes, std::shared_ptr<SPVType>> realTypes) {
    assert(args.size() == func->argsType.size() && "Missing arguments");

    uint32_t rID = realTypes[func->returnType]->m_id;

    words = {
            rID,
            (uint32_t)id,
            (uint32_t)func->funcID
    };

    for(auto& a : args) {
        words.push_back((uint32_t)a->GetReflectedID());
    }

    // Appending store op
    if(!noStore) {
        SpirvOperation OpStore{};
        OpStore.LoadOp(62, 3);
        OpStore.words = {
                (uint32_t) target->id,
                (uint32_t) id
        };

        auto store = OpStore.GetData();
        words.insert(words.end(), store.begin(), store.end());
    }
}

uint32_t SpirvDataWrapperFunRet::GetReflectedID() {
    return fcall->id;
}

HLTypes SpirvDataWrapperFunRet::GetReflectedType() {
    return fcall->func->returnType;
}

// ---------------------------------------------------------------------

SpOpStoreCtant::SpOpStoreCtant(std::shared_ptr<SpirvVariable> targ, std::shared_ptr<SpirvConstant> ct) {
    target = targ;
    ctant = ct;

    LoadOp(62, 3);
}

void SpOpStoreCtant::PreCompile(std::unordered_map<HLTypes, std::shared_ptr<SPVType>> realTypes) {
    assert((target->type & 0xFF) == ctant->type && "Constant and variable type must match");

    words = {
            (uint32_t)target->id,
            (uint32_t)ctant->id
    };
}