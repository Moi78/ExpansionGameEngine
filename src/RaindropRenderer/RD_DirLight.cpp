#include "RD_DirLight.h"

RD_DirLight::RD_DirLight(vec3 dir, float brightness, vec3 color, bool shadowCaster) {
    m_dir = dir;
    m_color = color;
    m_brightness = brightness;

    m_shadowCaster = shadowCaster;
    m_lightProj = mat4f(1.0f);
    m_lightView = mat4f(1.0f);

    ComputeMatrix();
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

    dlight.isShadowCaster = m_shadowCaster;

    buff->PartialFillBufferData(&dlight, sizeof(GLSLDirLight), idx * sizeof(GLSLDirLight));
}

void RD_DirLight::SetShadowCaster(bool state) {
    m_shadowCaster = state;
}

bool RD_DirLight::IsShadowCaster() {
    return m_shadowCaster;
}

void RD_DirLight::ComputeMatrix() {
    if(!m_shadowCaster) {
        return;
    }

    m_lightProj = ProjOrtho(-15.0f, 15.0f, -15.0f, 15.0f, -10.0f, 30.0f);
    m_lightView = LookAt((m_dir * -1.0f) + vec3(0.0f, 0.0f, 2.0f), vec3(0.0f, 0.0f, 2.0f), vec3(0.0f, 0.0f, 1.0f));
}

mat4f RD_DirLight::GetMatProj() {
    return m_lightProj;
}

mat4f RD_DirLight::GetMatView() {
    return m_lightView;
}

mat4f RD_DirLight::GetTransMat() {
    return m_lightProj * m_lightView;
}