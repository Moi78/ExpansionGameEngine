#ifndef EXPGE_RD_CAMERA_H
#define EXPGE_RD_CAMERA_H

#include "mat4.h"
#include "vec.h"
#include "quat.h"

#include "RD_Pipeline.h"
#include "RD_UniformBuffer.h"
#include "RD_API.h"

class RD_Camera {
public:
    RD_Camera(std::shared_ptr<RD_API> api, float FOV, vec3 pos, vec3 target, float near, float far);
    virtual ~RD_Camera();

    void PushToUniform(const std::shared_ptr<RD_UniformBuffer>& buff);
    void PushCamDataToUniform(const std::shared_ptr<RD_UniformBuffer>& buff);

    void SetPos(vec3 npos);
    void SetOffsetPos(vec3 noffpos);
    void SetTarget(vec3 ntarget);
    void SetUp(vec3 nup);

    vec3 GetForwardVector();

    void UpdateCamera();
    void UpdateView();
    void UpdateProj();

protected:
    std::shared_ptr<RD_API> m_api;

    mat4f m_proj;
    mat4f m_view;

    vec3 m_pos, m_offset_pos;
    vec3 m_target, m_up;
    float m_FOV, m_near, m_far;
};

#endif //EXPGE_RD_CAMERA_H
