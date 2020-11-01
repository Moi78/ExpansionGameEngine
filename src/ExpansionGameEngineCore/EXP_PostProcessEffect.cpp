#include "pch.h"
#include "EXP_PostProcessEffect.h"

EXP_PostProcessingEffect::EXP_PostProcessingEffect(EXP_Game* game, std::string shader) : RD_PostProcessEffect() {
	BD_MatCustomShaderRead* sread = new BD_MatCustomShaderRead(game->GetFilePathByRef(shader));

	m_game = game;

	m_shader = new RD_ShaderLoader();
	std::string vertContent = getFileData(game->GetGameInfo().RootEngineContentFolder + "/Shaders/glsl/PostProcessing.vert");
	m_shader->CompileShaderFromCode(vertContent, sread->GetShaderCode());

	RD_ShaderMaterial* mat = new RD_ShaderMaterial(m_shader);
	LoadShader(mat);

	for (int i = 0; i < sread->GetTextureCount(); i++) {
		RD_Texture* tex = new RD_Texture();
		tex->LoadTexture(sread->GetTexturePath(i));

		mat->AddTexture(sread->GetTextureParamName(i), tex->GetTextureID());

		delete tex;
	}

	game->GetRenderer()->AddPostProcessEffect(this);

	delete sread;
}

EXP_PostProcessingEffect::~EXP_PostProcessingEffect() {

}