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

#endif //BUILD_OPENGL

#ifdef BUILD_D3D11

RD_ShaderLoader_DX11::RD_ShaderLoader_DX11(ID3D11Device* device) {
	m_device = device;
}

RD_ShaderLoader_DX11::~RD_ShaderLoader_DX11() {

}

void RD_ShaderLoader_DX11::compileShaderFromFile(std::string vertFile, std::string fragFile) {
	std::vector<uint8_t> vertContent = getFileDataBin(vertFile);
	std::vector<uint8_t> fragContent = getFileDataBin(fragFile);

	HRESULT hr = m_device->CreateVertexShader(vertContent.data(), vertContent.size(), NULL, &m_vertShader);
	if (FAILED(hr)) {
		dispErrorMessageBox(StrToWStr(std::string("Cannot create vertex shader from " + vertFile)));
		return;
	}

	hr = m_device->CreatePixelShader(fragContent.data(), fragContent.size(), NULL, &m_fragShader);
	if (FAILED(hr)) {
		dispErrorMessageBox(StrToWStr(std::string("Cannot create fragment shader from " + fragFile)));
		return;
	}

	//Input layout of the shader
	D3D11_INPUT_ELEMENT_DESC inLayout[3];
	inLayout[0].SemanticName = "POSITION";
	inLayout[0].SemanticIndex = 0;
	inLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inLayout[0].InputSlot = 0;
	inLayout[0].AlignedByteOffset = 0;
	inLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inLayout[0].InstanceDataStepRate = 0;

	inLayout[1].SemanticName = "NORMAL";
	inLayout[1].SemanticIndex = 0;
	inLayout[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inLayout[1].InputSlot = 0;
	inLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	inLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inLayout[1].InstanceDataStepRate = 0;

	inLayout[2].SemanticName = "TEXCOORD";
	inLayout[2].SemanticIndex = 0;
	inLayout[2].Format = DXGI_FORMAT_R32G32_FLOAT;
	inLayout[2].InputSlot = 0;
	inLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	inLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inLayout[2].InstanceDataStepRate = 0;
	
	hr = m_device->CreateInputLayout(inLayout, 3, vertContent.data(), vertContent.size(), &m_layout);
	if (FAILED(hr)) {
		dispErrorMessageBox(StrToWStr("Failed to create input layout of shader " + vertFile));
		return;
	}
}

void RD_ShaderLoader_DX11::CompileShaderFromCode(std::string vertexCode, std::string fragmentCode) {

}

void RD_ShaderLoader_DX11::useShader() {

}

void RD_ShaderLoader_DX11::SetBool(const std::string& name, bool value) {}
void RD_ShaderLoader_DX11::SetInt(const std::string& name, int value) {}
void RD_ShaderLoader_DX11::SetFloat(const std::string& name, float value) {}
void RD_ShaderLoader_DX11::SetMatrix(const std::string& name, mat4f matrix) {}
void RD_ShaderLoader_DX11::SetVec3(const std::string& name, vec3f vec) {}

//------------------------------ RD_UniformBuffer_DX11 ------------------------------

RD_UniformBuffer_DX11::RD_UniformBuffer_DX11(ID3D11Device* device, const size_t bufferSize, const int binding) {
	m_device = device;
	m_binding = binding;

	m_device->GetImmediateContext(&m_context);

	CreateBuffer(bufferSize);
}

RD_UniformBuffer_DX11::~RD_UniformBuffer_DX11() {

}

void RD_UniformBuffer_DX11::SetBufferSubData(const int offset, const size_t size, void* data) {
	D3D11_BOX offst{};
	offst.right = offset;

	D3D11_MAPPED_SUBRESOURCE mappedSubr{};
	mappedSubr.pData = data;
	mappedSubr.DepthPitch = 0;
	mappedSubr.RowPitch = 0;

	m_context->Map(m_ubo, offset, D3D11_MAP::D3D11_MAP_WRITE, NULL, &mappedSubr);
}

void RD_UniformBuffer_DX11::BindBuffer() {
	return;
}

void RD_UniformBuffer_DX11::UnbindBuffer() {
	return;
}

void RD_UniformBuffer_DX11::CreateBuffer(const size_t size) {
	D3D11_BUFFER_DESC UBO_Desc{};
	UBO_Desc.ByteWidth = size;
	UBO_Desc.Usage = D3D11_USAGE_DYNAMIC;
	UBO_Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	UBO_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	UBO_Desc.MiscFlags = NULL;
	UBO_Desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = nullptr;
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	HRESULT hr = m_device->CreateBuffer(&UBO_Desc, nullptr, &m_ubo);
	if (FAILED(hr)) {
		dispErrorMessageBox(StrToWStr(std::string("Failed to create constant buffer (binding = " + std::to_string(m_binding) + ")")));
		return;
	}

	m_context->VSSetConstantBuffers(m_binding, 1, &m_ubo);
	m_context->PSSetConstantBuffers(m_binding, 1, &m_ubo);
}

#endif // BUILD_D3D11