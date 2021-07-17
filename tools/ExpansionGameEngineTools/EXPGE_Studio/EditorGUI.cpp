#include "EditorGUI.h"

EditorGUI::EditorGUI(EXP_Game* game, std::string projectPath, std::string contentPath) {
	m_game = game;
	m_reg = {};
	m_projectPath = projectPath;
	m_contentPath = contentPath;

	m_asset_browser = new AssetBrowser(game, m_projectPath + m_contentPath);
}

EditorGUI::~EditorGUI() {

}

void EditorGUI::RenderEditorGUI() {
	RD_WindowingSystem* winsys = m_game->GetRenderer()->GetRenderingAPI()->GetWindowingSystem();
	float wwidth = winsys->GetWidth();
	float wheight = winsys->GetHeight();

	{
		ImGui::SetNextWindowPos({ 0, 0 });
		ImGui::SetNextWindowSize(ImVec2((2.0f / 12.0f) * wwidth, (8.0f / 12.0f) * wheight));
		ImGui::Begin("Add", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

		if (ImGui::Button("Directionnal Light")) {

		}

		if (ImGui::Button("Point Light")) {
			EXP_PointLight* plight = new EXP_PointLight(m_game, vec3f(), vec3f(1.0f, 1.0f, 1.0f), 10.0f, 50.0f);
			plight->SetNameTag("pointlight" + std::to_string(m_reg.m_plights.size()));
			m_reg.m_plights.push_back(plight);
		}

		ImGui::End();
	}

	{
		m_asset_browser->Render(wwidth, wheight);
	}

	{
		ImGui::SetNextWindowPos({ (10.0f / 12.0f) * wwidth, 0 });
		ImGui::SetNextWindowSize(ImVec2((2.0f / 12.0f) * wwidth, (4.0f / 12.0f) * wheight));
		ImGui::Begin("Details", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

		ImGui::Text("Nothing selected.");

		ImGui::End();
	}

	{
		ImGui::SetNextWindowPos({ (10.0f / 12.0f) * wwidth, (4.0f / 12.0f) * wheight });
		ImGui::SetNextWindowSize({ (2.0f / 12.0f) * wwidth, (4.0f / 12.0f) * wheight });
		ImGui::Begin("Outliner", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

		ImGui::BeginTable("outliner_tab", 2, ImGuiTableFlags_BordersInnerV);

		for (auto p : m_reg.m_plights) {
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text(p->GetNameTag().c_str());
			ImGui::TableNextColumn();
			ImGui::Text("Point Light");
		}

		ImGui::EndTable();

		ImGui::End();
	}
}