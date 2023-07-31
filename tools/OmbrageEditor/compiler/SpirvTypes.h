#ifndef EXPGE_SPIRVTYPES_H
#define EXPGE_SPIRVTYPES_H

#include <vector>
#include <cstdint>
#include <memory>
#include <optional>
#include <cassert>
#include <iostream>

#define OPTYPEPTR 32

#define FLAG_PTR_FUNCTION 0x4000000
#define FLAG_PTR_OUTPUT 0x8000000
#define FLAG_PTR_INPUT 0x1000000
#define FLAG_PTR_UNIFORM_CTANT 0x0800000

#define FLAG_IS_VECTOR 0x80

#define FLAG_IS_NOT_TYPE 0x20000000

enum class HLTypes {
    // Primary types
    VOID = 0,
    FLOAT = 1,
    INT = 2,
    VECTOR2 = 3 | FLAG_IS_VECTOR,
    VECTOR3 = 4 | FLAG_IS_VECTOR,
    VECTOR4 = 5 | FLAG_IS_VECTOR,
    IMAGE = 6,
    SAMPLED_IMAGE = 7,

    // Function type pointers
    FLOATPTR = 1 | FLAG_PTR_FUNCTION,
    INTPTR = 2 | FLAG_PTR_FUNCTION,
    VECTOR2PTR = 3 | FLAG_IS_VECTOR | FLAG_PTR_FUNCTION,
    VECTOR3PTR = 4 | FLAG_IS_VECTOR | FLAG_PTR_FUNCTION,
    VECTOR4PTR = 5 | FLAG_IS_VECTOR | FLAG_PTR_FUNCTION,

    // Output type pointers
    FLOATPTRO = 1 | FLAG_PTR_OUTPUT,
    INTPTRO = 2 | FLAG_PTR_OUTPUT,
    VECTOR2PTRO = 3 | FLAG_IS_VECTOR | FLAG_PTR_OUTPUT,
    VECTOR3PTRO = 4 | FLAG_IS_VECTOR | FLAG_PTR_OUTPUT,
    VECTOR4PTRO = 5 | FLAG_IS_VECTOR | FLAG_PTR_OUTPUT,

    // Input type pointers
    FLOATPTRI = 1 | FLAG_PTR_INPUT,
    INTPTRI = 2 | FLAG_PTR_INPUT,
    VECTOR2PTRI = 3 | FLAG_IS_VECTOR | FLAG_PTR_INPUT,
    VECTOR3PTRI = 4 | FLAG_IS_VECTOR | FLAG_PTR_INPUT,
    VECTOR4PTRI = 5 | FLAG_IS_VECTOR | FLAG_PTR_INPUT,

    // Uniform Constant pointers
    SAMPLED_IMAGE_PTRUC = 7 | FLAG_PTR_UNIFORM_CTANT
};

HLTypes operator&(const HLTypes& a, const int& b);
HLTypes operator|(const HLTypes& a, const int& b);

enum class StorageClass {
    Uniform = 2,
    Output = 3,
    Function = 7,
    Input = 1
};

class SPVType {
public:
    virtual unsigned GetOpcode() = 0;

    virtual std::vector<uint32_t> GetTypeDecl() = 0;
    std::vector<uint32_t> GetTypePtrDecl(std::shared_ptr<SPVType> baseType);

    int m_id;

    bool m_isPointer = false;
    std::optional<StorageClass> m_ptrAttr;
};

class TVoid : public SPVType {
public:
    TVoid(int id) { m_id = id; }

    virtual unsigned GetOpcode() override { return 19; }

    virtual std::vector<uint32_t> GetTypeDecl() override;
};

class TFloat : public SPVType {
public:
    TFloat(int id) { m_id = id; }

    virtual unsigned GetOpcode() override { return 22; }
    virtual std::vector<uint32_t> GetTypeDecl() override;
};

class TInt : public SPVType {
public:
    TInt(int id) { m_id = id; }

    virtual unsigned GetOpcode() override { return 21; }
    virtual std::vector<uint32_t> GetTypeDecl() override;
};

template<int _size>
class TVector : public SPVType {
public:
    TVector(int id, std::shared_ptr<SPVType> primType) { m_id = id; m_primType = primType; }

    virtual unsigned GetOpcode() override { return 23; }

    virtual std::vector<uint32_t> GetTypeDecl() override {
        uint32_t firstWord = (4 << 16) | GetOpcode();

        return {firstWord, (uint32_t)m_id, (uint32_t)m_primType->m_id, _size};
    }

private:
    std::shared_ptr<SPVType> m_primType;
};

class TImage : public SPVType {
public:
    TImage(int id) { m_id = id; }

    virtual unsigned GetOpcode() override { return 25; }

    virtual std::vector<uint32_t> GetTypeDecl() override;
};

#endif //EXPGE_SPIRVTYPES_H
