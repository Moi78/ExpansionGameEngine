#include <cassert>
#include "BD_AnimReader.h"

BD_AnimReader::BD_AnimReader() {

}

BD_AnimReader::~BD_AnimReader() {

}

bool BD_AnimReader::ReadFile(std::string file) {
    std::ifstream bin(file, std::ios::binary);
    if(!bin.is_open()) {
        std::cerr << "ERROR: Failed to read " << file << std::endl;
        return false;
    }

    bin.read(reinterpret_cast<char*>(&m_meta), sizeof(BD_AnimMeta));

    size_t frameCount = 0;
    bin.read(reinterpret_cast<char*>(&frameCount), sizeof(size_t));

    for(int i = 0; i < frameCount; i++) {
        BD_Frame f{};

        bin.read(reinterpret_cast<char*>(&f.chanType), sizeof(uint32_t));
        bin.read(reinterpret_cast<char*>(&f.boneID), sizeof(uint32_t));
        bin.read(reinterpret_cast<char*>(&f.vecData), sizeof(float) * 4);
        bin.read(reinterpret_cast<char*>(&f.time), sizeof(float));

        m_frames.push_back(f);
    }

    bin.close();
    return true;
}

int BD_AnimReader::GetFrameCount() {
    return m_frames.size();
}

BD_Frame BD_AnimReader::GetFrameByIndex(int idx) {
    assert(idx < m_frames.size());
    return m_frames[idx];
}

BD_AnimMeta BD_AnimReader::GetMeta() {
    return m_meta;
}