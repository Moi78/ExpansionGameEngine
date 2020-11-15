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
#include "EXP_InputHandler.h"

EXP_Game::EXP_Game(BD_GameInfo gameinfo, vec3f refreshColor) {
	m_currentCamera = nullptr;
	m_error_flag = false;

	m_GameLib = std::make_unique<EXP_HotLoad>();

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
}

EXP_Game::EXP_Game(std::string gameinfo) {
	BD_GameInfo gi = CreateGameInfoFromJSON(gameinfo);
	m_error_flag = false;
	m_currentCamera = nullptr;

	m_GameLib = std::make_unique<EXP_HotLoad>();

	m_actors = {};

#ifdef _WIN32
	std::wstring glib = std::wstring(gi.GameLib.begin(), gi.GameLib.end());
    m_GameLib->LoadLib(glib.c_str());
#else
    m_GameLib->LoadLib(gi.GameLib.c_str());
#endif //_WIN32

	InitPhysicaSound();
	InitGame(vec3f(), gi);
	InitGui();
}

EXP_Game::~EXP_Game() {
	UnloadCurrentMap();
	m_soundEngine->shutdownAL();
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

	//Moving "root" to platform corresponding GameInfo
	Json::Value TargetRoot = root[EXECPLATFORM];

	std::string GameLib = TargetRoot["GameBin"].asString();

	std::string engineFolder = TargetRoot["RootEngineContentFolder"].asString();
	std::string contentFolder = TargetRoot["RootGameContentFolder"].asString();
	std::string gameName = TargetRoot["GameName"].asString();

	BD_Resolution res = { TargetRoot["GameBaseResolution"][0].asInt(), TargetRoot["GameBaseResolution"][1].asInt()};

	std::string startupMap = TargetRoot["StartupMap"].asString();

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

	m_rndr = std::make_shared<RaindropRenderer>(m_res.x, m_res.y, gameinfo.GameName, API::OPENGL, 60, false, gameinfo.RootEngineContentFolder);
	m_materialManager = std::make_unique<RD_MaterialLibrary>();
	m_hinput = std::make_unique<EXP_InputHandler>(m_rndr->GetRenderingAPI()->GetWindowingSystem());

	InitPhysics();

	m_PlayingMap = std::make_unique<EXP_MapLoader>(this);
	m_PlayingMap->LoadMap(gameinfo.RootGameContentFolder + gameinfo.StartupMap);

	m_sigClearMatMan = false;
	m_sigLevelFinalCleanup = false;

	EXP_Level* lvl = m_PlayingMap->GetLevelCode();
	if (lvl) {
		lvl->CallStart();
	}

	for (auto act : m_actors) {
		act->Start();
	}
}

void EXP_Game::InitPhysicaSound() {
	m_soundEngine = std::make_unique<PSound>();

	m_soundEngine->GetDevice();
	
	if (!m_soundEngine->initAL()) {
		std::cerr << "Cannot init OpenAL" << std::endl;

		return;
	}

    m_listener = std::make_shared<PS_Listener>(vec3f(), vec3f());
    m_soundEngine->RegisterListener(m_listener.get());
}

void EXP_Game::InitPhysics() {
	m_physicsHandler = std::make_unique<EXP_PhysicsHandler>(vec3f(0.0f, 0.0f, -9.82f), GetRenderer()->GetFrameLimit());
	m_physicsHandler->InitWorld();
}

void EXP_Game::UpdateSound() {
	if (m_listener && m_currentCamera) {	//nullptr security with threads
		m_listener->setPosition(m_currentCamera->GetPosition());
		m_listener->setOrientation(m_currentCamera->GetSubject());
	}

	if (m_soundEngine) {					//nullptr security with threads
		m_soundEngine->mainLoop();
	}
}

void EXP_Game::InitGui() {

}

void EXP_Game::RenderScene() {
	m_rndr->ClearWindow(m_refreshColor);

	vec3f CamLoc = vec3f();
	if (m_currentCamera != nullptr) {
		CamLoc = m_currentCamera->GetLocation();
		m_currentCamera->UpdateCamera();
	}
    
	//Process shadows
	m_rndr->RenderLightsDepth(CamLoc);
	//GBuff
	m_rndr->RenderGbuff(m_currentCamera);
	//Rendering GUI
	m_rndr->RenderGUI_Screen();
	//PostProcessing
	m_rndr->RenderBeauty();

	if (RENDER_DBG && m_currentCamera != nullptr) {
		m_rndr->RenderDbg(m_currentCamera);
	}

	//Process other threads signals... what a terribleness
	ProcessSignals();
}

void EXP_Game::EndFrame() {
	m_rndr->SwapWindow();
}

void EXP_Game::ProcessSignals() {
	if (m_sigClearMatMan) {
		m_materialManager->ClearLibrary();

		m_sigClearMatMan = false;
	}

	if (m_sigLevelFinalCleanup) {
		m_rndr->EmptyFramebufferGarbageCollector();
		m_rndr->EmptyTextureGarbageCollector();

		m_sigLevelFinalCleanup = false;
	}
}

void EXP_Game::ExecCallbacks() {
	UpdateCallbacks();
	UpdateLevel();

	m_hinput->ResetPointer();
}

void EXP_Game::UpdateLevel() {
	if (m_PlayingMap != nullptr) {
		EXP_Level* lvl = m_PlayingMap->GetLevelCode();
		if (lvl) {
			lvl->CallTick();
		}
	}
}

BD_GameInfo EXP_Game::GetGameInfo() {
	return m_gameinfo;
}

RaindropRenderer* EXP_Game::GetRenderer() {
	return m_rndr.get();
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

PSound* EXP_Game::GetSoundEngine() {
	return m_soundEngine.get();
}

void EXP_Game::PlaySimpleSound(std::string ref, float gain) {
	std::string fullref = m_gameinfo.RootGameContentFolder + ref;

	std::thread t([](PSound* engine, std::string file, float gain) {
		engine->playSimpleSound(file, gain);
	}, m_soundEngine.get(), fullref, gain);

	t.detach();
}

void EXP_Game::PlaySound3D(std::string ref, vec3f pos, float gain) {
	std::string fullref = m_gameinfo.RootGameContentFolder + ref;

	std::thread t([](PSound* engine, std::string file, vec3f pos, float gain) {
		engine->playSound3D(file, pos, gain);
	}, m_soundEngine.get(), fullref, pos, gain);

	t.detach();
}

void EXP_Game::RegisterCamera(EXP_Camera* cam) {
	m_currentCamera = cam;
	m_listener->setPosition(cam->GetPosition());
	m_listener->setOrientation(cam->GetSubject());
}

void EXP_Game::RegisterSoundEmitter(EXP_SoundEmitter* sm) {
	m_soundEngine->RegisterEmitter(sm, false);
}

std::string EXP_Game::GetFilePathByRef(std::string ref) {
	return m_gameinfo.RootGameContentFolder + ref;
}

void EXP_Game::UpdatePhysics() {
    if(m_physicsHandler)
        m_physicsHandler->UpdateWorld();
}

EXP_PhysicsHandler* EXP_Game::GetPhysicsHandler() {
	return m_physicsHandler.get();
}

void EXP_Game::RegisterKeyboardCallback(EXP_KeyboardCallback* callback) {
	std::cout << "Registering Keyboard Callback" << std::endl;
	m_hinput->RegisterKeyboardCallback(callback);
}

void EXP_Game::UnregisterKeyboardCallback(EXP_KeyboardCallback* kbcllbck, bool nodelete) {
	std::cout << "Unregistering Keyboard Callback" << std::endl;
	m_hinput->UnregisterKeyboardCallback(kbcllbck);

	if (!nodelete)
		delete kbcllbck;
}

void EXP_Game::UpdateCallbacks() {
	m_hinput->UpdateKeyboardInput();
	m_hinput->UpdateMouseInput();

	for (auto act : m_actors) {
		act->Tick();
	}
}

void EXP_Game::RegisterActor(EXP_Actor* act) {
	std::cout << "Registering new actor." << std::endl;

	m_actors.push_back(act);
}

void EXP_Game::UnregisterMesh(RD_Mesh* mesh, bool nodelete) {
	std::cout << "Unregistering mesh." << std::endl;
	m_rndr->UnregisterMesh(mesh);

	if (!nodelete)
		delete mesh;
}

void EXP_Game::UnregisterDirLight(RD_DirLight* dlight, bool nodelete) {
	std::cout << "Unregistering DirLight." << std::endl;
	m_rndr->UnregisterDirLight(dlight);

	if (!nodelete)
		delete dlight;
}

void EXP_Game::UnregisterPointLight(RD_PointLight* plight, bool nodelete) {
	std::cout << "Unregistering PointLight" << std::endl;
	m_rndr->UnregisterPointLight(plight);

	if (!nodelete)
		delete plight;
}

void EXP_Game::UnregisterActor(EXP_Actor* actor, bool nodelete) {
	std::cout << "Unregistering Actor" << std::endl;
	int index = GetElemIndex<EXP_Actor*>(m_actors, actor);

	if (index != -1) {
		m_actors.erase(m_actors.begin() + index);
		actor->Unregister();

		if(!nodelete)
			delete actor;
	}
	else {
		std::cerr << "ERROR: Element does not exists" << std::endl;
	}
}

EXP_HotLoad* EXP_Game::GetGameLib() {
	return m_GameLib.get();
}

void EXP_Game::UnloadCurrentMap() {
	for (int i = 0; i < m_actors.size(); i++) {
		if (m_actors[i]) {
			m_actors[i]->Unregister();
			delete m_actors[i];
		}
	}

	if(!m_actors.empty())
		m_actors.clear();

	m_PlayingMap->UnloadMap();
	m_hinput->UnregisterAllCallbacks();

	m_rndr->UnregisterAllDirLights();
	m_rndr->UnregisterAllPointLights();
	m_rndr->UnregisterAllMeshes();
	
	//Did this terribleness because openGL need to delete buffers in the same thread
	//as the context.
	m_sigClearMatMan = true;
	m_sigLevelFinalCleanup = true;
}

void EXP_Game::LoadMap(std::string map) {
	UnloadCurrentMap();
	m_PlayingMap->LoadMap(m_gameinfo.RootGameContentFolder + map);

	m_PlayingMap->GetLevelCode()->OnStart();
	for (auto actor : m_actors) {
		actor->Start();
	}
}

EXP_InputHandler* EXP_Game::GetInputHandler() {
	return m_hinput.get();
}

bool EXP_Game::CheckErrors() {
	if (m_rndr->GetErrorFlag() && m_error_flag) {
		return true;
	}

	return false;
}

void EXP_Game::RegisterMouseButtonCallback(EXP_MouseButtonCallback* cllbck) {
	std::cout << "Registering Mouse Button Callback" << std::endl;
	m_hinput->RegisterMouseButtonCallback(cllbck);
}

void EXP_Game::UnregisterMouseButtonCallback(EXP_MouseButtonCallback* cllbck, bool nodelete) {
	std::cout << "Unregistering Mouse Button Callback" << std::endl;
	m_hinput->UnregisterMouseButtonCallback(cllbck);

	if (nodelete)
		delete cllbck;
}

RD_ShaderMaterial* EXP_Game::GetShaderByFileRef(std::string ref) {
	std::string absPath = m_gameinfo.RootGameContentFolder + ref;

	return m_rndr->FetchShaderFromFile(absPath);
}