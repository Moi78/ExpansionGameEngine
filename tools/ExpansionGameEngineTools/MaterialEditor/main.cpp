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

	RD_ShaderMaterial* mat = game->GetRenderer()->FetchShaderFromFile("mat_editor/def_shader.exmtl");
	EXP_StaticMesh* msh = new EXP_StaticMesh(game,
											 mat,
											 "/meshes/sphere",
											 vec3f(),
											 vec3f(),
											 vec3f(1.0f, 1.0f, 1.0f));

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

				ImGui::Columns(2, nullptr, false);

				if (ImGui::Button("Reload Material", ImVec2(ImGui::GetColumnWidth(), 20.0f))) {
					if (matlib->DoMaterialExists("mat_editor/def_shader.exmtl")) {
						//delete matlib->GetMaterialByName("mat_editor/def_shader.exmtl");
						matlib->RemoveMaterialFromLib("mat_editor/def_shader.exmtl");
					}

					RD_ShaderMaterial* mat2 = game->GetRenderer()->FetchShaderFromFile("mat_editor/def_shader.exmtl");
					msh->SetMaterial(mat2);
				}

				ImGui::NextColumn();

				if (ImGui::Button("Compile Material", ImVec2(ImGui::GetColumnWidth(), 20.0f))) {
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
					msh->SetMaterial(shmat);
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