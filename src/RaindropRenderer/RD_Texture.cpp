#include "pch.h"
#include "RD_Texture.h"
#include "RaindropRenderer.h"
#include "RD_RenderingAPI.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#ifdef BUILD_OPENGL

RD_Texture_GL::RD_Texture_GL() {
	m_texture = 0;
	m_ms_texture = 0;
	m_ms = false;
	m_isTexBindless = false;
}

RD_Texture_GL::~RD_Texture_GL() {
	DeleteTexture();
}

void RD_Texture_GL::LoadTexture(const std::string& tex, const bool flipTex) {
	int w, h, nbrC;

	stbi_set_flip_vertically_on_load(flipTex);
	unsigned char* imgData = stbi_load(tex.c_str(), &w, &h, &nbrC, STBI_rgb_alpha);

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	//Wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//MipMaps
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLint format = GL_RGBA;

	if (imgData) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, format, GL_UNSIGNED_BYTE, imgData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cerr << "ERROR: Failed to load texture \"" << tex << "\"." << std::endl;
	}

	stbi_image_free(imgData);
}

unsigned int RD_Texture_GL::GetTextureID() {
	return m_texture;
}

void RD_Texture_GL::GenerateColorTex(vec3f color) {
	std::vector<unsigned char> image(16 * 16 * 3);

	for (int i = 0; i < image.size(); i += 3) {
		image[i] = 255 * color.getX();
		image[i + 1] = 255 * color.getY();
		image[i + 2] = 255 * color.getZ();
	}

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	//Wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	//Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 16, 16, 0, GL_RGB, GL_UNSIGNED_BYTE, &image[0]);

	image.clear();
}

void RD_Texture_GL::CreateAndAttachToFramebuffer(int w, int h, unsigned int FBO, unsigned int attachement, unsigned int format, unsigned int scaleMode, unsigned int wrapmode) {
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	unsigned int formatGL = GL_RGB;
	unsigned int typeGL = GL_UNSIGNED_BYTE;
	unsigned int scaleMde = GL_LINEAR;
	unsigned int wrapmde = GL_REPEAT;

	GetGLformat(format, scaleMode, wrapmode, &formatGL, &typeGL, &scaleMde, &wrapmde);

	int format2;
	if (format == IMGFORMAT_DEPTH) {
		format2 = GL_DEPTH_COMPONENT;
	}
	else if (format == IMGFORMAT_RGBA) {
		format2 = GL_RGBA;
	}
	else {
		format2 = GL_RGB;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, formatGL, w, h, 0, format2, typeGL, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, scaleMde);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, scaleMde);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapmde);

	if (format != IMGFORMAT_DEPTH) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachement, GL_TEXTURE_2D, m_texture, 0);
	}
	else {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_texture, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool RD_Texture_GL::BindTexture(unsigned int tex_unit) {
	if (m_ms) {
		return false;
	}

	if (m_isTexBindless) {
		return false;
	}

	glActiveTexture(GL_TEXTURE0 + tex_unit);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	return true;
}

void RD_Texture_GL::DeleteTexture() {
	glDeleteTextures(1, &m_texture);
}

void RD_Texture_GL::CreateTextureFromPixels(void* pixels, int w, int h, unsigned format, unsigned int wrapmode) {
	unsigned int formatGL = GL_RGB;
	unsigned int typeGL = GL_UNSIGNED_BYTE;
	unsigned int wrap = wrapmode;

	GetGLformat(format, NULL, NULL, &formatGL, &typeGL, nullptr, &wrap);

	glGenTextures(1, &m_texture);

	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, formatGL, w, h, 0, GL_RGB, typeGL, pixels);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
}

void RD_Texture_GL::CreateAndAttachToFramebufferMS(
	int w, int h,
	unsigned int FBO,
	unsigned int attachment, 
	unsigned int format, 
	unsigned int scaleMode, 
	unsigned int wrapmode) {
	m_ms = true;

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glGenTextures(1, &m_ms_texture);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_ms_texture);

	unsigned int formatGL = GL_RGB;
	unsigned int typeGL = GL_UNSIGNED_BYTE;
	unsigned int scaleMde = GL_LINEAR;
	unsigned int wrapmde = GL_REPEAT;

	GetGLformat(format, scaleMode, wrapmode, &formatGL, &typeGL, &scaleMde, &wrapmde);


	int format2;
	if (format == IMGFORMAT_DEPTH) {
		format2 = GL_DEPTH_COMPONENT;
	}
	else if (format == IMGFORMAT_RGBA) {
		format2 = GL_RGBA;
	}
	else {
		format2 = GL_RGB;
	}

	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 8, formatGL, w, h, GL_TRUE);

	if (format != IMGFORMAT_DEPTH) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachment, GL_TEXTURE_2D_MULTISAMPLE, m_ms_texture, 0);
	}
	else {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, m_ms_texture, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RD_Texture_GL::CreateTextureFromGlyph(void* data, const int w, const int h) {
	glGenTextures(1, &m_texture);

	glBindTexture(GL_TEXTURE_2D, m_texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
}

void RD_Texture_GL::MakeTexBindless(RaindropRenderer* rndr, RD_ShaderStorageBuffer* ssbo, const int index) {
	assert(m_texture != 0 && "ERROR: You can't make a texture bindless as long as you didn't create the texture.");
	if (!rndr->GetRenderingAPI()->AreBindlessTexturesAvailable()) {
		return;
	}

	m_texHandle = glGetTextureHandleARB(m_texture);
	glMakeTextureHandleResidentARB(m_texHandle);

	ssbo->BindBuffer();
	ssbo->SetBufferSubData(index * sizeof(uint64_t), sizeof(uint64_t), (void*)&m_texHandle);
	ssbo->UnbindBuffer();

	m_isTexBindless = true;
}

void RD_Texture_GL::GetGLformat(
	unsigned int format,
	unsigned int scaleMode,
	unsigned int wrapmode,
	unsigned int* formatgl,
	unsigned int* typeGL,
	unsigned int* scaleModegl,
	unsigned int* wrapmodeGL) {

	unsigned int fgl = GL_RGB;
	unsigned int tgl = GL_UNSIGNED_BYTE;
	unsigned int scaleMde = GL_LINEAR;
	unsigned int wrapmde = GL_REPEAT;
	
	switch (format) {
	case IMGFORMAT_R:
		fgl = GL_R;
		break;

	case IMGFORMAT_RG:
		fgl = GL_RG;
		break;

	case IMGFORMAT_RGB:
		fgl = GL_RGB;
		break;

	case IMGFORMAT_RGBA:
		fgl = GL_RGBA;
		break;

	case IMGFORMAT_R16F:
		fgl = GL_R16F;
		tgl = GL_FLOAT;
		break;

	case IMGFORMAT_RG16F:
		fgl = GL_RG16F;
		tgl = GL_FLOAT;
		break;

	case IMGFORMAT_RGB16F:
		fgl = GL_RGB16F;
		tgl = GL_FLOAT;
		break;

	case IMGFORMAT_RGBA16F:
		fgl = GL_RGBA16F;
		tgl = GL_FLOAT;
		break;

	case IMGFORMAT_DEPTH:
		fgl = GL_DEPTH_COMPONENT;
		tgl = GL_FLOAT;
		break;

	default:
		fgl = GL_RGB;
		tgl = GL_FLOAT;
		break;
	}

	switch (scaleMode) {
	case SCALEMODE_LINEAR:
		scaleMde = GL_LINEAR;
		break;

	case SCALEMODE_NEAREST:
		scaleMde = GL_NEAREST;
		break;

	default:
		scaleMde = GL_LINEAR;
		break;
	}

	switch(wrapmode) {
	case WRAPMODE_REPEAT:
		wrapmde = GL_REPEAT;
		break;
	case WRAPMODE_CLAMP2EDGE:
		wrapmde = GL_CLAMP_TO_EDGE;
		break;
	default:
		wrapmde = GL_REPEAT;
		break;
	}

	if(formatgl)
		*formatgl = fgl;

	if(typeGL)
		*typeGL = tgl;

	if(scaleModegl)
		*scaleModegl = scaleMde;

	if(wrapmodeGL) {
		*wrapmodeGL = wrapmde;
	}
}

uint64_t RD_Texture_GL::GetTextureHandle() {
	return m_texHandle;
}

unsigned int RD_Texture_GL::GetTextureMSID() {
	return m_ms_texture;
}

#endif //BUILD_OPENGL