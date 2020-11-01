#pragma once

#ifdef _WIN32
	#ifdef EXPANSIONGAMEENGINECORE_EXPORTS
		#define EXPGE_API __declspec(dllexport)
	#else
		#define EXPGE_API __declspec(dllimport)
	#endif //EXPANSIONGAMEENGINECORE_EXPORTS
#else
	#define EXPGE_API
#endif //_WIN32

#include "EXP_Game.h"

#include <RD_PostProcess.h>

#include <BD_MatRW.h>

class EXPGE_API EXP_PostProcessingEffect : public RD_PostProcessEffect
{
public:
	EXP_PostProcessingEffect(EXP_Game* game, std::string shaderFile);
	~EXP_PostProcessingEffect();

private:
	EXP_Game* m_game;

	RD_ShaderLoader* m_shader;
};

