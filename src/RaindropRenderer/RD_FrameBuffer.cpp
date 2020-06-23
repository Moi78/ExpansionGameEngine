#include "pch.h"
#include "RD_FrameBuffer.h"

RD_FrameBuffer::RD_FrameBuffer(int w, int h, unsigned int nbrAttachement) {
	m_w = w;
	m_h = h;
	m_FBO = 0;
	m_nbrAttachement = nbrAttachement;

	CreateFBO();
	CreateAttachements();
}

RD_FrameBuffer::~RD_FrameBuffer() {
	glDeleteFramebuffers(1, &m_FBO);
}

unsigned int RD_FrameBuffer::GetFBO() {
	return m_FBO;
}

void RD_FrameBuffer::CreateFBO() {
	glGenFramebuffers(1, &m_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RD_FrameBuffer::CreateAttachements() {
	unsigned int att = GL_COLOR_ATTACHMENT0;

	for (int i = 0; i < m_nbrAttachement; i++) {
		std::shared_ptr<RD_Texture> tex = std::make_shared<RD_Texture>();
		tex->CreateAndAttachToFramebuffer(m_w, m_h, m_FBO, att);

		m_attachments.push_back(tex.get());

		att++;
	}
}

void RD_FrameBuffer::BindFBO() {
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
}

void RD_FrameBuffer::UnbindFBO() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

RD_Texture* RD_FrameBuffer::GetAttachementByIndex(int index) {
	return m_attachments[index];
}

int RD_FrameBuffer::GetNumberOfAttachements() {
	return m_nbrAttachement;
}