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

EXP_Game::EXP_Game(const EXP_GameInfo& gameinfo, const vec3f& refreshColor) {
	m_currentCamera = nullptr;
	m_error_flag = false;

#ifdef _DEBUG
	m_toggle_debug_rendering = true;
#else
	m_toggle_debug_rendering = false;
#endif //_DEBUG

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

	m_close_override = false;
}

EXP_Game::EXP_Game(const std::string& gameinfo) {
	EXP_GameInfo gi = CreateGameInfoFromJSON(gameinfo);
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

	m_close_override = false;
}

EXP_Game::~EXP_Game() {
	UnloadCurrentMap();
	m_soundEngine->shutdownAL();
}

void EXP_Game::ToggleDebugRendering(bool toggle) {
	m_toggle_debug_rendering = toggle;
}

EXP_GameInfo EXP_Game::CreateGameInfoFromJSON(const std::string& file) {
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

	EXP_Resolution res = { TargetRoot["GameBaseResolution"][0].asInt(), TargetRoot["GameBaseResolution"][1].asInt()};

	Pipeline pline;
	if (TargetRoot["RenderingPipeline"].asString() == "pbr") {
		std::cout << "Loading PBR engine." << std::endl;
		pline = Pipeline::PBR_ENGINE;
	}
	else if(TargetRoot["RenderingPipeline"].asString() == "lambert") {
		std::cout << "Loading Lambert engine." << std::endl;
		pline = Pipeline::LAMBERT_ENGINE;
	}
	else {
		std::cout << "Loading Lambert engine." << std::endl;
		pline = Pipeline::LAMBERT_ENGINE;
	}

	API api;
	if (TargetRoot["RenderingAPI"].asString() == "OGL3") {
		api = API::OPENGL3;
	}
	else if (TargetRoot["RenderingAPI"].asString() == "OGL4") {
		api = API::OPENGL4;
	}
	else {
		api = API::OPENGL4;
	}

	std::string startupMap = TargetRoot["StartupMap"].asString();

	EXP_GameInfo gi = {};
	gi.GameName = gameName;
	gi.RootEngineContentFolder = engineFolder;
	gi.RootGameContentFolder = contentFolder;
	gi.GameBaseResolution = res;
	gi.GameLib = GameLib;
	gi.StartupMap = startupMap;
	gi.RenderingPipeline = pline;
	gi.RenderingAPI = api;

	return gi;
}

void EXP_Game::InitGame(const vec3f& refreshColor, const EXP_GameInfo& gameinfo) {
	m_res = gameinfo.GameBaseResolution;
	m_refreshColor = refreshColor;
	m_gameName = gameinfo.GameName;
	m_gameinfo = gameinfo;

	m_rndr = std::make_shared<RaindropRenderer>(m_res.x, m_res.y,
												gameinfo.GameName,
												gameinfo.RenderingAPI, gameinfo.RenderingPipeline,
												60,
												false,
												gameinfo.RootEngineContentFolder);

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

	for (auto* act : m_actors) {
		act->OnStart();
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

void EXP_Game::UpdateSound() const {
	if (m_listener && m_currentCamera) {	//nullptr security with threads
		m_listener->setPosition(m_currentCamera->GetPosition());
		m_listener->setOrientation(m_currentCamera->GetSubject());
	}

	if (m_soundEngine) {					//nullptr security with threads
		m_soundEngine->mainLoop();
	}
}

void EXP_Game::RenderScene() {
	m_rndr->ClearWindow(m_refreshColor);

	if (!m_currentCamera)
		return;

	const vec3f CamLoc = m_currentCamera->GetLocation();
    
	//Process shadows
	m_rndr->RenderLightsDepth(CamLoc);
	//GBuff
	m_rndr->RenderGbuff(m_currentCamera);

	//PostProcessing
	m_rndr->RenderBeauty();

	if (m_toggle_debug_rendering) {
		m_rndr->RenderDbg(m_currentCamera);
	}

	//Process other threads signals... what a terribleness
	ProcessSignals();
}

void EXP_Game::EndFrame() const {
	m_rndr->SwapWindow();
}

void EXP_Game::ProcessSignals() {
	if (m_sigClearMatMan) {
		m_rndr->GetMaterialLibrary()->ClearLibrary();

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

void EXP_Game::UpdateLevel() const {
	if (m_PlayingMap != nullptr) {
		EXP_Level* lvl = m_PlayingMap->GetLevelCode();
		if (lvl) {
			lvl->CallTick();
		}
	}

	m_rndr->UpdateParticles();

	if (m_rndr->DoNeedCamUpdate() && (m_currentCamera != nullptr)) {
		m_currentCamera->UpdateProj();
	}
}

EXP_GameInfo EXP_Game::GetGameInfo() const {
	return m_gameinfo;
}

RaindropRenderer* EXP_Game::GetRenderer() const {
	return m_rndr.get();
}

void EXP_Game::RegisterMesh(RD_Mesh* mesh) const {
	m_rndr->RegisterMesh(mesh);
}

void EXP_Game::RegisterPointLight(RD_PointLight* ptlight) const {
	m_rndr->AppendLight(ptlight);
}

vec3f EXP_Game::GetRefreshColor() const {
	return m_refreshColor;
}

PSound* EXP_Game::GetSoundEngine() const {
	return m_soundEngine.get();
}

void EXP_Game::PlaySimpleSound(const std::string& ref, float gain) const {
	std::string fullref = m_gameinfo.RootGameContentFolder + ref;

	std::thread t([](PSound* engine, const std::string& file, const float tgain) {
		engine->playSimpleSound(file, tgain);
	}, m_soundEngine.get(), std::ref(fullref), gain);

	t.detach();
}

void EXP_Game::PlaySound3D(const std::string& ref, const vec3f& pos, float gain) const {
	std::string fullref = m_gameinfo.RootGameContentFolder + ref;

	std::thread t([](PSound* engine, const std::string& file, const vec3f& tpos, const float tgain) {
		engine->playSound3D(file, tpos, tgain);
	}, m_soundEngine.get(), std::ref(fullref), std::ref(pos), gain);

	t.detach();
}

void EXP_Game::RegisterCamera(EXP_Camera* cam) {
	m_currentCamera = cam;
	m_listener->setPosition(cam->GetPosition());
	m_listener->setOrientation(cam->GetSubject());
}

void EXP_Game::RegisterSoundEmitter(EXP_SoundEmitter* sm) const {
	m_soundEngine->RegisterEmitter(sm, false);
}

std::string EXP_Game::GetFilePathByRef(const std::string& ref) const {
	return m_gameinfo.RootGameContentFolder + ref;
}

void EXP_Game::UpdatePhysics() const {
    if(m_physicsHandler)
        m_physicsHandler->UpdateWorld();
}

EXP_PhysicsHandler* EXP_Game::GetPhysicsHandler() const {
	return m_physicsHandler.get();
}

void EXP_Game::RegisterKeyboardCallback(EXP_KeyboardCallback* callback) const {
	std::cout << "Registering Keyboard Callback" << std::endl;
	m_hinput->RegisterKeyboardCallback(callback);
}

void EXP_Game::UnregisterKeyboardCallback(EXP_KeyboardCallback* kbcllbck, bool nodelete) const {
	std::cout << "Unregistering Keyboard Callback" << std::endl;
	m_hinput->UnregisterKeyboardCallback(kbcllbck);

	if (!nodelete)
		delete kbcllbck;
}

void EXP_Game::UpdateCallbacks() {
	m_hinput->UpdateKeyboardInput();
	m_hinput->UpdateMouseInput();
}

void EXP_Game::TickActors() {
	for (auto* act : m_actors) {
		act->OnTick();
	}
}

void EXP_Game::RegisterActor(EXP_Actor* act) {
	std::cout << "Registering new actor." << std::endl;

	m_actors.push_back(act);
}

void EXP_Game::UnregisterMesh(RD_Mesh* mesh, bool nodelete) const {
	std::cout << "Unregistering mesh." << std::endl;
	m_rndr->UnregisterMesh(mesh);

	if (!nodelete)
		delete mesh;
}

void EXP_Game::UnregisterDirLight(RD_DirLight* dlight, bool nodelete) const {
	std::cout << "Unregistering DirLight." << std::endl;
	m_rndr->UnregisterDirLight(dlight);

	if (!nodelete)
		delete dlight;
}

void EXP_Game::UnregisterPointLight(RD_PointLight* plight, bool nodelete) const {
	std::cout << "Unregistering PointLight" << std::endl;
	m_rndr->UnregisterPointLight(plight);

	if (!nodelete)
		delete plight;
}

void EXP_Game::UnregisterActor(EXP_Actor* actor, bool nodelete) {
	std::cout << "Unregistering Actor" << std::endl;
	const int index = GetElemIndex<EXP_Actor*>(m_actors, actor);

	if (index != -1) {
		m_actors.erase(m_actors.begin() + index);
		actor->OnUnregister();

		if(!nodelete)
			delete actor;
	}
	else {
		std::cerr << "ERROR: Element does not exists" << std::endl;
	}
}

EXP_HotLoad* EXP_Game::GetGameLib() const {
	return m_GameLib.get();
}

void EXP_Game::UnloadCurrentMap() {
	for (auto* actor : m_actors) {
		if (actor) {
			actor->OnUnregister();
			delete actor;
		}
	}

	if(!m_actors.empty())
		m_actors.clear();

	m_PlayingMap->UnloadMap();
	m_hinput->UnregisterAllCallbacks();

	m_rndr->UnregisterAllDirLights();
	m_rndr->UnregisterAllPointLights();
	m_rndr->UnregisterAllMeshes();
	m_rndr->UnregisterAllParticleEmitters();
	
	m_rndr->GetMaterialLibrary()->ClearLibrary();

	//m_physicsHandler->PurgeControllers();
    
	//Did this terribleness because openGL need to delete buffers in the same thread
	//as the context.
	m_sigLevelFinalCleanup = true;
}

void EXP_Game::LoadMap(const std::string& map) {
	UnloadCurrentMap();
    
    if(!m_rndr->GetMaterialLibrary()->DoMaterialExists("text")) {
        //Essential Material
        RD_ShaderLoader* ld = m_rndr->GetRenderingAPI()->CreateShader();
        ld->compileShaderFromFile(
            m_gameinfo.RootEngineContentFolder + "/Shaders/glsl/TextRender.vert",
            m_gameinfo.RootEngineContentFolder + "/Shaders/glsl/TextRender.frag"
        );
        m_rndr->GetMaterialLibrary()->AddMaterialToLib(new RD_ShaderMaterial(ld, GetRenderer()), "text");

    }

	if (!m_rndr->GetMaterialLibrary()->DoMaterialExists(m_gameinfo.RootEngineContentFolder + "/Materials/default_mat.exmtl")) {
		m_rndr->FetchShaderFromFile(m_gameinfo.RootEngineContentFolder + "/Materials/default_mat.exmtl");
	}
    
	m_PlayingMap->LoadMap(m_gameinfo.RootGameContentFolder + map);

	m_PlayingMap->GetLevelCode()->OnStart();
	for (auto* actor : m_actors) {
		actor->OnStart();
	}
}

EXP_InputHandler* EXP_Game::GetInputHandler() const {
	return m_hinput.get();
}

bool EXP_Game::CheckErrors() const {
	if (m_rndr->GetErrorFlag() && m_error_flag) {
		return true;
	}

	return false;
}

void EXP_Game::RegisterMouseButtonCallback(EXP_MouseButtonCallback* cllbck) const {
	std::cout << "Registering Mouse Button Callback" << std::endl;
	m_hinput->RegisterMouseButtonCallback(cllbck);
}

void EXP_Game::UnregisterMouseButtonCallback(EXP_MouseButtonCallback* cllbck, bool nodelete) const {
	std::cout << "Unregistering Mouse Button Callback" << std::endl;
	m_hinput->UnregisterMouseButtonCallback(cllbck);

	if (nodelete)
		delete cllbck;
}

RD_ShaderMaterial* EXP_Game::GetShaderByFileRef(const std::string& ref) const {
	const std::string absPath = m_gameinfo.RootGameContentFolder + ref;

	return m_rndr->FetchShaderFromFile(absPath, m_gameinfo.RootGameContentFolder);
}

EXP_MapLoader* EXP_Game::GetCurrentMap() const {
	return m_PlayingMap.get();
}

RD_ShaderMaterial* EXP_Game::GetShaderByFileRefInstanced(const std::string& ref) {
	const std::string absPath = m_gameinfo.RootGameContentFolder + ref;
	if (!std::filesystem::exists(absPath)) {
		std::cerr << "Shader file " << ref << " does not exist." << std::endl;
		dispErrorMessageBox(StrToWStr("Shader file " + ref + " does not exists"));
		return nullptr;
	}

	if (m_rndr->GetMaterialLibrary()->DoMaterialExists(ref)) {
		return m_rndr->GetMaterialLibrary()->GetMaterialByName(ref);
	}

	std::string folder, vertEXT;
	bool legacy = false;
	switch (m_rndr->GetRenderingAPI()->GetAPIType())
	{
	case OPENGL3:
		folder = "/glsl/gl3/";
		vertEXT = "vert";
		legacy = true;
		break;
	case OPENGL4:
		folder = "/glsl/gl4/";
		vertEXT = "vert";
		break;
	case VULKAN:
		break;
	case DIRECTX:
		break;
	default:
		break;
	}

	BD_MatCustomShaderRead mr(absPath);
	const std::string frag = mr.GetShaderCode(legacy);
	const std::string vert = getFileData(m_gameinfo.RootEngineContentFolder + "/Shaders/" + folder + "/Particles." + vertEXT);

	RD_ShaderLoader* shd = m_rndr->GetRenderingAPI()->CreateShader();
	shd->CompileShaderFromCode(vert, frag);

	RD_ShaderMaterial* sm = new RD_ShaderMaterial(shd, GetRenderer());

	for (int i = 0; i < mr.GetTextureCount(); i++) {
		RD_Texture* t = m_rndr->GetRenderingAPI()->CreateTexture();
		t->LoadTexture(mr.GetTexturePath(i));
		sm->AddTexture(mr.GetTextureParamName(i), t);
	}
	sm->MakeSSBO();

	mr.CloseFile();

	m_rndr->GetMaterialLibrary()->AddMaterialToLib(sm, ref);
	return sm;
}

void EXP_Game::Close() {
	m_close_override = true;
}

bool EXP_Game::GetCloseOverride() {
	return m_close_override;
}