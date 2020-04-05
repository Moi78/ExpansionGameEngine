#pragma once

#ifndef _RD_SHADER_LOADER_H__
#define _RD_SHADER_LOADER_H__

#ifdef _WIN32
#ifdef RAINDROPRENDERER_EXPORTS
#define RAINDROPRENDERER_API __declspec(dllexport)
#else
#define RAINDROPRENDERER_API __declspec(dllimport)
#endif

#include <Windows.h>

#else

#define RAINDROPRENDERER_API

#endif //_WIN32

#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "RD_Error.h"

#include "vec3.h"

class RAINDROPRENDERER_API RD_ShaderLoader
{
public:
	RD_ShaderLoader();
	~RD_ShaderLoader();

	std::string getFileData(std::string filePath);

	void compileShaderFromFile(std::string vertexShaderFile, std::string fragmentShaderFile);

	void useShader();

	void SetBool(const std::string &name, bool value);
	void SetInt(const std::string& name, int value);
	void SetFloat(const std::string& name, float value);

	void SetMatrix(const std::string& name, glm::mat4 matrix);
	void SetVec3(const std::string& name, vec3f vec);

	unsigned int GetProgID();

private:
	unsigned int m_program_id;
};

#endif //_RD_SHADER_LOADER_H__
