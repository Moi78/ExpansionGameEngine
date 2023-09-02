#include "FunctionDeserializer.h"

std::shared_ptr<SpirvFunction> FunctionDeserializer::Deserialize(std::string path) {
    std::ifstream f(path, std::ios::binary);
    if(!f.is_open()) {
        std::cerr << "ERROR: Failed to open " << path << std::endl;
        return {};
    }

    auto func = std::make_shared<SpirvFunction>();

    // Func name
    size_t namesize = 0;
    f.read((char*)&namesize, sizeof(namesize));

    char* cname = new char[namesize + 1];

    f.read(cname, namesize);
    cname[namesize] = '\0';

    func->funcName = std::move(cname);

    // Return type
    f.read((char*)&func->returnType, sizeof(HLTypes));

    // Params
    uint8_t paramCount = 0;
    f.read((char*)&paramCount, sizeof(uint8_t));

    for(int i = 0; i < paramCount; i++) {
        HLTypes t = HLTypes::VOID;
        f.read((char*)&t, sizeof(HLTypes));

        func->argsType.push_back(t);
    }

    // Func Size
    f.read((char*)&func->funcSize, sizeof(int));

    // Func type deps
    size_t tCount = 0;
    f.read((char*)&tCount, sizeof(size_t));

    func->typeDeps.resize(tCount);
    f.read((char*)func->typeDeps.data(), tCount * sizeof(HLTypes));

    // Func ctant deps
    size_t cCount = 0;
    f.read((char*)&cCount, sizeof(size_t));
    func->ctantDeps.resize(cCount);

    for(int i = 0; i < cCount; i++) {
        f.read((char*)&func->ctantDeps[i].first, sizeof(uint32_t));
        f.read((char*)&func->ctantDeps[i].second, sizeof(SimpleCtant));
    }

    // Func body
    size_t opCount = 0;
    f.read((char*)&opCount, sizeof(size_t));
    for(int i = 0; i < opCount; i++) {
        std::shared_ptr<SpirvOperation> des_op = DeserializeOp(f);
        func->funcBody.push_back(des_op);
    }

    f.close();

    return func;
}

std::shared_ptr<SpirvOperation> FunctionDeserializer::DeserializeOp(std::ifstream &file) {
    auto loaded_op = std::make_shared<SpirvOperation>();

    file.read((char*)&loaded_op->op, sizeof(uint32_t));

    uint32_t replcount = 0;
    file.read((char*)&replcount, sizeof(uint32_t));

     for(int i = 0; i < replcount; i++) {
         uint32_t repl_tmp = 0;
         file.read((char*)&repl_tmp, sizeof(uint32_t));

        loaded_op->id_repl.push_back(repl_tmp);
     }

    uint32_t hasResID = false;
    file.read((char*)&hasResID, sizeof(uint32_t));

    if(hasResID) {
        uint32_t resID = 0;
        file.read((char*)&resID, sizeof(uint32_t));
        loaded_op->result_id = resID;
    }

    int wordCount = ((loaded_op->op & 0xFFFF0000) >> 16) - 1;
    for(int i = 0; i < wordCount; i++) {
        uint32_t word = 0;
        file.read((char*)&word, sizeof(uint32_t));

        loaded_op->words.push_back(word);
    }

    return loaded_op;
}
