#include "BD_SkelReader.h"

BD_SkelReader::BD_SkelReader() {

}

BD_SkelReader::~BD_SkelReader() {

}

bool BD_SkelReader::ReadFile(std::string file) {
    if(!std::filesystem::exists(file)) {
        std::cerr << "ERROR: File " << file << " does not exists." << std::endl;
        return false;
    }

    std::ifstream bFile(file, std::ios::binary);
    if(!bFile.is_open()) {
        std::cerr << "ERROR: Failed to open " << file << std::endl;
        return false;
    }

    size_t nBones = 0;
    bFile.read(reinterpret_cast<char*>(&nBones), sizeof(size_t));

    for(int i = 0; i < nBones; i++) {
        Bone b;

        bFile.read(reinterpret_cast<char*>(&b.idx), sizeof(int));
        bFile.read(reinterpret_cast<char*>(&b.pos), sizeof(mat4f));
        bFile.read(reinterpret_cast<char*>(&b.offset), sizeof(mat4f));

        bFile.read(reinterpret_cast<char*>(&b.parent_id), sizeof(int));

        size_t ssize = 0;
        bFile.read(reinterpret_cast<char*>(&ssize), sizeof(size_t));

        b.name.resize(ssize);
        bFile.read(b.name.data(), ssize);

        m_bones.push_back(b);
    }

    bFile.close();

    return true;
}

int BD_SkelReader::GetBoneCount() {
    return m_bones.size();
}

Bone BD_SkelReader::GetBoneByIndex(int idx) {
    assert(idx < m_bones.size());
    return m_bones[idx];
}