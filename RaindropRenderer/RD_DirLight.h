#ifndef EXPGE_RD_DIRLIGHT_H
#define EXPGE_RD_DIRLIGHT_H

#include <vec.h>
#include "RD_UniformBuffer.h"

class RD_DirLight {
public:
    RD_DirLight(vec3 dir, float brightness, vec3 color);
    ~RD_DirLight();

    void PushToUniform(std::shared_ptr<RD_UniformBuffer> buff, int idx);

private:
    vec3 m_dir;
    vec3 m_color;
    float m_brightness;
};

struct GLSLDirLight {
    float dir[3];
    float brightness;
    float color[4];
};

#endif //EXPGE_RD_DIRLIGHT_H
