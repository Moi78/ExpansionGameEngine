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

SpOpStoreCtant::SpOpStoreCtant(std::shared_ptr<SpirvDataWrapperBase> targ, std::shared_ptr<SpirvDataWrapperBase> ct) {
    target = targ;
    ctant = ct;

    LoadOp(62, 3);
}

void SpOpStoreCtant::PreCompile(std::unordered_map<HLTypes, std::shared_ptr<SPVType>> realTypes) {
    assert((target->GetReflectedType() & 0xFF) == ctant->GetReflectedType() && "Constant and variable type must match");

    words = {
            (uint32_t)target->GetReflectedID(),
            (uint32_t)ctant->GetReflectedID()
    };
}

// ---------------------------------------------------------------------

SpOpAccessChain::SpOpAccessChain(std::shared_ptr<SpirvVariable> targ, std::shared_ptr<SpirvDataWrapperBase> idx, uint32_t rid) {
    target = targ;
    vecIdx = idx;
    resID = rid;

    LoadOp(65, 5);
}

void SpOpAccessChain::PreCompile(std::unordered_map<HLTypes, std::shared_ptr<SPVType>> realTypes) {
    uint32_t tid = 0;
    if(stcl == StorageClass::Output) {
        tid = realTypes[HLTypes::FLOATPTRO]->m_id;
    } else if(stcl == StorageClass::Function) {
        tid =realTypes[HLTypes::FLOATPTR]->m_id;
    }

    words = {
            tid,
            resID,
            (uint32_t)target->id,
            vecIdx->GetReflectedID()
    };
}

// ---------------------------------------------------------------------

uint32_t SpirvDataWrapperAccessChain::GetReflectedID() {
    return ptr->resID;
}

HLTypes SpirvDataWrapperAccessChain::GetReflectedType() {
    if(ptr->stcl == StorageClass::Function) {
        return HLTypes::FLOATPTR;
    } else if(ptr->stcl == StorageClass::Output) {
        return HLTypes::FLOATPTRO;
    }

    return HLTypes::FLOATPTRO;
}

// ---------------------------------------------------------------------

SpOpCompExtract::SpOpCompExtract(std::shared_ptr<SpirvDataWrapperBase> src, int idx, uint32_t rid) {
    source = src;
    vecIdx = idx;
    resId = rid;

    LoadOp(81, 5);
}

void SpOpCompExtract::PreCompile(std::unordered_map<HLTypes, std::shared_ptr<SPVType>> realTypes) {
    uint32_t tid = realTypes[HLTypes::FLOAT]->m_id;

    words = {
            tid,
            resId,
            source->GetReflectedID(),
            (uint32_t)vecIdx
    };
}

// ---------------------------------------------------------------------

uint32_t SpirvDataWrapperCompExtracted::GetReflectedID() {
    return ptr->resId;
}

HLTypes SpirvDataWrapperCompExtracted::GetReflectedType() {
    return HLTypes::FLOAT;
}
