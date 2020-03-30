#include "pch.h"
#include "EXP_Game.h"

#include "EXP_Actor.h"
#include "EXP_Camera.h"
#include "EXP_SoundEmitter.h"
#include "EXP_PhysicsHandler.h"
#include "EXP_Callbacks.h"

EXP_Game::EXP_Game(BD_Resolution res, BD_GameInfo gameinfo, vec3f refreshColor) {
	m_currentCamera = nullptr;
	m_first_exec = true;
	
	InitPhysicaSound();
	InitGame(res, refreshColor, gameinfo);
	InitGui();
	InitPhysics();

	RD_Texture* defTex = new RD_Texture();
	defTex->LoadTexture(gameinfo.RootEngineContentFolder + "/Textures/defTex.png");

	m_def_mat = {};
	m_def_mat.BaseColor = defTex->GetTextureID();
	m_def_mat.SpecularColor = vec3f(1.0f, 1.0f, 1.0f);
	m_def_mat.Shininess = 1.0f;
	m_def_mat.SpecularStrength = 0.5f;
}

EXP_Game::~EXP_Game() {
	m_soundEngine->shutdownAL();

	m_points_light.clear();
	m_staticMeshes.clear();
	m_actors.clear();

	delete m_soundEngine;
	delete m_rndr;
	delete m_physicsHandler;
	delete m_currentCamera;
	delete m_listener;
}

void EXP_Game::InitGame(BD_Resolution res, vec3f refreshColor, BD_GameInfo gameinfo) {
	m_res = res;
	m_refreshColor = refreshColor;
	m_gameName = gameinfo.GameName;
	m_gameinfo = gameinfo;

	m_rndr = new RaindropRenderer(res.x, res.y, gameinfo.GameName, 60);
}

void EXP_Game::InitPhysicaSound() {
	m_soundEngine = new PSound();

	m_soundEngine->GetDevice();
	
	if (!m_soundEngine->initAL()) {
		std::cerr << "Cannot init OpenAL" << std::endl;

		return;
	}

	if (m_currentCamera != nullptr) {
		m_listener = new PS_Listener(m_currentCamera->GetPosition(), m_currentCamera->GetSubject());
		m_soundEngine->RegisterListener(m_listener);
	}
	else {
		m_listener = new PS_Listener(vec3f(), vec3f());
		m_soundEngine->RegisterListener(m_listener);
	}
}

void EXP_Game::InitPhysics() {
	m_physicsHandler = new EXP_PhysicsHandler(vec3f(0.0f, 0.0f, -9.82f), GetRenderer()->GetFrameLimit());
	m_physicsHandler->InitWorld();
}

void EXP_Game::UpdateSound() {
	m_listener->setPosition(m_currentCamera->GetPosition());
	m_listener->setOrientation(m_currentCamera->GetSubject());
	m_soundEngine->mainLoop();
}

void EXP_Game::InitGui() {

}

void EXP_Game::MainLoop() {
	m_rndr->ClearWindow(m_refreshColor);

	for (auto mesh : m_staticMeshes) {
		mesh->render();
	}

	if (RENDER_DBG) {
		m_rndr->RenderDbg();
	}

	if (m_currentCamera != nullptr) {
		m_currentCamera->UpdateCamera();
	}

	if (m_first_exec) {
		for (auto act : m_actors) {
			act->CallOnStart();
		}

		m_first_exec = false;
	}

	UpdateSound();
	UpdatePhysics();
	UpdateCallbacks();

	m_rndr->SwapWindow();
}

BD_GameInfo EXP_Game::GetGameInfo() {
	return m_gameinfo;
}

RaindropRenderer* EXP_Game::GetRenderer() {
	return m_rndr;
}

void EXP_Game::RegisterMesh(RD_Mesh* mesh) {
	m_staticMeshes.push_back(mesh);
}

void EXP_Game::RegisterPointLight(RD_PointLight* ptlight) {
	m_points_light.push_back(ptlight);
	m_rndr->AppendLight(ptlight);
}

vec3f EXP_Game::GetRefreshColor() {
	return m_refreshColor;
}

std::vector<RD_Mesh*> EXP_Game::GetStaticMeshes() {
	return m_staticMeshes;
}

BD_MatDef EXP_Game::GetDefaultMaterial() {
	return m_def_mat;
}

BD_MatDef EXP_Game::FetchMaterialFromFile(std::string ref) {
	std::string fullpath = m_gameinfo.RootGameContentFolder + ref + ".exmtl";

	BD_MatRead* mr = new BD_MatRead();
	BD_MatDef mat = mr->ReadMaterialFromFile(fullpath);

	delete mr;

	return mat;
}

PSound* EXP_Game::GetSoundEngine() {
	return m_soundEngine;
}

void EXP_Game::PlaySimpleSound(std::string ref, float gain) {
	std::string fullref = m_gameinfo.RootGameContentFolder + ref;

	std::thread t([](PSound* engine, std::string file, float gain) {
		engine->playSimpleSound(file, gain);
	}, m_soundEngine, fullref, gain);

	t.detach();
}

void EXP_Game::PlaySound3D(std::string ref, vec3f pos, float gain) {
	std::string fullref = m_gameinfo.RootGameContentFolder + ref;

	vec3f* cpos = &m_currentCamera->GetPosition();
	vec3f* cdir = &m_currentCamera->GetSubject();

	std::thread t([](PSound* engine, std::string file, vec3f pos, float gain) {
		engine->playSound3D(file, pos, gain);
	}, m_soundEngine, fullref, pos, gain);

	t.detach();
}

void EXP_Game::RegisterCamera(EXP_Camera* cam) {
	m_currentCamera = cam;
	m_listener->setPosition(cam->GetPosition());
	m_listener->setOrientation(cam->GetSubject());
}

void EXP_Game::RegisterSoundEmitter(EXP_SoundEmitter* sm) {
	m_soundEngine->RegisterEmitter(sm, false);
	m_rndr->RegisterSoundEmitter(sm);
}

std::string EXP_Game::GetFilePathByRef(std::string ref) {
	return m_gameinfo.RootGameContentFolder + ref;
}

void EXP_Game::UpdatePhysics() {
	m_physicsHandler->UpdateWorld();
}

EXP_PhysicsHandler* EXP_Game::GetPhysicsHandler() {
	return m_physicsHandler;
}

void EXP_Game::RegisterKeyboardCallback(EXP_KeyboardCallback* callback) {
	m_kb_callbacks.push_back(callback);
}

void EXP_Game::UpdateCallbacks() {
	for (auto cllbck : m_kb_callbacks) {
		cllbck->UpdateCallback();
	}

	for (auto act : m_actors) {
		act->CallTick();
	}
}

void EXP_Game::RegisterActor(EXP_Actor* act) {
	m_actors.push_back(act);
}