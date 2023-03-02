#ifndef EXPGE_BD_ANIMREADER_H
#define EXPGE_BD_ANIMREADER_H

#include "BD_AnimWriter.h"
#include <fstream>

class BD_AnimReader {
public:
    BD_AnimReader();
    ~BD_AnimReader();

    bool ReadFile(std::string file);

    BD_Frame GetFrameByIndex(int idx);
    int GetFrameCount();

    BD_AnimMeta GetMeta();

private:
    std::vector<BD_Frame> m_frames;
    BD_AnimMeta m_meta;
};


#endif //EXPGE_BD_ANIMREADER_H
