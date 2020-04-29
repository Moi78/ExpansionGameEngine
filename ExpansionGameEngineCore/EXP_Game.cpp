#include "pch.h"
#include "EXP_Game.h"

#include "EXP_Actor.h"
#include "EXP_Camera.h"
#include "EXP_SoundEmitter.h"
#include "EXP_PhysicsHandler.h"
#include "EXP_Callbacks.h"
#include "EXP_HotLoad.h"
#include "EXP_MapLoader.h"
#include "EXP_Level.h"

EXP_Game::EXP_Game(BD_GameInfo gameinfo, vec3f refreshColor) {
	m_currentCamera = nullptr;
	m_first_exec = true;

	m_GameLib = new EXP_HotLoad();

	m_actors = {};
    

#ifdef _WIN32
	std::wstring glib = std::wstring(gameinfo.GameLib.begin(), gameinfo.GameLib.end());
    m_GameLib->LoadLib(glib.c_str());
#else
    m_GameLib->LoadLib(gameinfo.GameLib.c_str());
#endif //_WIN32
    
	InitPhysicaSound();
	InitGame(refreshColor, gameinfo);
	InitGui();
	InitPhysics();

	RD_Texture* defTex = new RD_Texture();
	defTex->LoadTexture(gameinfo.RootEngineContentFolder + "/Textures/defTex.png");

	RD_Texture* spec = new RD_Texture();
	spec->GenerateColorTex(vec3f(1.0f, 1.0f, 1.0f));

	m_def_mat = {};
	m_def_mat.BaseColor = defTex->GetTextureID();
	m_def_mat.Specular = spec->GetTextureID();
	m_def_mat.Shininess = 1.0f;

	delete spec;
	delete defTex;
}

EXP_Game::EXP_Game(std::string gameinfo) {
	BD_GameInfo gi = CreateGameInfoFromJSON(gameinfo);

	m_GameLib = new EXP_HotLoad();

	m_actors = {};

#ifdef _WIN32
	std::wstring glib = std::wstring(gi.GameLib.begin(), gi.GameLib.end());
    m_GameLib->LoadLib(glib.c_str());
#else
    m_GameLib->LoadLib(gi.GameLib.c_str());
#endif //_WIN32

	m_currentCamera = nullptr;
	m_first_exec = true;

	InitPhysicaSound();
	InitGame(vec3f(), gi);
	InitGui();
	InitPhysics();

	RD_Texture* defTex = new RD_Texture();
	defTex->LoadTexture(gi.RootEngineContentFolder + "/Textures/defTex.png");

	RD_Texture* spec = new RD_Texture();
	spec->GenerateColorTex(vec3f(1.0f, 1.0f, 1.0f));

	m_def_mat = {};
	m_def_mat.BaseColor = defTex->GetTextureID();
	m_def_mat.Specular = spec->GetTextureID();
	m_def_mat.Shininess = 1.0f;

	delete spec;
	delete defTex;
}

EXP_Game::~EXP_Game() {
	UnloadCurrentMap();
	delete m_GameLib;

	m_soundEngine->shutdownAL();

	delete m_physicsHandler;
	delete m_rndr;
}

BD_GameInfo EXP_Game::CreateGameInfoFromJSON(std::string file) {
	Json::Value root;
	JSONCPP_STRING errs;

	Json::CharReaderBuilder builder;
	builder["collectComment"] = false;

	std::ifstream gameinfo(file);

	if (!Json::parseFromStream(builder, gameinfo, &root, &errs)) {
		std::cerr << "Cannot load GameInfo.json file. " << errs << std::endl;
		return {}; //Returning void structure
	}

	std::string GameLib = root["GameBin"].asString();

	std::string engineFolder = root["RootEngineContentFolder"].asString();
	std::string contentFolder = root["RootGameContentFolder"].asString();
	std::string gameName = root["GameName"].asString();

	BD_Resolution res = {root["GameBaseResolution"][0].asInt(), root["GameBaseResolution"][1].asInt()};

	std::string startupMap = root["StartupMap"].asString();

	BD_GameInfo gi = {};
	gi.GameName = gameName;
	gi.RootEngineContentFolder = engineFolder;
	gi.RootGameContentFolder = contentFolder;
	gi.GameBaseResolution = res;
	gi.GameLib = GameLib;
	gi.StartupMap = startupMap;

	return gi;
}

void EXP_Game::InitGame(vec3f refreshColor, BD_GameInfo gameinfo) {
	m_res = gameinfo.GameBaseResolution;
	m_refreshColor = refreshColor;
	m_gameName = gameinfo.GameName;
	m_gameinfo = gameinfo;
    
	m_rndr = new RaindropRenderer(m_res.x, m_res.y, gameinfo.GameName, 60);

	m_PlayingMap = new EXP_MapLoader(this);
	m_PlayingMap->LoadMap(gameinfo.RootGameContentFolder + gameinfo.StartupMap);

	EXP_Level* lvl = m_PlayingMap->GetLevelCode();
	if (lvl) {
		lvl->CallStart();
	}
}

void EXP_Game::InitPhysicaSound() {
	m_soundEngine = new PSound();

	m_soundEngine->GetDevice();
	
	if (!m_soundEngine->initAL()) {
		std::cerr << "Cannot init OpenAL" << std::endl;

		return;
	}

    m_listener = new PS_Listener(vec3f(), vec3f());
    m_soundEngine->RegisterListener(m_listener);
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

	if (m_PlayingMap != nullptr) {
		EXP_Level* lvl = m_PlayingMap->GetLevelCode();
		if (lvl) {
			lvl->CallTick();
		}
	}

    vec3f CamLock;
    if(m_currentCamera == nullptr) {
        CamLock = vec3f();
    } else {
        CamLock = m_currentCamera->GetLocation();
    }
    
	//Process shadows
	m_rndr->RenderLightsDepth(CamLock);
	//GBuff
	m_rndr->RenderGbuff(m_currentCamera);
	//Light pass
	m_rndr->RenderLightPass(CamLock);

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
	m_rndr->RegisterMesh(mesh);
}

void EXP_Game::RegisterPointLight(RD_PointLight* ptlight) {
	m_rndr->AppendLight(ptlight);
}

vec3f EXP_Game::GetRefreshColor() {
	return m_refreshColor;
}

BD_MatDef EXP_Game::GetDefaultMaterial() {
	return m_def_mat;
}

BD_MatDef EXP_Game::FetchMaterialFromFile(std::string ref) {
	std::string fullpath = m_gameinfo.RootGameContentFolder + ref + ".exmtl";
	if (!std::filesystem::exists(fullpath)) {
		std::cerr << "Material file " << ref << " does not exist. Returning default material." << std::endl;
		return GetDefaultMaterial();
	}

	BD_MatRead* mr = new BD_MatRead();
	BD_WriteMdef mat = mr->ReadMaterialFromFile(fullpath);

	delete mr;

	BD_MatDef mdef = {};

	RD_Texture* bc = new RD_Texture();

	if (mat.BaseColorRef != "") {
		bc->LoadTexture(m_gameinfo.RootGameContentFolder + mat.BaseColorRef);
	}
	else {
		bc->GenerateColorTex(mat.BaseColor);
	}

	mdef.BaseColor = bc->GetTextureID();
	delete bc;

	RD_Texture* spec = new RD_Texture();

	if (mat.SpecularColorRef != "") {
		spec->LoadTexture(m_gameinfo.RootGameContentFolder + mat.SpecularColorRef);
	}
	else {
		spec->GenerateColorTex(mat.SpecularColor);
	}

	if (mat.NormalMap != "") {
		RD_Texture* normm = new RD_Texture();
		normm->LoadTexture(m_gameinfo.RootGameContentFolder + mat.NormalMap);
		mdef.NormalMap = normm->GetTextureID();
		mdef.NormalEnabled = true;
	}
	else {
		mdef.NormalEnabled = false;
	}

	mdef.Specular = spec->GetTextureID();
	delete spec;

	mdef.Shininess = mat.Shininess;

	return mdef;
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
		act->UpdateActor();
	}
}

void EXP_Game::RegisterActor(EXP_Actor* act) {
	m_actors.push_back(act);
}

void EXP_Game::UnregisterKeyboardCallback(EXP_KeyboardCallback* kbcllbck) {
	int index = GetElemIndex<EXP_KeyboardCallback*>(m_kb_callbacks, kbcllbck);

	if (index != -1) {
		m_kb_callbacks.erase(m_kb_callbacks.begin() + index);
	}
	else {
		std::cerr << "ERROR: Element does not exists" << std::endl;
	}
}

void EXP_Game::UnregisterMesh(RD_Mesh* mesh) {
	m_rndr->UnregisterMesh(mesh);
}

void EXP_Game::UnregisterDirLight(RD_DirLight* dlight) {
	m_rndr->UnregisterDirLight(dlight);
}

void EXP_Game::UnregisterPointLight(RD_PointLight* plight) {
	m_rndr->UnregisterPointLight(plight);
}

void EXP_Game::UnregisterActor(EXP_Actor* actor) {
	int index = GetElemIndex<EXP_Actor*>(m_actors, actor);

	if (index != -1) {
		m_actors.erase(m_actors.begin() + index);
		actor->CallUnregister();
	}
	else {
		std::cerr << "ERROR: Element does not exists" << std::endl;
	}
}

EXP_HotLoad* EXP_Game::GetGameLib() {
	return m_GameLib;
}

void EXP_Game::UnloadCurrentMap() {
	//m_actors.size();

	for (int i = 0; i < m_actors.size(); i++) {
		if (m_actors[i]) {
			m_actors[i]->CallUnregister();
			delete m_actors[i];
		}
	}
	if(!m_actors.empty())
		m_actors.clear();

	m_PlayingMap->UnloadMap();
}

void EXP_Game::LoadMap(std::string map) {
	UnloadCurrentMap();
	m_PlayingMap->LoadMap(m_gameinfo.RootGameContentFolder + map);
}