#include <iostream>

#include <EXP_Game.h>
#include <EXP_MapLoader.h>
#include <EXP_StaticMesh.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imnodes.h"

#include "Node_Editor.h"
#include "Filebrowser.h"

#include <glad/glad.h>

float lightDirX = 1.0f;
float lightDirY = -0.8f;
float lightDirZ = -0.6f;
float lightBrtn = 5.0f;

void CompileMat(EXP_Game* game, Node_Editor* editor, RD_ShaderMaterial* mat) {
	std::string FragCode = editor->EvalNodes();

	std::string VertCode = "";
	std::ifstream vertFile;
	vertFile.open("Engine/Shaders/glsl/Gshad.vert", std::ios::beg);
	while (!vertFile.eof()) {
		char buf[100];
		vertFile.getline(buf, 100);
		VertCode += std::string(buf) + "\n";
	}

	RD_ShaderLoader* sl = game->GetRenderer()->GetRenderingAPI()->CreateShader();
	sl->CompileShaderFromCode(VertCode, FragCode);
	
	mat->PurgeTextures();
	for (int i = 0; i < editor->GetTextureCount(); i++) {
		RD_Texture* tex = game->GetRenderer()->GetRenderingAPI()->CreateTexture();
		tex->LoadTexture(editor->GetTextureRefByIndex(i).first);

		mat->AddTexture(editor->GetTextureRefByIndex(i).second, tex);
	}

	mat->SetShader(sl, false);
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
#ifdef _DEBUG
	gi.GameLib = "../Debug/MatEditorDLL.dll";
#else
	gi.GameLib = "MatEditorDLL.dll";
#endif
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
	ImGui_ImplOpenGL3_Init("#version 450");

	std::string projRoot = "C:/";

	std::vector<std::string> args;
	for (int i = 0; i < argc; i++) {
		args.push_back(argv[i]);
	}

	//Arg Parsing I
	std::string contentPath;
	for (int i = 0; i < args.size(); i++) {
		if (args[i] == "-p") {
			if (i + 1 < args.size()) {
				projRoot = args[i + 1];
			}
			else {
				std::cerr << "No args for -p" << std::endl;
			}
		}

		if(args[i] == "-c") {
			if (i + 1 < args.size()) {
				contentPath = args[i + 1];
			}
			else {
				std::cerr << "No args for -c" << std::endl;
			}
		}
	}

	if (!std::filesystem::exists(projRoot + contentPath)) {
		std::cerr << "Bad args for -p; Using C:/ as project root." << std::endl;
		projRoot = "C:/";
	}
	
	//Node Editing
	Node_Editor* editor = new Node_Editor(game, projRoot, contentPath);

	std::vector<std::pair<int, int>> links;

	ShaderNode* snode = new ShaderNode(0, 0);
	editor->AddNode(snode);

	ShaderInputs* inode = new ShaderInputs(1, 8);
	editor->AddNode(inode);

	imnodes::SetNodeEditorSpacePos(snode->GetId(), ImVec2(450.0f, 50.0f));

	RD_ShaderMaterial* mat = new RD_ShaderMaterial(nullptr);
	CompileMat(game, editor, mat);

	EXP_StaticMesh* msh = new EXP_StaticMesh(game,
		mat,
		"/meshes/sphere",
		vec3f(0.0f, 0.0f, 12.0f),
		vec3f(),
		vec3f(1.0f, 1.0f, 1.0f)
	);
	game->GetRenderer()->GetMaterialLibrary()->AddMaterialToLib(mat, "custom_mat");

	//Arg parsing II
	for (int i = 0; i < args.size(); i++) {
		if (args[i] == "-o") {
			if (i + 1 < args.size()) {
				std::string fullPath = projRoot + contentPath + args[i + 1];
				if (std::filesystem::exists(fullPath)) {
					editor->OpenMaterialDraft(fullPath);
					CompileMat(game, editor, mat);
				} else {
					std::cerr << args[i + 1] << " : Files does not exists." << std::endl;
				}
			}
		}
	}

	Filebrowser openBrowser(projRoot + contentPath);
	openBrowser.AddFilter("draft");

	Filebrowser saveBrowserDraft(projRoot + contentPath);
	saveBrowserDraft.AddFilter("draft");

	Filebrowser saveFinalMaterial(projRoot + contentPath);
	saveFinalMaterial.AddFilter("exmtl");

	while (!game->GetRenderer()->WantToClose()) {
		msh->addTranslation(vec3f(0.0f, 0.0f, -0.01f));

		game->RenderScene();

		int w = game->GetRenderer()->getWindowWidth();
		int h = game->GetRenderer()->getWindowHeigh();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		{
			ImGui::SetNextWindowPos(ImVec2((float)(w / 2), 0));
			ImGui::SetNextWindowSize(ImVec2(w / 2, h));
			ImGui::Begin("Material", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

			{
				ImGui::BeginChild("Light Configaration", ImVec2((w / 2.0f) - 30, 105), true);

				if (ImGui::SliderFloat("Light direction X", &lightDirX, -1.0f, 1.0f)) {
					EXP_DirLight* dlight = game->GetCurrentMap()->GetDirLightByName("sun");
					if (dlight) {
						dlight->SetLightDir(vec3f(lightDirX, lightDirY, lightDirZ));
						game->GetRenderer()->UpdateDirLighting();
					}
				}

				if (ImGui::SliderFloat("Light direction Y", &lightDirY, -1.0f, 1.0f)) {
					EXP_DirLight* dlight = game->GetCurrentMap()->GetDirLightByName("sun");
					if (dlight) {
						dlight->SetLightDir(vec3f(lightDirX, lightDirY, lightDirZ));
						game->GetRenderer()->UpdateDirLighting();
					}
				}

				if (ImGui::SliderFloat("Light direction Z", &lightDirZ, -1.0f, 1.0f)) {
					EXP_DirLight* dlight = game->GetCurrentMap()->GetDirLightByName("sun");
					if (dlight) {
						dlight->SetLightDir(vec3f(lightDirX, lightDirY, lightDirZ));
						game->GetRenderer()->UpdateDirLighting();
					}
				}

				if (ImGui::InputFloat("Light Brightness", &lightBrtn, 0.1f, 1.0f)) {
					EXP_DirLight* dlight = game->GetCurrentMap()->GetDirLightByName("sun");
					if (dlight) {
						dlight->SetLightBrightness(lightBrtn);
						game->GetRenderer()->UpdateDirLighting();
					}
				}

				ImGui::EndChild();
			}

			{
				ImGui::BeginChild("Material Configuration", ImVec2((w / 2) - 30, 100), true);

				if (ImGui::Button("Compile Material", ImVec2(ImGui::GetColumnWidth(), 19.0f))) {
					CompileMat(game, editor, mat);
				}

				ImGui::Columns(1, nullptr, false);
				
				if (ImGui::Button("Save Final Material", ImVec2(ImGui::GetColumnWidth(), 19.0f))) {
					saveFinalMaterial.Open();
				}
				
				if (ImGui::Button("Save Draft Material", ImVec2(ImGui::GetColumnWidth(), 19.0f))) {
					saveBrowserDraft.Open();
				}

				if (ImGui::Button("Open Draft Material", ImVec2(ImGui::GetColumnWidth(), 19.0f))) {
					openBrowser.Open();
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

			openBrowser.Render(game->GetRenderer());
			saveBrowserDraft.Render(game->GetRenderer());
			saveFinalMaterial.Render(game->GetRenderer());
		}

		game->ExecCallbacks();

		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (openBrowser.HasSelected()) {
			editor->OpenMaterialDraft(openBrowser.GetSelectedFile());
			openBrowser.ResetBools();

			CompileMat(game, editor, mat);
		}

		if (saveBrowserDraft.OkPressed()) {
			editor->SaveMaterialDraft(saveBrowserDraft.GetFileNameBuffer());
			saveBrowserDraft.ResetBools();
		}

		if(saveFinalMaterial.OkPressed()) {
			editor->SaveFinalMaterial(saveFinalMaterial.GetFileNameBuffer());
			saveFinalMaterial.ResetBools();
		}

		game->EndFrame();
	}

	ImGui::DestroyContext();
	imnodes::Shutdown();

	delete editor;
	delete game;

	return 0;
}