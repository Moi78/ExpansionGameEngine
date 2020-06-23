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

#include <BD_StructMan.h>
#include <BD_MatRW.h>

#include <PhysicaSound.h>
#include <PS_Listener.h>

#include <vec3.h>

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

class EXPGE_API EXP_Game
{
public:
	EXP_Game(BD_GameInfo gameinfo, vec3f refreshColor);
	EXP_Game(std::string gameInfoJSON);

	~EXP_Game();
	
	BD_GameInfo GetGameInfo();
    
	//For internal usage only
	void RegisterMesh(RD_Mesh*);
	void RegisterPointLight(RD_PointLight*);
	void RegisterCamera(EXP_Camera*);
	void RegisterKeyboardCallback(EXP_KeyboardCallback*);
	void RegisterActor(EXP_Actor*);

	void UnregisterKeyboardCallback(EXP_KeyboardCallback*);
	void UnregisterActor(EXP_Actor*);
	void UnregisterMesh(RD_Mesh*);
	void UnregisterDirLight(RD_DirLight*);
	void UnregisterPointLight(RD_PointLight*);

	EXP_HotLoad* GetGameLib();

	EXP_InputHandler* GetInputHandler();

	void UnloadCurrentMap();
	void LoadMap(std::string map);

	//Rendering
	RaindropRenderer* GetRenderer();
	vec3f GetRefreshColor();

	void RenderScene();
	void ExecCallbacks();
	void EndFrame();
	
	void UpdateCallbacks();
	void UpdateLevel();
	void ProcessSignals();

	//Sounds
	void UpdateSound();
	PSound* GetSoundEngine();
	void RegisterSoundEmitter(EXP_SoundEmitter*);
	void PlaySimpleSound(std::string ref, float gain);
	void PlaySound3D(std::string ref, vec3f pos, float gain);

	std::string GetFilePathByRef(std::string ref);

	//Physics
	void UpdatePhysics();
	EXP_PhysicsHandler* GetPhysicsHandler();

private:
	void InitGame(vec3f refreshColor, BD_GameInfo gameinfo);
	void InitPhysicaSound();
	void InitGui();
	void InitPhysics();
	BD_GameInfo CreateGameInfoFromJSON(std::string gameInfo);

	std::unique_ptr<EXP_MapLoader> m_PlayingMap;

	std::shared_ptr<RaindropRenderer> m_rndr;
	std::unique_ptr<RD_MaterialLibrary> m_materialManager;

	vec3f m_refreshColor;
	std::string m_gameName;

	std::unique_ptr<PSound> m_soundEngine;
	std::shared_ptr<PS_Listener> m_listener;

	BD_Resolution m_res;
	BD_GameInfo m_gameinfo;

	EXP_Camera* m_currentCamera;

	std::unique_ptr<EXP_HotLoad> m_GameLib;

	std::unique_ptr<EXP_InputHandler> m_hinput;

	//Signals
	bool m_sigClearMatMan;
	bool m_sigLevelFinalCleanup;
	//Physics
	std::unique_ptr<EXP_PhysicsHandler> m_physicsHandler;
	//Actors
	std::vector<EXP_Actor*> m_actors;
};

#endif //_EXP_GAME_H__
