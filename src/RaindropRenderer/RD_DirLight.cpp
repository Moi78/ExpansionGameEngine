#include "RD_DirLight.h"

RD_DirLight::RD_DirLight(vec3 dir, float brightness, vec3 color) {
    m_dir = dir;
    m_color = color;
    m_brightness = brightness;
}

RD_DirLight::~RD_DirLight() {

}

void RD_DirLight::PushToUniform(std::shared_ptr<RD_UniformBuffer> buff, int idx) {
    GLSLDirLight dlight{};
    dlight.brightness = m_brightness;

    dlight.color[0] = m_color.GetX();
    dlight.color[1] = m_color.GetY();
    dlight.color[2] = m_color.GetZ();

    dlight.dir[0] = m_dir.GetX();
    dlight.dir[1] = m_dir.GetY();
    dlight.dir[2] = m_dir.GetZ();

    buff->PartialFillBufferData(&dlight, sizeof(GLSLDirLight), idx * sizeof(GLSLDirLight));
}