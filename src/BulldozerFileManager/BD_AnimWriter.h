#ifndef EXPGE_BD_ANIMWRITER_H
#define EXPGE_BD_ANIMWRITER_H

#include <stdint.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

enum BD_ChannelType {
    BD_CHAN_ROT = 0x00,
    BD_CHAN_POS = 0x01,
    BD_CHAN_SCALE = 0x02,
    BD_CHAN_EMPTY = 0x03
};

struct BD_Frame {
    uint32_t chanType;
    uint32_t boneID;

    float vecData[4];
    float time;
};

struct BD_AnimMeta {
    double framerate;
    double duration;
};

class BD_AnimWriter {
public:
    BD_AnimWriter();
    ~BD_AnimWriter();

    void AddFrame(BD_Frame frame);
    void SetAnimMeta(const BD_AnimMeta& meta);
    void WriteAnimation(std::string path);

private:
    std::vector<BD_Frame> m_frames;
    BD_AnimMeta m_meta;
};


#endif //EXPGE_BD_ANIMWRITER_H
