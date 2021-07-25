#include <iostream>
#include <EXP_Game.h>
#include <EXP_MapLoader.h>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "EditorRegistry.h"
#include "EditorGUI.h"

int main(int argc, char* argv[]) {
	//EXPGE Renderer
	EXP_GameInfo gi;
	gi.GameBaseResolution = { 1280, 720 };
	gi.GameName = "Expansion Game Engine - Studio";
	gi.RenderingAPI = API::OPENGL;
	gi.RenderingPipeline = Pipeline::PBR_ENGINE;
	gi.RootEngineContentFolder = "Engine";
	gi.RootGameContentFolder = "studio";
	gi.StartupMap = "/map/startup.json";
#ifdef _DEBUG
	gi.GameLib = "../Debug/EXPGE_StudioDLL.dll";
#else
	gi.GameLib = "EXPGE_StudioDLL.dll";
#endif

	EXP_Game* game = new EXP_Game(gi, vec3f());
	RD_WindowingSystemGLFW* winsys = reinterpret_cast<RD_WindowingSystemGLFW*>(game->GetRenderer()->GetRenderingAPI()->GetWindowingSystem());

	RD_MaterialLibrary* matlib = game->GetRenderer()->GetMaterialLibrary();

	//Editor
	std::string projectPath = "C:/";
	std::string contentPath;
	for (int i = 0; i < argc; i++) {
		std::string arg(argv[i]);
		if (arg == "-p") {
			if (i + 1 > argc) {
				dispErrorMessageBox(StrToWStr("No work directory specified."));
				exit(-1);
			}

			if (!std::filesystem::exists(std::string(argv[i + 1]))) {
				dispErrorMessageBox(StrToWStr("Invalid work directory specified."));
				exit(-1);
			}

			if (!std::filesystem::is_directory(std::string(argv[i + 1]))) {
				dispErrorMessageBox(StrToWStr("The specified work directory is not a directory."));
				exit(-1);
			}

			projectPath = std::string(argv[i + 1]);
			std::cout << "Using project path : " << projectPath << std::endl;
		}

		if (arg == "-c") {
			if (i + 1 > argc) {
				dispErrorMessageBox(StrToWStr("No content directory specified."));
				exit(-1);
			}

			contentPath = argv[i + 1];
		}
	}

	if ((!std::filesystem::exists(projectPath + contentPath)) || (!std::filesystem::is_directory(projectPath + contentPath))) {
		dispErrorMessageBox(StrToWStr("Invalid content directory specified."));
		exit(-1);
	}

	std::cout << "Using content path : " << contentPath << std::endl;

	EditorGUI* editgui = new EditorGUI(game, projectPath, contentPath);

	//ImGui
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	ImGui::StyleColorsDark();
	ImGui::GetStyle().WindowRounding = 3.0f;

	ImGui_ImplGlfw_InitForOpenGL(winsys->GetWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 450");

	ImFont* fnt = io.Fonts->AddFontFromFileTTF("studio/font/open_sans.ttf", 17.0f);
	io.Fonts->Fonts[0] = fnt;

	while (!game->GetRenderer()->WantToClose()) {
		game->RenderScene();
		
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		
		editgui->RenderEditorGUI();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		game->ExecCallbacks();

		game->EndFrame();
	}

	ImGui::DestroyContext();

	delete editgui;
	delete game;
	return 0;
}