#include "RD_PointLight.h"

RD_PointLight::RD_PointLight(vec3 pos, float brightness, vec3 color, float radius) {
    m_pos = pos;
    m_brightness = brightness;
    m_color = color;
    m_radius = radius;
}

RD_PointLight::~RD_PointLight() {

}

void RD_PointLight::PushToUniform(std::shared_ptr<RD_UniformBuffer> buff, int index) {
    GLSLPointLight plight{};
    plight.pos[0] = m_pos.GetX();
    plight.pos[1] = m_pos.GetY();
    plight.pos[2] = m_pos.GetZ();

    plight.color[0] = m_color.GetX();
    plight.color[1] = m_color.GetY();
    plight.color[2] = m_color.GetZ();

    plight.brightness = m_brightness;
    plight.radius = m_radius;

    buff->PartialFillBufferData(&plight, sizeof(GLSLPointLight), sizeof(GLSLPointLight) * index);
}