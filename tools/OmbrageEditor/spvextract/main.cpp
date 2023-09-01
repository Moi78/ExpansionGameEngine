#include <iostream>
#include <fstream>
#include <filesystem>
#include <unordered_map>

#include <jsoncpp/json/json.h>

#include "SpirvTypes.h"
#include "SpirvFunction.h"
#include "SpirvProgram.h"
#include "SpirvSpecialOP.h"
#include "FunctionSerializer.h"

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
            if(op[3] == 2) {
                t = HLTypes::VECTOR2;
            } else if(op[3] == 3) {
                t = HLTypes::VECTOR3;
            } else if(op[3] == 4) {
                t = HLTypes::VECTOR4;
            }
            break;
        case 25:
            t = HLTypes::IMAGE;
            break;
        case 27:
            t = HLTypes::SAMPLED_IMAGE;
            break;
        case 24:
            if(op[3] == 3) {
                t = HLTypes::MAT3;
            } else {
                throw "UNKOWN TYPE";
            }
            break;
        case 32:
            if(known_types.find(op[3]) != known_types.end()) {
                if(op[2] == (uint32_t)StorageClass::Function) {
                    t = known_types[op[3]] | FLAG_PTR_FUNCTION;
                } else if(op[2] == (uint32_t)StorageClass::Output) {
                    t = known_types[op[3]] | FLAG_PTR_OUTPUT;
                } else if(op[2] == (uint32_t)StorageClass::UniformConstant) {
                    t = known_types[op[3]] | FLAG_PTR_UNIFORM_CTANT;
                } else {
                    throw "UNSUPPORTED STORAGE CLASS";
                }
            } else {
                throw "UNKNOWN TYPE";
            }
            break;
        default:
            throw "UNKOWN TYPE";
            break;
    }

    return t;
}

Json::Value FindOp(Json::Value instrRoot, int opcode) {
    for(auto& i : instrRoot) {
        if(opcode == i["opcode"].asInt()) {
            return i;
        }
    }

    return {};
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

    std::string output = "out.func";
    if(argc > 3) {
        output = std::string(argv[3]);
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

    if(!fid) {
        std::cerr << "ERROR: Function " << funcName << " not found." << std::endl;
        return -6;
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
    std::shared_ptr<SpirvFunction> func = std::make_shared<SpirvFunction>();
    func->funcName = funcName;

    int idcounter = 0;
    std::unordered_map<int, int> local_id_map;
    std::vector<uint32_t> repl;

    std::string binDir = std::filesystem::path(std::string(argv[0])).parent_path();
    std::ifstream spvJson(binDir + "/third_party/spirv.core.grammar.json");

    Json::Value root;
    Json::String err;

    Json::CharReaderBuilder builder;
    if(!parseFromStream(builder, spvJson, &root, &err)) {
        std::cerr << "ERROR: Failed to parse spirv core grammar." << std::endl;
        std::cerr << err << std::endl;

        return -5;
    }

    Json::Value instrs = root["instructions"];

    int dbgLoopCount = 0;
    for(int i = 0; i < funcBody.size(); i += GetOpSize(funcBody[i])) {
        auto dbg = GetOpCode(funcBody[i]);
        if(dbgLoopCount == 28) {
            std::cout << "bp" << std::endl;
        }
        std::string dbg2 = FindOp(instrs, dbg)["opname"].asString();

        if(GetOpCode(funcBody[i]) == 54) {              // Func decl
            std::cout << "FUNC RTYPE : " << (int)types_map[funcBody[i + 1]] << std::endl;
            func->returnType = types_map[funcBody[i + 1]];

        } else if(GetOpCode(funcBody[i]) == 55) {       // Func params
            std::cout << "FUNC PARAM" << idcounter << " -> " << funcBody[i + 2] << std::endl;
            func->argsType.push_back(types_map[funcBody[i + 1]]);

            local_id_map[funcBody[i + 2]] = idcounter;
            idcounter++;
        } else if(GetOpCode(funcBody[i]) == 56) {       // Func end
            break;
        } else if(GetOpCode(funcBody[i]) == 248) {      // OpLabel ignore
            continue;
        } else {                                        // Other ops
            auto op = FindOp(instrs, GetOpCode(funcBody[i]));
            auto spvOp = std::make_shared<SpirvOperation>();
            spvOp->op = funcBody[i];

            if(op.empty()) {
                std::cout << "Failed to fetch op" << std::endl;
            } else {
                Json::Value operands = op["operands"];
                int count = 0;

                for(auto& o : operands) {
                    uint32_t word = ID_PLACEHOLDER;

                    if(o["kind"] == "IdResultType") {
                        spvOp->id_repl.push_back((int)types_map[funcBody[i + count + 1]]);
                        spvOp->words.push_back(word);
                        count++;
                    } else if(o["kind"] == "IdResult") {
                        local_id_map[funcBody[i + count + 1]] = idcounter;
                        spvOp->result_id = idcounter;
                        spvOp->id_repl.push_back(-1);
                        spvOp->words.push_back(word);

                        std::cout << funcBody[i + count + 1] << " -> " << idcounter << std::endl;

                        idcounter++;
                        count++;
                    } else if(o["kind"] == "IdRef") {
                        int nbIDs = 1;
                        if (o.get("quantifier", "") == "*") {
                            nbIDs = GetOpSize(funcBody[i]) - count - 1;
                        }

                        for (int a = 0; a < nbIDs; a++) {
                            if (local_id_map.find(funcBody[i + count + 1]) == local_id_map.end()) {
                                local_id_map[funcBody[i + count + 1]] = idcounter;
                                spvOp->id_repl.push_back(idcounter | FLAG_IS_NOT_TYPE);

                                std::cout << funcBody[i + count + 1] << " -> " << idcounter << std::endl;

                                idcounter++;
                            } else {
                                std::cout << funcBody[i + count + 1] << " -> " << local_id_map[funcBody[i + count + 1]]
                                          << std::endl;
                                spvOp->id_repl.push_back(local_id_map[funcBody[i + count + 1]] | FLAG_IS_NOT_TYPE);
                            }

                            spvOp->words.push_back(word);
                            count++;
                        }
                    } else if(o["kind"].asString().find("Literal") != std::string::npos) {
                        int remainingWords = GetOpSize(funcBody[i]) - count - 1;
                        for(int a = 0; a < remainingWords; a++) {
                            word = funcBody[i + count + a + 1];
                            spvOp->words.push_back(word);
                        }
                    } else {
                        word = funcBody[i + count + 1];
                        spvOp->words.push_back(word);

                        count++;
                    }

                    if(count == (GetOpSize(funcBody[i]) - 1)) {
                        break;
                    }
                }

                func->funcBody.push_back(spvOp);
                dbgLoopCount++;
            }
        }
    }
    func->funcSize = idcounter;

    f.close();
    spvJson.close();

    FunctionSerializer ser{};
    ser.Serialize(func, output);

    return 0;
}