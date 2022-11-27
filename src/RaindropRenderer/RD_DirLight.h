#ifndef EXPGE_RD_DIRLIGHT_H
#define EXPGE_RD_DIRLIGHT_H

#include <vec.h>
#include <mat4.h>

#include "RD_UniformBuffer.h"

class RD_DirLight {
public:
    RD_DirLight(vec3 dir, float brightness, vec3 color, bool shadow_caster = true);
    ~RD_DirLight();

    void SetShadowCaster(bool state);
    bool IsShadowCaster();

    void ComputeMatrix();
    mat4f GetMatProj();
    mat4f GetMatView();
    mat4f GetTransMat();

    void PushToUniform(std::shared_ptr<RD_UniformBuffer> buff, int idx);

private:
    vec3 m_dir;
    vec3 m_color;
    float m_brightness;

    bool m_shadowCaster;
    mat4f m_lightProj;
    mat4f m_lightView;
};

struct GLSLDirLight {
    float dir[3];
    float brightness;
    float color[4];
};

#endif //EXPGE_RD_DIRLIGHT_H
