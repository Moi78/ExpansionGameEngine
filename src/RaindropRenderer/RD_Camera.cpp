#include "RD_Camera.h"

RD_Camera::RD_Camera(std::shared_ptr<RD_API> api, float FOV, vec3 pos, vec3 target, float near, float far) {
    m_api = api;

    m_pos = pos;
    m_offset_pos = vec3();
    m_target = target;

    m_FOV = FOV;
    m_near = near;
    m_far = far;

    m_up = vec3(0.0f, 0.0f, 1.0f);

    UpdateCamera();
}

RD_Camera::~RD_Camera() {

}

void RD_Camera::UpdateCamera() {
    UpdateProj();
    UpdateView();
}

void RD_Camera::UpdateProj() {
    auto window = m_api->GetWindowingSystem();
    const float w = (float)window->GetWidth();
    const float h = (float)window->GetHeight();

    m_proj = ProjPersp(DEG_TO_RAD(-m_FOV), (w / h), m_near, m_far);
}

void RD_Camera::UpdateView() {
    m_view = LookAt(m_pos + m_offset_pos, m_target, m_up);
}

void RD_Camera::PushToUniform(const std::shared_ptr<RD_UniformBuffer>& buff) {
    buff->PartialFillBufferData(m_proj.GetPTR(), 16 * sizeof(float), 0);
    buff->PartialFillBufferData(m_view.GetPTR(), 16 * sizeof(float), 16 * sizeof(float));
}

void RD_Camera::SetPos(vec3 npos) {
    m_pos = npos;
    UpdateView();
}

void RD_Camera::SetOffsetPos(vec3 noffpos) {
    m_offset_pos = noffpos;
    UpdateView();
}

void RD_Camera::SetTarget(vec3 ntarget) {
    m_target = ntarget;
    UpdateView();
}

void RD_Camera::SetUp(vec3 nup) {
    m_up = nup;
    UpdateView();
}

vec3 RD_Camera::GetForwardVector() {
    return m_target.NormalizedVector();
}