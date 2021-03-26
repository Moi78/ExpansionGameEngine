#pragma once

#ifdef _WIN32
	#ifdef RAINDROPRENDERER_EXPORTS
		#define RD_API __declspec(dllexport)
	#else
		#define RD_API __declspec(dllimport)
	#endif //RAINDROPRENDERER_EXPORTS
#else
	#define RD_API
#endif //_WIN32

#include "RaindropRenderer.h"
#include "RD_ShaderLoader.h"
#include "RD_RenderingAPI.h"

#include <vec3.h>
#include <mat4.h>

#include <vector>
#include <chrono>
#include <random>

struct smParticle {
	vec3f partPos;
	vec3f partDir;
	mat4f partMat;
	float velocity;

	int creationStamp;
};

class RD_API RD_ParticleEmitter {
public:
	RD_ParticleEmitter() {};
	virtual ~RD_ParticleEmitter() {};

	virtual void UpdateParticles() = 0;
	virtual void RenderParticles() = 0;

	virtual void UseParentMatrix(mat4f mat) = 0;
};

class RD_API RD_SmallParticleEmitter : public RD_ParticleEmitter
{
public:
	RD_SmallParticleEmitter(RaindropRenderer* rndr, RD_ShaderMaterial* shader, vec3f emitPos, vec3f emitDir, float rate, float velocity, float lifetime);
	virtual ~RD_SmallParticleEmitter();

	void SetEmittingDirectionRandomness(float threshold);
	void SetEmittingPos(vec3f nPos) { m_emitPos = nPos; }
	void SetEmittingDir(vec3f nDir) { m_emitDir = nDir; }

	virtual void UpdateParticles() override;
	virtual void RenderParticles() override;

	virtual void UseParentMatrix(mat4f mat);

private:
	void Bufferize();

	RaindropRenderer* m_rndr;
	RD_RenderingAPI* m_api;
	RD_ShaderMaterial* m_shader;
	RD_RenderingAPI_VertexBufferInstanced* m_vbuff;

	vec3f m_emitPos;
	vec3f m_emitDir;

	std::vector<smParticle> m_particles;
	std::vector<vec3f> m_transf;

	std::random_device m_dev;
	std::mt19937 m_generator;
	std::uniform_real_distribution<float> m_distributor;
	std::uniform_real_distribution<float> m_distribAngle;

	mat4f m_matrix;
	mat4f m_parent_mat;

	float m_rate;
	float m_velocity;
	float m_lifetime;

	long long lastPart;
};

