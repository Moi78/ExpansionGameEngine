#include "pch.h"
#include "RD_Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

RD_Texture::RD_Texture() {
	m_texture = 0;
}

RD_Texture::~RD_Texture() {

}

void RD_Texture::LoadTexture(std::string tex) {
	int w, h, nbrC;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* imgData = stbi_load(tex.c_str(), &w, &h, &nbrC, 0);

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

	GLint format;
	if (nbrC == 3) {
		format = GL_RGB;
	}
	else {
		format = GL_RGBA;
	}

	if (imgData) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, format, GL_UNSIGNED_BYTE, imgData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cerr << "ERROR: Failed to load texture \"" << tex << "\"." << std::endl;
	}

	stbi_image_free(imgData);
}

unsigned int RD_Texture::GetTextureID() {
	return m_texture;
}

void RD_Texture::GenerateColorTex(vec3f color) {
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

void RD_Texture::CreateAndAttachToFramebuffer(int w, int h, unsigned int FBO, unsigned int attachement, unsigned int format) {
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, attachement, GL_TEXTURE_2D, m_texture, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RD_Texture::BindTexture(unsigned int tex_unit) {
	glActiveTexture(tex_unit);
	glBindTexture(GL_TEXTURE_2D, m_texture);
}