#ifndef EXPGE_BD_SKELREADER_H
#define EXPGE_BD_SKELREADER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>

#include "BD_SkelWriter.h"

class BD_SkelReader {
public:
    BD_SkelReader();
    ~BD_SkelReader();

    bool ReadFile(std::string file);

    Bone GetBoneByIndex(int idx);
    int GetBoneCount();

private:
    std::vector<Bone> m_bones;
};


#endif //EXPGE_BD_SKELREADER_H
