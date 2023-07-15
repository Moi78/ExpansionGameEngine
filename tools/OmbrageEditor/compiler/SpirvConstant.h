#ifndef EXPGE_SPIRVCONSTANT_H
#define EXPGE_SPIRVCONSTANT_H

#include <array>

#include "SpirvData.h"
#include "SpirvOperation.h"

struct SPVFloatConstant : public SpirvConstant {
    float data;

    std::vector<uint32_t> DeclConstant(std::unordered_map<HLTypes, std::shared_ptr<SPVType>> realTypes, int& idcounter) override;
};

struct SPVIntConstant : public SpirvConstant {
    int data;

    std::vector<uint32_t> DeclConstant(std::unordered_map<HLTypes, std::shared_ptr<SPVType>> realTypes, int& idcounter) override;
};

template<int size_>
struct SPVVecConstant : public SpirvConstant {
    std::array<float, size_> data;

    std::vector<uint32_t> DeclConstant(std::unordered_map<HLTypes, std::shared_ptr<SPVType>> realTypes, int& idcounter) override {
        static_assert((size_ <= 4) && (size_ >= 2) && "Vector size must be between 2 and 4");

        std::vector<uint32_t> decl;
        std::vector<int> ids;
        for(auto& d : data) {
            SPVFloatConstant ctant;
            ctant.data = d;
            ctant.id = idcounter;
            ids.push_back(idcounter);

            auto ct = ctant.DeclConstant(realTypes, idcounter);
            decl.insert(decl.end(), ct.begin(), ct.end());

            idcounter++;
        }

        HLTypes t = HLTypes::VECTOR2;
        if(size_ == 3) {
            t = HLTypes::VECTOR3;
        } else if(size_ == 4) {
            t = HLTypes::VECTOR4;
        }

        SpirvOperation OpCtantComposite{};
        OpCtantComposite.LoadOp(44, 3 + size_);
        OpCtantComposite.words = {
                (uint32_t)realTypes[t]->m_id,
                (uint32_t)id,
        };
        idcounter++;

        OpCtantComposite.words.insert(OpCtantComposite.words.end(), ids.begin(), ids.end());
        auto compoDecl = OpCtantComposite.GetData();
        decl.insert(decl.end(), compoDecl.begin(), compoDecl.end());

        return decl;
    }
};

bool CtantCompare(std::shared_ptr<SpirvConstant> a, std::shared_ptr<SpirvConstant> b);

#endif //EXPGE_SPIRVCONSTANT_H
