#ifndef EXPGE_RD_CAMERA_H
#define EXPGE_RD_CAMERA_H

#include "mat4.h"
#include "vec.h"

#include "RD_Pipeline.h"
#include "RD_UniformBuffer.h"
#include "RD_API.h"

class RD_Camera {
public:
    RD_Camera(std::shared_ptr<RD_API> api, float FOV, vec3 pos, vec3 target, float near, float far);
    virtual ~RD_Camera();

    void SetupPipeline(std::shared_ptr<RD_Pipeline> pline);

    void UpdateCamera();
    void UpdateView();
    void UpdateProj();
private:
    std::shared_ptr<RD_API> m_api;

    mat4f m_proj;
    mat4f m_view;

    vec3 m_pos;
    vec3 m_target;
    float m_FOV, m_near, m_far;

    std::shared_ptr<RD_UniformBuffer> m_camBuff;
};

#endif //EXPGE_RD_CAMERA_H
