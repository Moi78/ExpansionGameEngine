#include "pch.h"
#include "RD_FrameBuffer.h"

#ifdef BUILD_OPENGL

RD_FrameBuffer_GL::RD_FrameBuffer_GL(int w, int h, bool nodepth) : RD_FrameBuffer() {
	m_w = w;
	m_h = h;
	m_FBO = 0;
	m_nodepth = nodepth;
	m_ms = false;

	m_storage = GL_DEPTH_COMPONENT;
	m_rbo_attachement = GL_DEPTH_ATTACHMENT;

	CreateFBO();
}

RD_FrameBuffer_GL::~RD_FrameBuffer_GL() {
    for (auto attachement : m_attachments) {
		delete attachement.tex;
	}
	m_attachments.clear();
    
    glDeleteFramebuffers(1, &m_FBO);
	glDeleteRenderbuffers(1, &m_RBO);
}

unsigned int RD_FrameBuffer_GL::GetFBO() {
	return m_FBO;
}

void RD_FrameBuffer_GL::CreateFBO() {
	glGenFramebuffers(1, &m_FBO);
}

void RD_FrameBuffer_GL::AddAttachement(unsigned int format, unsigned int scaleMode, unsigned int wrapmode) {
	if (m_ms) {
		m_FBO_nMS->AddAttachement(format, scaleMode, wrapmode);
	}

	Attachement a;

	RD_Texture_GL* tex = nullptr;

	a.tex = tex;
	a.format = format;
	a.scaleMode = scaleMode;
	a.wrapmode = wrapmode;

	m_attachments.push_back(a);
}

void RD_FrameBuffer_GL::BuildFBO() {
	std::vector<unsigned int> attach;
	bool renderBufferDepth = !m_nodepth;

	for (int i = 0; i < m_attachments.size(); i++) {
		delete m_attachments[i].tex;

		m_attachments[i].tex = new RD_Texture_GL();

		if (m_ms) {
			m_attachments[i].tex->CreateAndAttachToFramebufferMS(
				m_w,
				m_h,
				m_FBO,
				i,
				m_attachments[i].format,
				m_attachments[i].scaleMode,
				m_attachments[i].wrapmode
			);
		}
		else {
			m_attachments[i].tex->CreateAndAttachToFramebuffer(
				m_w,
				m_h,
				m_FBO,
				i,
				m_attachments[i].format,
				m_attachments[i].scaleMode,
				m_attachments[i].wrapmode
			);
		}

		attach.push_back(GL_COLOR_ATTACHMENT0 + i);

		if(m_attachments[i].format == IMGFORMAT_DEPTH) {
			renderBufferDepth = false;
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glDrawBuffers(attach.size(), &attach[0]);

	if (renderBufferDepth) {
		glGenRenderbuffers(1, &m_RBO);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);

		if (!m_ms) {
			glRenderbufferStorage(GL_RENDERBUFFER, m_storage, m_w, m_h);
		}
		else {
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, m_storage, m_w, m_h);
		}

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, m_rbo_attachement, GL_RENDERBUFFER, m_RBO);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cerr << "ERROR: Framebuffer not complete." << std::endl;
		}
	}

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "ERROR: Framebuffer not complete." << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (m_ms) {
		m_FBO_nMS->BuildFBO();
	}
}

void RD_FrameBuffer_GL::BindFBO() {
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
}

void RD_FrameBuffer_GL::BindNonMSFBO() {
	m_FBO_nMS->BindFBO();
}

void RD_FrameBuffer_GL::UnbindFBO() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

RD_Texture* RD_FrameBuffer_GL::GetAttachementByIndex(int index) {
	assert(m_attachments.size() > index && "(RD_Framebuffer::GetAttachementByIndex(int)) Wrong given index.");

	if (m_ms) {
		return m_FBO_nMS->GetAttachementByIndex(index);
	}

	return m_attachments[index].tex;
}

int RD_FrameBuffer_GL::GetNumberOfAttachements() {
	return m_nbrAttachement;
}

void RD_FrameBuffer_GL::ChangeFramebufferSize(int nw, int nh) {
	glDeleteFramebuffers(1, &m_FBO);
	glDeleteRenderbuffers(1, &m_RBO);

	m_h = nh;
	m_w = nw;

	CreateFBO();
	BuildFBO();

	if (m_ms) {
		m_FBO_nMS->ChangeFramebufferSize(nw, nh);
	}
}

void RD_FrameBuffer_GL::ConfigureRenderbuffer(int storage, int attachement) {
	switch (attachement)
	{
	case DEPTH_ATTACHEMENT:
		m_rbo_attachement = GL_DEPTH_ATTACHMENT;
		break;

	case DEPTH24_STENCIL8_ATTACHEMENT:
		m_rbo_attachement = GL_DEPTH24_STENCIL8;
		break;
	default:
		m_rbo_attachement = GL_DEPTH_ATTACHMENT;
		break;
	}

	switch (storage)
	{
	case DEPTH_COMPONENT:
		m_storage = GL_DEPTH_COMPONENT;
		break;

	case DEPTH_STENCIL_COMPONENT:
		m_storage = GL_DEPTH_STENCIL;
		break;
	default:
		m_storage = GL_DEPTH_COMPONENT;
		break;
	}
}

void RD_FrameBuffer_GL::DebugMode() {
	glBindFramebuffer(GL_READ_FRAMEBUFFER, GetFBO());
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, m_w, m_h, 0, 0, m_w, m_h, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RD_FrameBuffer_GL::SetMultisampled(bool state) {
	m_ms = state;
	m_FBO_nMS = new RD_FrameBuffer_GL(m_w, m_h, m_nodepth);
}

void RD_FrameBuffer_GL::BindRead() {
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_FBO);
}

void RD_FrameBuffer_GL::BindWrite() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FBO);
}

bool RD_FrameBuffer_GL::IsFBMultisampled() {
	return m_ms;
}

std::vector<Attachement> RD_FrameBuffer_GL::GetFBAttachements() {
	return m_attachments;
}

void RD_FrameBuffer_GL::MultisampledToIntermediate() {
	BindRead();
	m_FBO_nMS->BindWrite();

	for (int i = 0; i < m_attachments.size(); i++) {
		glReadBuffer(GL_COLOR_ATTACHMENT0 + i);
		glDrawBuffer(GL_COLOR_ATTACHMENT0 + i);

		glBlitFramebuffer(0, 0, m_w, m_h, 0, 0, m_w, m_h, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}
}

#endif //BUILD_OPENGL
