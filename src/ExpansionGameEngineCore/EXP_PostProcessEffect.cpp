#include "pch.h"
#include "EXP_PostProcessEffect.h"

EXP_PostProcessingEffect::EXP_PostProcessingEffect(EXP_Game* game, std::string shader) : RD_PostProcessEffect(game->GetRenderer()) {
	BD_MatCustomShaderRead* sread = new BD_MatCustomShaderRead(game->GetFilePathByRef(shader));

	m_game = game;

	m_shader = game->GetRenderer()->GetRenderingAPI()->CreateShader();
	std::string vertContent = getFileData(game->GetGameInfo().RootEngineContentFolder + "/Shaders/glsl/PostProcessing.vert");

	m_shader->CompileShaderFromCode(
		vertContent,
		sread->GetShaderCode(game->GetRenderer()->GetRenderingAPI()->GetAPIType() == API::OPENGL3 ? true : false)
	);

	RD_ShaderMaterial* mat = new RD_ShaderMaterial(m_shader, m_game->GetRenderer());
	LoadShader(mat);

	for (int i = 0; i < sread->GetTextureCount(); i++) {
		RD_Texture* tex = game->GetRenderer()->GetRenderingAPI()->CreateTexture();
		tex->LoadTexture(sread->GetTexturePath(i));

		mat->AddTexture(sread->GetTextureParamName(i), tex);
	}
	mat->MakeSSBO();

	game->GetRenderer()->AddPostProcessEffect(this);

	delete sread;
}

EXP_PostProcessingEffect::~EXP_PostProcessingEffect() {

}