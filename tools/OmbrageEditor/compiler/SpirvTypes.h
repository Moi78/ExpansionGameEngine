#ifndef EXPGE_SPIRVTYPES_H
#define EXPGE_SPIRVTYPES_H

#include <vector>
#include <cstdint>
#include <memory>
#include <optional>
#include <cassert>
#include <iostream>

#define OPTYPEPTR 32

#define FLAG_PTR_FUNCTION 0x4000
#define FLAG_PTR_OUTPUT 0x8000

enum class HLTypes {
    // Primary types
    VOID = 0,
    FLOAT = 1,
    INT = 1 << 2,
    VECTOR2 = 1 << 3,
    VECTOR3 = 1 << 4,
    VECTOR4 = 1 << 5,

    // Function type pointers
    FLOATPTR = (1 << 1) | 0x4000,
    INTPTR = (1 << 2) | 0x4000,
    VECTOR2PTR = (1 << 3) | 0x4000,
    VECTOR3PTR = (1 << 4) | 0x4000,
    VECTOR4PTR = (1 << 5) | 0x4000,

    // Output type pointers
    FLOATPTRO = (1 << 1) | 0x8000,
    INTPTRO = (1 << 2) | 0x8000,
    VECTOR2PTRO = (1 << 3) | 0x8000,
    VECTOR3PTRO = (1 << 4) | 0x8000,
    VECTOR4PTRO = (1 << 5) | 0x8000,
};

HLTypes operator&(const HLTypes& a, const int& b);

enum class StorageClass {
    Uniform = 2,
    Output = 3,
    Function = 7
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
    TVoid(int id) { m_id = id; std::cout << "TYPE ID " << id << std::endl; }

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
        uint32_t firstWord = (GetOpcode() << 16) | 4;

        return {firstWord, (uint32_t)m_id, (uint32_t)m_primType->m_id};
    }

private:
    std::shared_ptr<SPVType> m_primType;
};

#endif //EXPGE_SPIRVTYPES_H
