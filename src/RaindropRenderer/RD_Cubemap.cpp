#include "pch.h"
#include "RD_Cubemap.h"

#ifdef BUILD_OPENGL

RD_Cubemap_GL::RD_Cubemap_GL() {
	m_texture = 0;
}

RD_Cubemap_GL::~RD_Cubemap_GL() {
	if(m_texture != 0)
		glDeleteTextures(1, &m_texture);
}


void RD_Cubemap_GL::BuildCubemapFromImages(std::array<std::string, 6> face_filepath, const bool flipTex) {
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);

	int faceID = 0;
	for(auto& path : face_filepath) {
		int w = 0, h = 0, nbrChan = 0;
		unsigned char* data = stbi_load(path.c_str(), &w, &h, &nbrChan, STBI_rgb_alpha);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + faceID, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		faceID++;
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void RD_Cubemap_GL::BindTexture(const int tex_unit) {
	glActiveTexture(GL_TEXTURE0 + tex_unit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
}


#endif //BUILD_OPENGL