#ifndef EXPGE_BD_SKELWRITER_H
#define EXPGE_BD_SKELWRITER_H

#include <vector>
#include <fstream>
#include <string>
#include <filesystem>

#include <vec.h>
#include <mat4.h>

struct Bone {
    std::string name;
    mat4f pos;
    int idx;
};

class BD_SkelWriter {
public:
    BD_SkelWriter();
    ~BD_SkelWriter();

    void AppendBone(Bone b);

    void WriteBinary(std::string outpath);

private:
    std::vector<Bone> m_bones;
};


#endif //EXPGE_BD_SKELWRITER_H
