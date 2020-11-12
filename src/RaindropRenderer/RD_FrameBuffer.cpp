#include "pch.h"
#include "RD_FrameBuffer.h"

RD_FrameBuffer_GL::RD_FrameBuffer_GL(int w, int h) : RD_FrameBuffer() {
	m_w = w;
	m_h = h;
	m_FBO = 0;

	CreateFBO();
}

RD_FrameBuffer_GL::~RD_FrameBuffer_GL() {
	for (auto attachement : m_attachments) {
		delete attachement;
	}
	m_attachments.clear();

	glDeleteFramebuffers(1, &m_FBO);
}

unsigned int RD_FrameBuffer_GL::GetFBO() {
	return m_FBO;
}

void RD_FrameBuffer_GL::CreateFBO() {
	glGenFramebuffers(1, &m_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RD_FrameBuffer_GL::AddAttachement(unsigned int format) {
	RD_Texture_GL* tex = new RD_Texture_GL();
	tex->CreateAndAttachToFramebuffer(m_w, m_h, m_FBO, m_nbrAttachement, format);

	m_nbrAttachement++;
}

void RD_FrameBuffer_GL::BuildFBO() {

}

void RD_FrameBuffer_GL::BindFBO() {
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
}

void RD_FrameBuffer_GL::UnbindFBO() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

RD_Texture* RD_FrameBuffer_GL::GetAttachementByIndex(int index) {
	return m_attachments[index];
}

int RD_FrameBuffer_GL::GetNumberOfAttachements() {
	return m_nbrAttachement;
}

void RD_FrameBuffer_GL::ChangeFramebufferSize(int nw, int nh) {
	for (auto attachement : m_attachments) {
		delete attachement;
	}
	m_attachments.clear();

	glDeleteFramebuffers(1, &m_FBO);

	m_h = nh;
	m_w = nw;

	CreateFBO();
	BuildFBO();
}