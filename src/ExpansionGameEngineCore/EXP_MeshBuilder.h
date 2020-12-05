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

#include <RD_MeshBuilder.h>

#include "EXP_Game.h"
#include "EXP_Component.h"

class EXPGE_API EXP_MeshBuilder :
	public RD_MeshBuilder,
	public EXP_Component
{
public:
	EXP_MeshBuilder(EXP_Game* game, RD_ShaderMaterial* shader, vec3f pos, vec3f rot, vec3f scale);
	~EXP_MeshBuilder();

	virtual void SetPosition(vec3f nPos) override;
	virtual void SetRotation(vec3f nRot) override;
	virtual void SetScale(vec3f nScale) override;

	virtual void UseParentMatrix(mat4f parent) override;
};

