#ifndef EXPGE_RD_POINTLIGHT_H
#define EXPGE_RD_POINTLIGHT_H

#include <memory>

#include <vec.h>
#include "RD_UniformBuffer.h"

class RD_PointLight {
public:
    RD_PointLight(vec3 pos, float brightness, vec3 color, float radius);
    ~RD_PointLight();

    void PushToUniform(std::shared_ptr<RD_UniformBuffer> buff, int index);

private:
    vec3 m_pos;
    float m_brightness;
    float m_radius;
    vec3 m_color;
};

struct GLSLPointLight {
    float pos[3];
    float brightness;
    float color[3];
    float radius;
};

#endif //EXPGE_RD_POINTLIGHT_H
