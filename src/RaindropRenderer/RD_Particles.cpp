#include "pch.h"
#include "RD_Particles.h"

RD_SmallParticleEmitter::RD_SmallParticleEmitter(
	RaindropRenderer* rndr, RD_ShaderMaterial* shader,
	vec3f emitPos, vec3f emitDir,
	float rate, float velocity, float lifetime)
{
	m_rndr = rndr;
	m_shader = shader;

	m_emitPos = emitPos;
	m_emitDir = emitDir;
	m_lifetime = lifetime;

	m_rate = rate;
	m_velocity = velocity;

	m_api = rndr->GetRenderingAPI();

	m_vbuff = rndr->GetRenderingAPI()->CreateVertexBufferInstanced();
	Bufferize();

	m_transf.resize(1000);

	m_matrix = TranslateMatrix(m_matrix, emitDir);
	m_matrix = ScaleMatrix(m_matrix, vec3f(0.5f, 0.5f, 0.5f));

	m_generator = std::mt19937(m_generator);
	m_distributor = std::uniform_real_distribution<float>(-0.1f, 0.1f);
	m_distribAngle = std::uniform_real_distribution<float>(-90.0f, 90.0f);
}

RD_SmallParticleEmitter::~RD_SmallParticleEmitter() {
	delete m_vbuff;
}

void RD_SmallParticleEmitter::Bufferize() {
	m_vbuff->CreateBuffer();

	float geom[] = {
		-1.0f, 1.0f, 0.0f,    0.0f, 0.0f, 1.0f,    0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,    0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,     0.0f, 0.0f, 1.0f,    1.0f, 1.0f,

		1.0f, 1.0f, 0.0f,     0.0f, 0.0f, 1.0f,    1.0f, 1.0f,
		-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,    0.0f, 0.0f,
		1.0f, -1.0f, 0.0f,    0.0f, 0.0f, 1.0f,    1.0f, 0.0f
	};

	float t[] = {
		0.0f, 0.0f, 0.0f
	};

	float t2[] = {
		0.0f, 0.0f, 0.0f
	};

	m_vbuff->FillBufferData(geom, 48);
	m_vbuff->SetVertexAttr(t , DataTypes::TVEC3, 3, 1, 0);
	m_vbuff->SetVertexAttr(t2, DataTypes::TVEC3, 3, 1, 1);
}

void RD_SmallParticleEmitter::UpdateParticles() {
	using namespace std::chrono;
	long long now = duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();

	if ((float)(now - lastPart) / 1000 > (1 / m_rate)) {
		lastPart = now;
		smParticle p{};
		p.creationStamp = now;
		p.partDir = m_emitDir + m_distributor(m_generator);
		p.partPos = m_emitPos;
		p.velocity = m_velocity;
		p.partRot = vec3f(m_distribAngle(m_generator), m_distribAngle(m_generator), 0.0f);

		m_particles.push_back(p);
	}

	if (m_particles.size() == 0) {
		return;
	}

	std::vector<float> pPos;
	std::vector<float> pRot;
	for (int i = m_particles.size() - 1; i >= 0; i--) {
		if ((float)(now - m_particles[i].creationStamp) / 1000 > m_lifetime) {
			m_particles.erase(m_particles.begin() + i);
			continue;
		}

		m_particles[i].partPos += (m_particles[i].partDir * m_particles[i].velocity);

		pPos.insert(pPos.end(), &m_particles[i].partPos.GetPTR()[0], &m_particles[i].partPos.GetPTR()[3]);
		pRot.insert(pRot.end(), &m_particles[i].partRot.GetPTR()[0], &m_particles[i].partRot.GetPTR()[3]);
	}

	m_vbuff->UpdateBufferData(pPos.data(), pPos.size() / 3, 0);
	m_vbuff->UpdateBufferData(pRot.data(), pRot.size() / 3, 1);
}

void RD_SmallParticleEmitter::RenderParticles() {
	if (m_particles.size() == 0) {
		return;
	}

	m_api->DisableFaceCulling();

	m_shader->GetShader()->useShader();
	m_shader->BindMaterial();
	m_shader->GetShader()->SetMatrix("model", m_matrix);

	m_vbuff->BindBuffer();
	m_api->DrawInstanced(m_vbuff, m_particles.size(), DrawMode::TRIANGLES);
	m_vbuff->UnbindBuffer();

	m_api->EnableFaceCulling();
}

void RD_SmallParticleEmitter::SetEmittingDirectionRandomness(float threshold) {
	m_distributor = std::uniform_real_distribution(-threshold, threshold);
}