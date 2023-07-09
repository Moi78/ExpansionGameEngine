#include <iostream>
#include <fstream>
#include <filesystem>
#include <unordered_map>

#include <jsoncpp/json/json.h>

#include "SpirvTypes.h"

void showUsage() {
    std::cout << "Usage : spvextract in_file function" << std::endl;
    std::cout << "in_file : Binary SPIR-V file to extract a function from." << std::endl;
    std::cout << "function: Name of the function to extract." << std::endl;
}

std::vector<uint32_t> GetFunction(std::string funcName, std::vector<uint32_t> programData) {
    return {};
}

constexpr int GetOpCode(const uint32_t& in) {
    return (in & 0xFFFF);
}

constexpr int GetOpSize(const uint32_t& in) {
    return (in & 0xFFFF0000) >> 16;
}

HLTypes OpToType(std::vector<uint32_t> op, std::unordered_map<int, HLTypes> known_types) {
    HLTypes t;
    switch(GetOpCode(op[0])) {
        case 19:
            t = HLTypes::VOID;
            break;
        case 21:
            t = HLTypes::INT;
            break;
        case 22:
            t = HLTypes::FLOAT;
            break;
        case 23:
            if(op[2] == 2) {
                t = HLTypes::VECTOR2;
            } else if(op[2] == 3) {
                t = HLTypes::VECTOR3;
            } else if(op[2] == 4) {
                t = HLTypes::VECTOR4;
            }
            break;
        case 32:
            if(known_types.find(op[3]) != known_types.end()) {
                if(op[2] == (uint32_t)StorageClass::Function) {
                    t = known_types[op[3]] | FLAG_PTR_FUNCTION;
                } else if(op[2] == (uint32_t)StorageClass::Output) {
                    t = known_types[op[3]] | FLAG_PTR_OUTPUT;
                } else {
                    throw "UNSUPPORTED STORAGE CLASS";
                }
            } else {
                throw "UNKNOWN TYPE";
            }
            break;
    }

    return t;
}

int main(int argc, char* argv[]) {
    std::cout << "SPIRV EXTRACT" << std::endl;

    if(argc < 3) {
        showUsage();
        return -1;
    }

    std::string file = argv[1];
    if(!std::filesystem::exists(file)) {
        std::cerr << "ERROR: File " << file << " does not exists." << std::endl;
        return -2;
    }

    std::string funcName = argv[2];
    std::cout << "Searching for " << funcName << std::endl;

    std::ifstream f(file, std::ios::binary | std::ios::ate);
    if(!f.is_open()) {
        std::cerr << "ERROR: Unable to open file " << file << std::endl;
        return -3;
    }

    size_t fsize = f.tellg();
    f.seekg(0);
    if(fsize % 4 != 0) {
        std::cerr << "ERORR: Bad SPIR-V format." << std::endl;
        return -4;
    }

    std::vector<uint32_t> spvData(fsize);
    f.read((char*)spvData.data(), fsize);

    int idx = 5;
    int fid = 0;

    // Find OpNames
    while(idx < ((spvData.size() / 4) - 1)) {
        if(GetOpCode(spvData[idx]) == 5) {
            uint32_t funcID = spvData[idx + 1];

            size_t wordCount = GetOpSize(spvData[idx]) - 2;
            idx += 2;

            char* dbgData = new char[wordCount * 4];
            memcpy(dbgData, &spvData[idx], wordCount * 4);

            std::string name(dbgData);

            if(name.find(funcName) != std::string::npos) {
                std::cout << "Found " << funcName << " (ID : " << (int)funcID << ")" << std::endl;
                fid = funcID;
                break;
            }

            idx += wordCount;
        } else {
            idx += GetOpSize(spvData[idx]);
        }
    }

    idx = 5;
    std::unordered_map<int, HLTypes> types_map;

    // Mapping types
    while(idx < ((spvData.size() / 4) - 1)) {
        if((GetOpCode(spvData[idx]) >= 19) && (GetOpCode(spvData[idx]) <= 32)) {
            size_t size = GetOpSize(spvData[idx]);
            std::vector<uint32_t> op(spvData.begin() + idx, spvData.begin() + idx + size);

            types_map[op[1]] = OpToType(op, types_map);

            idx += size;
        } else {
            idx += GetOpSize(spvData[idx]);
        }
    }

    // Finding func words
    std::vector<uint32_t>::iterator funcBeg, funcEnd;

    bool foundBeg = false;
    idx = 5;

    while(idx < ((spvData.size() / 4))) {
        if(GetOpCode(spvData[idx]) == 54) {
            if(spvData[idx + 2] == fid) {
                funcBeg = spvData.begin() + idx;
                foundBeg = true;
            }
        }

        auto dbg = GetOpCode(spvData[idx]);
        if(foundBeg && (GetOpCode(spvData[idx]) == 56)) {
            funcEnd = spvData.begin() + idx + 1;
            break;
        }

        idx += GetOpSize(spvData[idx]);
    }

    std::vector<uint32_t> funcBody(funcBeg, funcEnd);

    f.close();

    return 0;
}