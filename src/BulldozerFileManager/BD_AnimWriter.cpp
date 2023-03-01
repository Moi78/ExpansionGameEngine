#include "BD_AnimWriter.h"

BD_AnimWriter::BD_AnimWriter() {
    m_meta = BD_AnimMeta{};
}

BD_AnimWriter::~BD_AnimWriter() {

}

void BD_AnimWriter::AddFrame(BD_Frame frame) {
    m_frames.push_back(frame);
}

void BD_AnimWriter::SetAnimMeta(const BD_AnimMeta &meta) {
    m_meta = meta;
}

void BD_AnimWriter::WriteAnimation(std::string path) {
    std::ofstream file = std::ofstream(path.c_str(), std::ios::binary);
    if(!file.is_open()) {
        std::cerr << "ERROR: Failed to write file " << path << std::endl;
        return;
    }

    file.write(reinterpret_cast<const char*>(&m_meta), sizeof(BD_AnimMeta));

    size_t arr_size = m_frames.size();
    file.write(reinterpret_cast<const char*>(&arr_size), sizeof(size_t));
    for(auto& f : m_frames) {
        file.write(reinterpret_cast<const char*>(&f.chanType), sizeof(uint32_t));
        file.write(reinterpret_cast<const char*>(&f.boneID), sizeof(uint32_t));
        file.write(reinterpret_cast<const char*>(&f.vecData), sizeof(float) * 4);
        file.write(reinterpret_cast<const char*>(&f.time), sizeof(float));
    }

    file.close();
}