#include "FunctionSerializer.h"

bool FunctionSerializer::Serialize(std::shared_ptr<SpirvFunction> func, std::string outpath) {
    std::ofstream f(outpath, std::ios::binary);
    if(!f.is_open()) {
        std::cerr << "ERROR: Failed to create " << outpath << std::endl;
        return false;
    }

    // Func name
    size_t str_size = func->funcName.size();
    f.write((char*)&str_size, sizeof(size_t));
    f.write(func->funcName.c_str(), str_size);

    // Return type
    f.write((char*)&func->returnType, sizeof(HLTypes));

    // Params
    uint8_t paramNumber = func->argsType.size();
    f.write((char*)&paramNumber, sizeof(uint8_t));

    for(auto& p : func->argsType) {
        f.write((char*)&p, sizeof(HLTypes));
    }

    // Func size
    f.write((char*)&func->funcSize, sizeof(int));

    // Func body
    size_t opCount = func->funcBody.size();
    f.write((char*)&opCount, sizeof(size_t));

    for(auto& b : func->funcBody) {
        auto serialized = SerializeOp(b);
        f.write(serialized.data(), serialized.size());
    }

    f.close();

    return true;
}

std::vector<char> FunctionSerializer::SerializeOp(std::shared_ptr<SpirvOperation> op) {
    std::vector<uint32_t> out;
    out.push_back(op->op);

    out.push_back(op->id_repl.size());
    out.insert(out.end(), op->id_repl.begin(), op->id_repl.end());

    out.push_back(op->result_id.has_value());
    if(op->result_id.has_value()) {
        out.push_back(op->result_id.value());
    }

    out.insert(out.end(), op->words.begin(), op->words.end());

    std::vector<char> toChar(out.size() * sizeof(uint32_t));
    memcpy(toChar.data(), out.data(), out.size() * sizeof(uint32_t));

    return toChar;
}
