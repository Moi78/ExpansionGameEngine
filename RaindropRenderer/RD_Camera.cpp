#include "RD_Camera.h"

RD_Camera::RD_Camera(std::shared_ptr<RD_API> api, float FOV, vec3 pos, vec3 target, float near, float far) {
    m_api = api;

    m_pos = pos;
    m_target = target;

    m_FOV = FOV;
    m_near = near;
    m_far = far;

    m_camBuff = api->CreateUniformBuffer(0);
    m_camBuff->BuildAndAllocateBuffer(16 * sizeof(float) * 2);

    UpdateCamera();
}

RD_Camera::~RD_Camera() {

}

void RD_Camera::UpdateCamera() {
    UpdateProj();
    UpdateView();
}

void RD_Camera::UpdateProj() {
    m_proj = ProjPersp(m_FOV, 1280.0f / 720.0f, m_near, m_far);

    m_camBuff->PartialFillBufferData(m_proj.GetPTR(), 16 * sizeof(float), 0);
}

void RD_Camera::UpdateView() {
    m_view = LookAt(m_pos, m_target, vec3(0.0f, 0.0f, 1.0f));

    m_camBuff->PartialFillBufferData(m_view.GetPTR(), 16 * sizeof(float), 16 * sizeof(float));
}

void RD_Camera::SetupPipeline(std::shared_ptr<RD_Pipeline> pline) {
    pline->RegisterUniformBuffer(m_camBuff);
}