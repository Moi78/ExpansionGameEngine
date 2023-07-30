#include "SpirvSpecialOP.h"
#include "SpirvOperation.h"

SpOpFunCall::SpOpFunCall(std::shared_ptr<SpirvFunction> fn, std::vector<std::shared_ptr<SpirvDataWrapperBase>> arg, bool nStore)
: SpirvOperation(), func(fn), args(arg), noStore(nStore)
{
    LoadOp(57, 4 + fn->argsType.size());
}

void SpOpFunCall::PreCompile(std::unordered_map<HLTypes, std::shared_ptr<SPVType>> realTypes, int &idcounter) {
    assert(args.size() == func->argsType.size() && "Missing arguments");

    uint32_t rID = realTypes[func->returnType]->m_id;

    std::vector<uint32_t> argVarID;
    for(auto& arg : args) {
        if(arg->isVarStored() && (arg->stcl != StorageClass::Input)) {
            argVarID.push_back(arg->GetReflectedID());
        } else {
            auto OpVariable = std::make_shared<SpirvOperation>();
            OpVariable->LoadOp(59, 4);

            HLTypes vartype = (arg->GetReflectedType() & 0xFF) | FLAG_PTR_FUNCTION;
            uint32_t rtype_id = realTypes[vartype]->m_id;

            OpVariable->words = {
                    rtype_id,
                    (uint32_t) idcounter,
                    (uint32_t) StorageClass::Function
            };
            argVarID.push_back(idcounter);
            preOp.push_back(OpVariable);

            idcounter++;

            std::shared_ptr<SpirvDataWrapperBase> toStore = arg;
            if(arg->stcl == StorageClass::Input) {
                auto OpLoad = std::make_shared<SpOpLoad>(arg, idcounter);
                idcounter++;

                auto loadwrap = std::make_shared<SpirvDataWrapperLoad>();
                loadwrap->ptr = OpLoad;

                toStore = loadwrap;

                OpLoad->PreCompile(realTypes, idcounter);
                preOp.push_back(OpLoad);
            }

            auto OpStore = std::make_shared<SpirvOperation>();
            OpStore->LoadOp(62, 3);
            OpStore->words = {
                    OpVariable->words[1],
                    toStore->GetReflectedID()
            };
            preOp.push_back(OpStore);

            idcounter++;
        }
    }

    words = {
            rID,
            (uint32_t)id,
            (uint32_t)func->funcID
    };

    for(auto& argID : argVarID) {
        words.push_back((uint32_t)argID);
    }

    uint32_t rtype_ptr_id = realTypes[func->returnType | FLAG_PTR_FUNCTION]->m_id;

    auto OpVariable = std::make_shared<SpirvOperation>();
    OpVariable->LoadOp(59, 4);
    OpVariable->words = {
            rtype_ptr_id,
            (uint32_t)data_id,
            (uint32_t)StorageClass::Function
    };

    auto OpStore_ret = std::make_shared<SpirvOperation>();
    OpStore_ret->LoadOp(62, 3);
    OpStore_ret->words = {
            (uint32_t) data_id,
            (uint32_t) id
    };
    auto storebin = OpStore_ret->GetData();
    auto varbin = OpVariable->GetData();

    words.insert(words.end(), varbin.begin(), varbin.end());
    words.insert(words.end(), storebin.begin(), storebin.end());

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
    return fcall->data_id;
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

void SpOpStoreCtant::PreCompile(std::unordered_map<HLTypes, std::shared_ptr<SPVType>> realTypes, int &idcounter) {
    assert((target->GetReflectedType() & 0xFF) == (ctant->GetReflectedType() & 0xFF) && "Constant and variable type must match");

    words = {
            (uint32_t)target->GetReflectedID(),
            (uint32_t)ctant->GetReflectedID()
    };
}

// ---------------------------------------------------------------------

SpOpAccessChain::SpOpAccessChain(std::shared_ptr<SpirvDataWrapperBase> targ, std::shared_ptr<SpirvDataWrapperBase> idx, uint32_t rid) {
    target = targ;
    vecIdx = idx;
    resID = rid;

    stcl = targ->stcl;

    LoadOp(65, 5);
}

void SpOpAccessChain::PreCompile(std::unordered_map<HLTypes, std::shared_ptr<SPVType>> realTypes, int &idcounter) {
    uint32_t tid = 0;
    if(stcl == StorageClass::Output) {
        tid = realTypes[HLTypes::FLOATPTRO]->m_id;
    } else if(stcl == StorageClass::Function) {
        tid = realTypes[HLTypes::FLOATPTR]->m_id;
    } else if(stcl == StorageClass::Input) {
        tid = realTypes[HLTypes::FLOATPTRI]->m_id;
    }

    words = {
            tid,
            resID,
            (uint32_t)target->GetReflectedID(),
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
    } else if(ptr->stcl == StorageClass::Input) {
        return HLTypes::FLOATPTRI;
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

void SpOpCompExtract::PreCompile(std::unordered_map<HLTypes, std::shared_ptr<SPVType>> realTypes, int &idcounter) {
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

// ---------------------------------------------------------------------

SpOpLoad::SpOpLoad(std::shared_ptr<SpirvDataWrapperBase> src, int rid) {
    ptr = src;
    resID = rid;
    std::cout << "LOAD RES ID IS " << rid << std::endl;

    LoadOp(61, 4);
}

void SpOpLoad::PreCompile(std::unordered_map<HLTypes, std::shared_ptr<SPVType>> realTypes, int &idcounter) {
    type = ptr->GetReflectedType() & 0xFF;
    uint32_t tid = realTypes[type]->m_id;

    words = {
            tid,
            resID,
            ptr->GetReflectedID(),
    };
}

HLTypes SpirvDataWrapperLoad::GetReflectedType() {
    return ptr->type;
}

uint32_t SpirvDataWrapperLoad::GetReflectedID() {
    return ptr->resID;
}
