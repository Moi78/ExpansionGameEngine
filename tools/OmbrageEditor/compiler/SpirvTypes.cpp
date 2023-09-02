#include "SpirvTypes.h"

HLTypes operator&(const HLTypes& a, const int& b) {
    uint32_t casted_a = static_cast<uint32_t>(a);

    return static_cast<HLTypes>(casted_a & b);
}

HLTypes operator|(const HLTypes& a, const int& b) {
    uint32_t casted_a = static_cast<uint32_t>(a);

    return static_cast<HLTypes>(casted_a | b);
}

std::vector<uint32_t> SPVType::GetTypePtrDecl(std::shared_ptr<SPVType> baseType) {
    assert(m_isPointer && m_ptrAttr.has_value() && "Invalid pointer type");

    uint32_t firstWord = (4 << 16) | OPTYPEPTR;

    return {firstWord, (uint32_t)m_id, (uint32_t)m_ptrAttr.value(), (uint32_t)baseType->m_id};
}

std::vector<uint32_t> TVoid::GetTypeDecl() {
    uint32_t firstWord = (2 << 16) | GetOpcode();

    return {firstWord, (uint32_t)m_id};
}

std::vector<uint32_t> TFloat::GetTypeDecl() {
    uint32_t firstWord = (3 << 16) | GetOpcode();

    return {firstWord, (uint32_t)m_id, 32};
}

std::vector<uint32_t> TInt::GetTypeDecl() {
    uint32_t firstWord = (4 << 16) | GetOpcode();

    return {firstWord, (uint32_t)m_id, 32, 1};
}

std::vector<uint32_t> TImage::GetTypeDecl() {
    uint32_t firstword = (9 << 16) | GetOpcode();

    return {firstword, (uint32_t)m_id, (uint32_t)m_4v->m_id, 0x01, 0, 0, 0, 1, 0};
}

std::vector<uint32_t> TSampledImage::GetTypeDecl() {
    uint32_t firstword = (3 << 16) | GetOpcode();

    return {firstword, (uint32_t)m_id, (uint32_t)m_img->m_id};
}

std::vector<uint32_t> TMat3::GetTypeDecl() {
    uint32_t firstword = (4 << 16) | GetOpcode();

    return {firstword, (uint32_t)m_id, (uint32_t)m_tvector3->m_id, 3};
}