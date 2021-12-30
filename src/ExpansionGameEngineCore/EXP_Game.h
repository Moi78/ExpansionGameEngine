#pragma once

#ifndef _EXP_GAME_H__
#define _EXP_GAME_H__

#ifdef _WIN32
	#ifdef EXPANSIONGAMEENGINECORE_EXPORTS
		#define EXPGE_API __declspec(dllexport)
	#else
		#define EXPGE_API __declspec(dllimport)
	#endif
	
    #define EXECPLATFORM "windows"
#else

    #define EXPGE_API
    #define EXECPLATFORM "linux"
#endif //_WIN32

#ifdef UBUNTU
	#include <jsoncpp/json/json.h>
#else
	#include <json/json.h>
#endif

#ifdef _DEBUG
#define RENDER_DBG true
#else
#define RENDER_DBG false
#endif

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <thread>

#include <thread>
#include <mutex>

#include <RaindropRenderer.h>
#include <RD_Mesh.h>
#include <RD_PointLight.h>
#include <RD_Texture.h>
#include <RD_MaterialLibrary.h>
#include <RD_RenderingAPI_GL.h>

#include <BD_MatRW.h>

#include <PhysicaSound.h>
#include <PS_Listener.h>

#include <vec3.h>

#include "EXP_Structs.h"

//Forward Declaration
class EXP_Actor;
class EXP_Camera;
class EXP_SoundEmitter;
class EXP_RigidBody;
class EXP_PhysicsHandler;
class EXP_KeyboardCallback;
class EXP_HotLoad;
class EXP_MapLoader;
class EXP_Level;
class EXP_InputHandler;
class EXP_MouseButtonCallback;

class EXPGE_API EXP_Game
{
public:
	EXP_Game(const EXP_GameInfo& gameinfo, const vec3f& refreshColor);
	EXP_Game(const std::string& gameInfoJSON);

	~EXP_Game();
	
	EXP_GameInfo GetGameInfo() const;

	void Close();
	bool GetCloseOverride();
    
	//For internal usage only
	void RegisterMesh(RD_Mesh*) const;
	void RegisterPointLight(RD_PointLight*) const;
	void RegisterCamera(EXP_Camera*);
	void RegisterKeyboardCallback(EXP_KeyboardCallback*) const;
	void RegisterActor(EXP_Actor*);
	void RegisterMouseButtonCallback(EXP_MouseButtonCallback*) const;

	void UnregisterKeyboardCallback(EXP_KeyboardCallback*, bool nodelete = false) const;
	void UnregisterActor(EXP_Actor*, bool nodelete = false);
	void UnregisterMesh(RD_Mesh*, bool nodelete = false) const;
	void UnregisterDirLight(RD_DirLight*, bool nodelete = false) const;
	void UnregisterPointLight(RD_PointLight*, bool nodelete = false) const;
	void UnregisterMouseButtonCallback(EXP_MouseButtonCallback*, bool nodelete = false) const;

	EXP_HotLoad* GetGameLib() const;

	EXP_InputHandler* GetInputHandler() const;

	void UnloadCurrentMap();
	void LoadMap(const std::string& map);
	EXP_MapLoader* GetCurrentMap() const;

	//Rendering
	RaindropRenderer* GetRenderer() const;
	vec3f GetRefreshColor() const;

	void RenderScene();
	void ExecCallbacks();
	void EndFrame() const;
	
	void UpdateCallbacks();
	void UpdateLevel() const;
	void ProcessSignals();
	void TickActors();

	void ToggleDebugRendering(bool toggle);

	//Sounds
	void UpdateSound() const;
	PSound* GetSoundEngine() const;
	void RegisterSoundEmitter(EXP_SoundEmitter*) const;
	void PlaySimpleSound(const std::string& ref, float gain) const;
	void PlaySound3D(const std::string& ref, const vec3f& pos, float gain) const;

	//Files
	std::string GetFilePathByRef(const std::string& ref) const;
	RD_ShaderMaterial* GetShaderByFileRef(const std::string& ref) const;
	RD_ShaderMaterial* GetShaderByFileRefInstanced(const std::string& ref);

	//Physics
	void UpdatePhysics() const;
	EXP_PhysicsHandler* GetPhysicsHandler() const;

	bool CheckErrors() const;

private:
	void InitGame(const vec3f& refreshColor, const EXP_GameInfo& gameinfo);
	void InitPhysicaSound();
	void InitPhysics();
	EXP_GameInfo CreateGameInfoFromJSON(const std::string& gameInfo);

	std::unique_ptr<EXP_MapLoader> m_PlayingMap;

	std::shared_ptr<RaindropRenderer> m_rndr;

	vec3f m_refreshColor;
	std::string m_gameName;

	std::unique_ptr<PSound> m_soundEngine;
	std::shared_ptr<PS_Listener> m_listener;

	EXP_Resolution m_res;
	EXP_GameInfo m_gameinfo;

	EXP_Camera* m_currentCamera;

	std::unique_ptr<EXP_HotLoad> m_GameLib;

	std::unique_ptr<EXP_InputHandler> m_hinput;

	//Signals
	bool m_sigClearMatMan;
	bool m_sigLevelFinalCleanup;
	bool m_error_flag;
	//Physics
	std::unique_ptr<EXP_PhysicsHandler> m_physicsHandler;
	//Actors
	std::vector<EXP_Actor*> m_actors;

	bool m_close_override;
	bool m_toggle_debug_rendering;
};

#endif //_EXP_GAME_H__
