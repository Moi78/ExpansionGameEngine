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

#define TEXT(x) L"x"

#endif //_WIN32

#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>

#include <glad/glad.h>

#include <mat4.h>
#include <vec3.h>

#include "RD_Error.h"

#include <BulldozerFileManager.h>

class RAINDROPRENDERER_API RD_ShaderLoader
{
public:
	RD_ShaderLoader();
	~RD_ShaderLoader();

	void compileShaderFromFile(std::string vertexShaderFile, std::string fragmentShaderFile);
	void CompileShaderFromCode(std::string vertexCode, std::string fragmentCode);

	void useShader();

	void SetBool(const std::string &name, bool value);
	void SetInt(const std::string& name, int value);
	void SetFloat(const std::string& name, float value);

	void SetMatrix(const std::string& name, mat4f matrix);

	void SetVec3(const std::string& name, vec3f vec);

	unsigned int GetProgID();

private:
	unsigned int m_program_id;
};

#endif //_RD_SHADER_LOADER_H__
