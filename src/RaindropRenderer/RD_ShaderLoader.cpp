#include "pch.h"
#include "RD_ShaderLoader.h"

#ifdef BUILD_OPENGL

RD_ShaderLoader_GL::RD_ShaderLoader_GL() {
	m_program_id = 0;
}

RD_ShaderLoader_GL::~RD_ShaderLoader_GL() {
	glUseProgram(0);
	glDeleteProgram(m_program_id);
}

void RD_ShaderLoader_GL::compileShaderFromFile(std::string vertexShaderFile, std::string fragmentShaderFile) {
	if (!std::filesystem::exists(vertexShaderFile)) {
		dispErrorMessageBox(StrToWStr("Cannot open " + vertexShaderFile + " path does not exists."));
		return;
	}

	if (!std::filesystem::exists(fragmentShaderFile)) {
		dispErrorMessageBox(StrToWStr("Cannot open " + fragmentShaderFile + " path does not exists."));
		return;
	}

	//Common variables
	std::string vertexShaderData;
	std::string fragmentShaderData;

	const char* c_vertexShaderData;
	const char* c_fragmentShaderData;

	unsigned int vertexShader;
	unsigned int fragmentShader;

	int success;
	char infoLog[512];

	//Variable Initialization
	vertexShaderData = getFileData(vertexShaderFile);
	fragmentShaderData = getFileData(fragmentShaderFile);

	c_vertexShaderData = vertexShaderData.c_str();
	c_fragmentShaderData = fragmentShaderData.c_str();

	//Compiling vertex shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &c_vertexShaderData, NULL);
	glCompileShader(vertexShader);

	//Get compiling error
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		dispErrorMessageBox(TEXT("Vertex shader error, see console."));
		std::cerr << "GLSL Compile Error (Vertex shader) : " << infoLog << std::endl;
	}

	//Compiling fragment shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, &c_fragmentShaderData, NULL);
	glCompileShader(fragmentShader);

	//Get compiling error
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		dispErrorMessageBox(TEXT("Fragment shader error, see console."));
		std::cerr << "GLSL Compile Error (Fragment shader) : " << infoLog << std::endl;
	}

	//Creating && Linking program
	m_program_id = glCreateProgram();
	//Linking
	glAttachShader(m_program_id, vertexShader);
	glAttachShader(m_program_id, fragmentShader);
	glLinkProgram(m_program_id);

	//Get linking errors
	glGetProgramiv(m_program_id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(m_program_id, 512, NULL, infoLog);
		dispErrorMessageBox(TEXT("Error while linking program, see console."));
		std::cerr << "GLSL Linking Error : " << infoLog << std::endl;
	}

	//Delete shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void RD_ShaderLoader_GL::CompileShaderFromCode(std::string vertexCode, std::string fragmentCode) {
	//Common variables
	const char* c_vertexShaderData;
	const char* c_fragmentShaderData;

	unsigned int vertexShader;
	unsigned int fragmentShader;

	int success;
	char infoLog[512];

	//Variable Initialization
	c_vertexShaderData = vertexCode.c_str();
	c_fragmentShaderData = fragmentCode.c_str();

	//Compiling vertex shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &c_vertexShaderData, NULL);
	glCompileShader(vertexShader);

	//Get compiling error
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		dispErrorMessageBox(TEXT("Vertex shader error, see console."));
		std::cerr << "GLSL Compile Error (Vertex shader) : " << infoLog << std::endl;
	}

	//Compiling fragment shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, &c_fragmentShaderData, NULL);
	glCompileShader(fragmentShader);

	//Get compiling error
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		dispErrorMessageBox(TEXT("Fragment shader error, see console."));
		std::cerr << "GLSL Compile Error (Fragment shader) : " << infoLog << std::endl;
	}

	//Creating && Linking program
	m_program_id = glCreateProgram();
	//Linking
	glAttachShader(m_program_id, vertexShader);
	glAttachShader(m_program_id, fragmentShader);
	glLinkProgram(m_program_id);

	//Get linking errors
	glGetProgramiv(m_program_id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(m_program_id, 512, NULL, infoLog);
		dispErrorMessageBox(TEXT("Error while linking program, see console."));
		std::cerr << "GLSL Linking Error : " << infoLog << std::endl;
	}

	//Delete shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}


void RD_ShaderLoader_GL::useShader() {
	glUseProgram(m_program_id);
}

void RD_ShaderLoader_GL::SetBool(const std::string& name, bool value) {
	unsigned int uniLoc = glGetUniformLocation(m_program_id, name.c_str());

	glUniform1i(uniLoc, (int)value);
}

void RD_ShaderLoader_GL::SetInt(const std::string& name, int value) {
	unsigned int uniloc = glGetUniformLocation(m_program_id, name.c_str());

	glUniform1i(uniloc, value);
}

void RD_ShaderLoader_GL::SetFloat(const std::string& name, float value) {
	unsigned int uniloc = glGetUniformLocation(m_program_id, name.c_str());

	glUniform1f(uniloc, value);
}

void RD_ShaderLoader_GL::SetMatrix(const std::string& name, mat4f matrix) {
	unsigned int uniloc = glGetUniformLocation(m_program_id, name.c_str());

	glUniformMatrix4fv(uniloc, 1, GL_TRUE, matrix.GetPTR());
}

void RD_ShaderLoader_GL::SetVec3(const std::string& name, vec3f vec) {
	unsigned int uniloc = glGetUniformLocation(m_program_id, name.c_str());

	//glUniform3f(uniloc, vec.getX(), vec.getY(), vec.getZ());
	glUniform3fv(uniloc, 1, vec.GetPTR());
}

unsigned int RD_ShaderLoader_GL::GetProgID() {
	return m_program_id;
}

RD_UniformBuffer_GL::RD_UniformBuffer_GL(const size_t bufferSize, const int binding) {
	m_binding = binding;
	m_UBO = NULL;

	CreateUBO(bufferSize);
}

void RD_UniformBuffer_GL::CreateUBO(const size_t bufferSize) {
	glGenBuffers(1, &m_UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, m_UBO);
	glBufferData(GL_UNIFORM_BUFFER, bufferSize, NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, m_binding, m_UBO, 0, bufferSize);
}

RD_UniformBuffer_GL::~RD_UniformBuffer_GL() {
	glDeleteBuffers(1, &m_UBO);
}

void RD_UniformBuffer_GL::BindBuffer() {
	glBindBuffer(GL_UNIFORM_BUFFER, m_UBO);
}

void RD_UniformBuffer_GL::UnbindBuffer() {
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void RD_UniformBuffer_GL::SetBufferSubData(const int offset, const size_t size, void* data) {
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
}

RD_ShaderStorageBuffer_GL::RD_ShaderStorageBuffer_GL(const size_t bufferSize, const int binding) {
	m_binding = binding;
	m_SSBO = 0;
	CreateSSBO(bufferSize);
}

RD_ShaderStorageBuffer_GL::~RD_ShaderStorageBuffer_GL() {
	glDeleteBuffers(1, &m_SSBO);
}

void RD_ShaderStorageBuffer_GL::BindBuffer() {
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_SSBO);
}

void RD_ShaderStorageBuffer_GL::UnbindBuffer() {
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void RD_ShaderStorageBuffer_GL::SetBufferSubData(const int offset, const size_t size, void* data) {
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, size, data);
}

void RD_ShaderStorageBuffer_GL::CreateSSBO(const size_t bufferSize) {
	glGenBuffers(1, &m_SSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_SSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, bufferSize, NULL, GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_binding, m_SSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

#endif //BUILD_OPENGL