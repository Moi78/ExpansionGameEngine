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

class RAINDROPRENDERER_API RD_ShaderLoader {
public:
	RD_ShaderLoader() {}
	virtual ~RD_ShaderLoader() {};

	virtual void compileShaderFromFile(std::string vertexShaderFile, std::string fragmentShaderFile) = 0;
	virtual void CompileShaderFromCode(std::string vertexCode, std::string fragmentCode) = 0;

	virtual void useShader() = 0;

	virtual void SetBool(const std::string& name, bool value) = 0;
	virtual void SetInt(const std::string& name, int value) = 0;
	virtual void SetFloat(const std::string& name, float value) = 0;

	virtual void SetMatrix(const std::string& name, mat4f matrix) = 0;

	virtual void SetVec3(const std::string& name, vec3f vec) = 0;

	virtual unsigned int GetProgID() = 0;
};

#ifdef BUILD_OPENGL

class RAINDROPRENDERER_API RD_ShaderLoader_GL : public RD_ShaderLoader
{
public:
	RD_ShaderLoader_GL();
	~RD_ShaderLoader_GL();

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

#endif //BUILD_OPENGL

#endif //_RD_SHADER_LOADER_H__
