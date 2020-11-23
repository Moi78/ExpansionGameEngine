#include "pch.h"
#include "RD_Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#ifdef BUILD_OPENGL

RD_Texture_GL::RD_Texture_GL() {
	m_texture = 0;
}

RD_Texture_GL::~RD_Texture_GL() {
	DeleteTexture();
}

void RD_Texture_GL::LoadTexture(std::string tex, bool flipTex) {
	int w, h, nbrC;

	stbi_set_flip_vertically_on_load(flipTex);
	unsigned char* imgData = stbi_load(tex.c_str(), &w, &h, &nbrC, STBI_rgb_alpha);

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	//Wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

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

void RD_Texture_GL::CreateAndAttachToFramebuffer(int w, int h, unsigned int FBO, unsigned int attachement, unsigned int format, unsigned int scaleMode) {
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	int formatGL;
	int typeGL = GL_UNSIGNED_BYTE;
	int scaleMde;

	switch(format){
	case IMGFORMAT_R:
		formatGL = GL_R;
		break;

	case IMGFORMAT_RG:
		formatGL = GL_RG;
		break;
	
	case IMGFORMAT_RGB:
		formatGL = GL_RGB;
		break;

	case IMGFORMAT_RGBA:
		formatGL = GL_RGBA;
		break;

	case IMGFORMAT_R16F:
		formatGL = GL_R16F;
		typeGL = GL_FLOAT;
		break;

	case IMGFORMAT_RG16F:
		formatGL = GL_RG16F;
		typeGL = GL_FLOAT;
		break;

	case IMGFORMAT_RGB16F:
		formatGL = GL_RGB16F;
		typeGL = GL_FLOAT;
		break;

	case IMGFORMAT_RGBA16F:
		formatGL = GL_RGBA16F;
		typeGL = GL_FLOAT;
		break;

	case IMGFORMAT_DEPTH:
		formatGL = GL_DEPTH_COMPONENT;
		typeGL = GL_FLOAT;
		break;

	default:
		formatGL = GL_RGB;
		typeGL = GL_FLOAT;
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

	int format2 = format == IMGFORMAT_DEPTH ? GL_DEPTH_COMPONENT : GL_RGB;

	glTexImage2D(GL_TEXTURE_2D, 0, formatGL, w, h, 0, format2, typeGL, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, scaleMde);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, scaleMde);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

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

void RD_Texture_GL::BindTexture(unsigned int tex_unit) {
	glActiveTexture(GL_TEXTURE0 + tex_unit);
	glBindTexture(GL_TEXTURE_2D, m_texture);
}

void RD_Texture_GL::DeleteTexture() {
	glDeleteTextures(1, &m_texture);
}

#endif //BUILD_OPENGL