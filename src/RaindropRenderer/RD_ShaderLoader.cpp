#include "pch.h"
#include "RD_ShaderLoader.h"

RD_ShaderLoader::RD_ShaderLoader() {
	m_program_id = 0;
}

RD_ShaderLoader::~RD_ShaderLoader() {
	glUseProgram(0);
	glDeleteProgram(m_program_id);
}

void RD_ShaderLoader::compileShaderFromFile(std::string vertexShaderFile, std::string fragmentShaderFile) {
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

void RD_ShaderLoader::CompileShaderFromCode(std::string vertexCode, std::string fragmentCode) {
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


void RD_ShaderLoader::useShader() {
	glUseProgram(m_program_id);
}

void RD_ShaderLoader::SetBool(const std::string& name, bool value) {
	unsigned int uniLoc = glGetUniformLocation(m_program_id, name.c_str());

	glUniform1i(uniLoc, (int)value);
}

void RD_ShaderLoader::SetInt(const std::string& name, int value) {
	unsigned int uniloc = glGetUniformLocation(m_program_id, name.c_str());

	glUniform1i(uniloc, value);
}

void RD_ShaderLoader::SetFloat(const std::string& name, float value) {
	unsigned int uniloc = glGetUniformLocation(m_program_id, name.c_str());

	glUniform1f(uniloc, value);
}

void RD_ShaderLoader::SetMatrix(const std::string& name, glm::mat4 matrix) {
	unsigned int uniloc = glGetUniformLocation(m_program_id, name.c_str());

	glUniformMatrix4fv(uniloc, 1, GL_FALSE, glm::value_ptr(matrix));
}

void RD_ShaderLoader::SetMatrix(const std::string& name, mat4f matrix) {
	unsigned int uniloc = glGetUniformLocation(m_program_id, name.c_str());

	glUniformMatrix4fv(uniloc, 1, GL_TRUE, matrix.GetPTR());
}

void RD_ShaderLoader::SetVec3(const std::string& name, vec3f vec) {
	unsigned int uniloc = glGetUniformLocation(m_program_id, name.c_str());

	glUniform3f(uniloc, vec.getX(), vec.getY(), vec.getZ());
}

unsigned int RD_ShaderLoader::GetProgID() {
	return m_program_id;
}
