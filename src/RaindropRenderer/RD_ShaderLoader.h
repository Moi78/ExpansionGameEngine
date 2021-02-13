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
#include <filesystem>

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

class RAINDROPRENDERER_API RD_UniformBuffer {
public:
	RD_UniformBuffer() {}
	virtual ~RD_UniformBuffer() {}

	virtual void SetBufferSubData(const int offset, const size_t size, void* data) = 0;

	virtual void BindBuffer() = 0;
	virtual void UnbindBuffer() = 0;
};

#ifdef BUILD_OPENGL

class RAINDROPRENDERER_API RD_ShaderLoader_GL : public RD_ShaderLoader
{
public:
	RD_ShaderLoader_GL();
	~RD_ShaderLoader_GL();

	virtual void compileShaderFromFile(std::string vertexShaderFile, std::string fragmentShaderFile);
	virtual void CompileShaderFromCode(std::string vertexCode, std::string fragmentCode);

	virtual void useShader();

	virtual void SetBool(const std::string &name, bool value);
	virtual void SetInt(const std::string& name, int value);
	virtual void SetFloat(const std::string& name, float value);

	virtual void SetMatrix(const std::string& name, mat4f matrix);

	virtual void SetVec3(const std::string& name, vec3f vec);

	virtual unsigned int GetProgID();

private:
	unsigned int m_program_id;
};

class RAINDROPRENDERER_API RD_UniformBuffer_GL : public RD_UniformBuffer {
public:
	RD_UniformBuffer_GL(const size_t bufferSize, const int binding);
	virtual ~RD_UniformBuffer_GL();

	virtual void SetBufferSubData(const int offset, const size_t size, void* data);

	virtual void BindBuffer();
	virtual void UnbindBuffer();
private:
	void CreateUBO(const size_t bufferSize);

	unsigned int m_UBO;
	int m_binding;
};

#endif //BUILD_OPENGL

#ifdef BUILD_D3D11

#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>

class RAINDROPRENDERER_API RD_ShaderLoader_DX11 : public RD_ShaderLoader
{
public:
	RD_ShaderLoader_DX11(ID3D11Device* device);
	~RD_ShaderLoader_DX11();

	virtual void compileShaderFromFile(std::string vertexShaderFile, std::string fragmentShaderFile);
	virtual void CompileShaderFromCode(std::string vertexCode, std::string fragmentCode);

	virtual void useShader();

	virtual void SetBool(const std::string& name, bool value);
	virtual void SetInt(const std::string& name, int value);
	virtual void SetFloat(const std::string& name, float value);

	virtual void SetMatrix(const std::string& name, mat4f matrix);

	virtual void SetVec3(const std::string& name, vec3f vec);

	virtual unsigned int GetProgID() { return 0; }

private:
	ID3D11Device* m_device;

	ID3D11VertexShader* m_vertShader;
	ID3D11PixelShader* m_fragShader;

	ID3D11InputLayout* m_layout;
};

#endif // BUILD_D3D11

#endif //_RD_SHADER_LOADER_H__
