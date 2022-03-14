#include "pch.h"
#include "RD_ReflectionCapture.h"

RD_ReflectionCapture::RD_ReflectionCapture(RaindropRenderer* rndr, vec3f capturePoint) {
	m_rndr = rndr;
	m_capPoint = capturePoint;

	m_cam = new RD_Camera(rndr, 90.0f, 0.001f, 10000.0f, capturePoint, vec3f(0.0f, 0.0f, 0.0f), true);

	for (int i = 0; i < 6; i++) {
		m_fbs[i] = m_rndr->GetRenderingAPI()->CreateFrameBuffer(512, 512, true);
		m_fbs[i]->AddAttachement(IMGFORMAT_RGB);
		m_fbs[i]->BuildFBO();
	}

	m_capEnv = m_rndr->GetRenderingAPI()->CreateCubemap();
}

RD_ReflectionCapture::~RD_ReflectionCapture() {
	delete m_cam;

	for (auto fb : m_fbs) {
		delete fb;
	}

	delete m_capEnv;
}

void RD_ReflectionCapture::RenderCapture() {
	m_cam->UseCamera(nullptr);
	m_rndr->ResizeViewport(vec2f(0.0f, 0.0f), vec2f(512.0f, 512.0f));

	const std::array<vec3f, 6> rots = {
		vec3f(0.0f, 0.0f, 90.0f),
		vec3f(0.0f, 0.0f, 90.0f),
		vec3f(0.0f, 0.0f, 90.0f),
		vec3f(0.0f, 0.0f, 90.0f),
		vec3f(90.0f, 0.0f, 0.0f),
		vec3f(-180.0f, 0.0f, 0.0f),
	};

	for (int i = 0; i < 6; i++) {
		//Process shadows
		m_rndr->RenderLightsDepth(m_cam->GetLocation());
		//GBuff
		m_rndr->RenderGbuff(m_cam);

		//PostProcessing
		m_fbs[i]->BindFBO();
		m_rndr->RenderBeauty();
		m_fbs[i]->UnbindFBO();

		m_cam->RotateCamera(rots[i]);
	}

	m_rndr->ResizeViewport(vec2f(0.0f, 0.0f), vec2f(m_rndr->getWindowWidth(), m_rndr->getWindowHeigh()));
}