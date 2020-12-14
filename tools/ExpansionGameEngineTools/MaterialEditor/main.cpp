#include <iostream>

#include <EXP_Game.h>
#include <EXP_MapLoader.h>
#include <EXP_StaticMesh.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imnodes.h"

#include "Node_Editor.h"

#include <glad/glad.h>

float lightDirX = 1.0f;
float lightDirY = -0.8f;
float lightDirZ = -0.6f;
float lightBrtn = 5.0f;

RD_ShaderMaterial* CompileMat(EXP_Game* game, Node_Editor* editor) {
	std::string FragCode = editor->EvalNodes();

	std::string VertCode = "";
	std::ifstream vertFile;
	vertFile.open("Engine/ShaderFragments/VertShader.vert", std::ios::beg);
	while (!vertFile.eof()) {
		char buf[100];
		vertFile.getline(buf, 100);
		VertCode += std::string(buf) + "\n";
	}

	RD_ShaderLoader* sl = game->GetRenderer()->GetRenderingAPI()->CreateShader();
	sl->CompileShaderFromCode(VertCode, FragCode);
	RD_ShaderMaterial* shmat = new RD_ShaderMaterial(sl);
	
	for (int i = 0; i < editor->GetTextureCount(); i++) {
		RD_Texture* tex = game->GetRenderer()->GetRenderingAPI()->CreateTexture();
		tex->LoadTexture(editor->GetTextureRefByIndex(i).first);

		shmat->AddTexture(editor->GetTextureRefByIndex(i).second, tex);
	}

	return shmat;
}

int main(int argc, char* argv[]) {
	//EXPGE Renderer
	EXP_GameInfo gi;
	gi.GameBaseResolution = { 1280, 720 };
	gi.GameName = "Material Editor";
	gi.RenderingAPI = API::OPENGL;
	gi.RenderingPipeline = Pipeline::PBR_ENGINE;
	gi.RootEngineContentFolder = "Engine";
	gi.RootGameContentFolder = "mat_editor";
	gi.StartupMap = "/map/startup.json";
	gi.GameLib = "../Debug/MatEditorDLL.dll";

	EXP_Game* game = new EXP_Game(gi, vec3f());
	RD_WindowingSystemGLFW* winsys = reinterpret_cast<RD_WindowingSystemGLFW*>(game->GetRenderer()->GetRenderingAPI()->GetWindowingSystem());

	RD_MaterialLibrary* matlib = game->GetRenderer()->GetMaterialLibrary();

	//ImGui
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	imnodes::Initialize();

	ImGuiIO& io = ImGui::GetIO();
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(winsys->GetWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 410");

	//Node Editing
	Node_Editor* editor = new Node_Editor(game);

	std::vector<std::pair<int, int>> links;

	ShaderNode* snode = new ShaderNode(0, 0);
	editor->AddNode(snode);

	ShaderInputs* inode = new ShaderInputs(1, 8);
	editor->AddNode(inode);

	RD_ShaderMaterial* mat = CompileMat(game, editor);
	EXP_StaticMesh* msh = new EXP_StaticMesh(game,
		mat,
		"/meshes/sphere",
		vec3f(),
		vec3f(),
		vec3f(1.0f, 1.0f, 1.0f));

	while (!game->GetRenderer()->WantToClose()) {
		game->RenderScene();

		int w = game->GetRenderer()->getWindowWidth();
		int h = game->GetRenderer()->getWindowHeigh();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		{
			ImGui::SetNextWindowPos(ImVec2(w / 2, 0));
			ImGui::SetNextWindowSize(ImVec2(w / 2, h));
			ImGui::Begin("Material", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

			{
				ImGui::BeginChild("Light Configaration", ImVec2((w / 2) - 30, 105), true);

				if (ImGui::SliderFloat("Light direction X", &lightDirX, -1.0f, 1.0f)) {
					EXP_DirLight* dlight = game->GetCurrentMap()->GetDirLightByName("sun");
					if (dlight) {
						dlight->SetLightDir(vec3f(lightDirX, lightDirY, lightDirZ));
					}
				}

				if (ImGui::SliderFloat("Light direction Y", &lightDirY, -1.0f, 1.0f)) {
					EXP_DirLight* dlight = game->GetCurrentMap()->GetDirLightByName("sun");
					if (dlight) {
						dlight->SetLightDir(vec3f(lightDirX, lightDirY, lightDirZ));
					}
				}

				if (ImGui::SliderFloat("Light direction Z", &lightDirZ, -1.0f, 1.0f)) {
					EXP_DirLight* dlight = game->GetCurrentMap()->GetDirLightByName("sun");
					if (dlight) {
						dlight->SetLightDir(vec3f(lightDirX, lightDirY, lightDirZ));
					}
				}

				if (ImGui::InputFloat("Light Brightness", &lightBrtn, 0.1f, 1.0f)) {
					EXP_DirLight* dlight = game->GetCurrentMap()->GetDirLightByName("sun");
					if (dlight) {
						dlight->SetLightBrightness(lightBrtn);
					}
				}

				ImGui::EndChild();
			}

			{
				ImGui::BeginChild("Material Configuration", ImVec2((w / 2) - 30, 100), true);

				if (ImGui::Button("Compile Material", ImVec2(ImGui::GetColumnWidth(), 20.0f))) {
					delete mat;
					mat = CompileMat(game, editor);
					msh->SetMaterial(mat);
				}

				ImGui::EndChild();
			}

			{
				ImGui::BeginChild("Node Graph", ImVec2((w / 2) - 30, 800), true);
				imnodes::BeginNodeEditor();

				editor->RenderNodes();

				imnodes::EndNodeEditor();

				editor->MakeLinks();

				ImGui::EndChild();
			}
		}

		game->ExecCallbacks();

		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		game->EndFrame();
	}

	ImGui::DestroyContext();
	imnodes::Shutdown();

	delete editor;
	delete game;

	return 0;
}