#include "BD_SkelWriter.h"

BD_SkelWriter::BD_SkelWriter() {

}

BD_SkelWriter::~BD_SkelWriter() {

}

void BD_SkelWriter::AppendBone(Bone b) {
    m_bones.push_back(b);
}

void BD_SkelWriter::WriteBinary(std::string outpath) {
    std::ofstream file = std::ofstream(outpath.c_str(), std::ios::binary);
    if(!file.is_open()) {
        std::cerr << "ERROR: Failed to write file " << outpath << std::endl;
        return;
    }

    size_t arr_size = m_bones.size();
    file.write(reinterpret_cast<const char*>(&arr_size), sizeof(size_t));
    for(auto& b : m_bones) {
        file.write(reinterpret_cast<const char*>(&b.idx), sizeof(int));
        file.write(reinterpret_cast<const char*>(&b.pos), sizeof(mat4f));
        file.write(reinterpret_cast<const char*>(&b.offset), sizeof(mat4f));

        file.write(reinterpret_cast<const char*>(&b.parent_id), sizeof(int));

        size_t size = b.name.size();
        file.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
        file.write(b.name.c_str(), size);
    }

    file.close();
}
