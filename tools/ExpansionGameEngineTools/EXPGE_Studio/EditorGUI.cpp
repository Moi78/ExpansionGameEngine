#include "EditorGUI.h"

EditorGUI::EditorGUI(EXP_Game* game, std::string projectPath, std::string contentPath) {
	m_game = game;

	m_reg = {};

	m_conf = {};
	#ifdef _DEBUG
		m_conf.MaterialEditorPath = std::getenv("EXPGE_MAT_EDITOR_DIR");
	#else
		m_conf.MaterialEditorPath = "./";
	#endif //_DEBUG

	m_projectPath = projectPath;
	m_contentPath = contentPath;

	m_asset_browser = new AssetBrowser(game, m_projectPath + m_contentPath, &m_conf, &m_reg);
}

EditorGUI::~EditorGUI() {
	delete m_asset_browser;
}

void EditorGUI::RenderEditorGUI() {
	float shift = 22.0f;

	RD_WindowingSystem* winsys = m_game->GetRenderer()->GetRenderingAPI()->GetWindowingSystem();
	float wwidth = winsys->GetWidth();
	float wheight = winsys->GetHeight() - 20.0f;

	RenderMenuBar();

	{
		ImGui::Begin("Add");

		if (ImGui::Button("Directionnal Light")) {
			EXP_DirLight* dlight = new EXP_DirLight(m_game, vec3f(-1.0f, -1.0f, -1.0f), vec3f(1.0f, 1.0f, 1.0f), 5.0f);
			dlight->SetNameTag("directionnalLight" + std::to_string(m_reg.m_dlights.size()));
			m_reg.m_dlights.push_back(dlight);
		}

		if (ImGui::Button("Point Light")) {
			EXP_PointLight* plight = new EXP_PointLight(m_game, vec3f(), vec3f(1.0f, 1.0f, 1.0f), 10.0f, 50.0f);
			plight->SetNameTag("pointlight" + std::to_string(m_reg.m_plights.size()));
			m_reg.m_plights.push_back(plight);
		}

		ImGui::End();
	}

	{
		m_asset_browser->Render(wwidth, wheight, shift);
	}

	{
		ImGui::Begin("Details");

		if (!m_selected.second) {
			ImGui::Text("Nothing selected.");
		}
		else {
			if (m_selected.first == COMP_TYPES::TPLIGHT) {
				EXP_PointLight* plight = reinterpret_cast<EXP_PointLight*>(m_selected.second);
				DetailPLight(plight);
			} else if (m_selected.first == COMP_TYPES::TDLIGHT) {
				EXP_DirLight* dlight = reinterpret_cast<EXP_DirLight*>(m_selected.second);
				DetailDLight(dlight);
			} else if (m_selected.first == COMP_TYPES::TSMESH) {
				EXP_StaticMesh* smesh = reinterpret_cast<EXP_StaticMesh*>(m_selected.second);
				DetailSMesh(smesh);
			}
		}

		ImGui::End();
	}

	{
		ImGui::Begin("Outliner");

		if (ImGui::BeginTable("outliner_tab", 2, ImGuiTableFlags_BordersInnerV)) {

			for (auto p : m_reg.m_plights) {
				ImGui::TableNextRow();
				ImGui::TableNextColumn();

				bool selected = ((void*)p) == m_selected.second ? true : false;
				if (ImGui::Selectable(p->GetNameTag().c_str(), selected)) {
					m_selected = std::pair<COMP_TYPES, void*>(COMP_TYPES::TPLIGHT, p);
				}

				ImGui::TableNextColumn();
				ImGui::Text("Point Light");
			}

			for (auto m : m_reg.m_meshes) {
				ImGui::TableNextRow();
				ImGui::TableNextColumn();

				bool selected = ((void*)m) == m_selected.second ? true : false;
				if (ImGui::Selectable(m->GetNameTag().c_str(), selected)) {
					m_selected = std::pair<COMP_TYPES, void*>(COMP_TYPES::TSMESH, m);
				}

				ImGui::TableNextColumn();
				ImGui::Text("Static Mesh");
			}

			for (auto d : m_reg.m_dlights) {
				ImGui::TableNextRow();
				ImGui::TableNextColumn();

				bool selected = ((void*)d) == m_selected.second ? true : false;
				if (ImGui::Selectable(d->GetNameTag().c_str(), selected)) {
					m_selected = std::pair<COMP_TYPES, void*>(COMP_TYPES::TDLIGHT, d);
				}

				ImGui::TableNextColumn();
				ImGui::Text("Directionnal Light");
			}

			ImGui::EndTable();
		}

		ImGui::End();
	}
}

void EditorGUI::RenderMenuBar() {
	ImGui::BeginMainMenuBar();

	if (ImGui::BeginMenu("File")) {
		ImGui::MenuItem("New...", nullptr);

		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();
}

void EditorGUI::DetailPLight(EXP_PointLight* plight) {
	char name[50] = {};
	strcpy(name, plight->GetNameTag().c_str());

	if (ImGui::InputText("Component Name Tag", name, 50)) {
		plight->SetNameTag(name);
	}

	ImGui::Separator();

	vec3f pos = plight->GetPosition();
	if (ImGui::InputFloat3("Position", pos.GetPTR())) {
		plight->SetPosition(pos);
		m_game->GetRenderer()->UpdatePointsLighting();
	}

	vec3f color = plight->GetColor();
	if(ImGui::ColorEdit3("Light Color", color.GetPTR())) {
		plight->SetColor(color);
		m_game->GetRenderer()->UpdatePointsLighting();
	}

	float b = plight->GetBrightness();
	if (ImGui::InputFloat("Light brightness", &b)) {
		plight->SetBrightness(b);
		m_game->GetRenderer()->UpdatePointsLighting();
	}

	float r = plight->GetLightRadius();
	if (ImGui::InputFloat("Light radius", &r)) {
		plight->SetLightRadius(r);
		m_game->GetRenderer()->UpdatePointsLighting();
	}
}

void EditorGUI::DetailDLight(EXP_DirLight* dirlight) {
	char name[50] = {};
	strcpy(name, dirlight->GetNameTag().c_str());

	if (ImGui::InputText("Component Name Tag", name, 50)) {
		dirlight->SetNameTag(name);
	}

	ImGui::Separator();

	vec3f dir = dirlight->GetLightDir();
	if (ImGui::SliderFloat3("Light direction", dir.GetPTR(), -1.0f, 1.0f)) {
		dirlight->SetLightDir(dir);
		m_game->GetRenderer()->UpdateDirLighting();
	}

	vec3f color = dirlight->GetLightColor();
	if (ImGui::ColorEdit3("Light Color", color.GetPTR())) {
		dirlight->SetLightColor(color);
		m_game->GetRenderer()->UpdateDirLighting();
	}

	float b = dirlight->GetBrightness();
	if (ImGui::InputFloat("Light Brightness", &b)) {
		dirlight->SetLightBrightness(b);
		m_game->GetRenderer()->UpdateDirLighting();
	}

	ImGui::Separator();

	bool shadowCasting = dirlight->GetShadowCasting();
	if (ImGui::Checkbox("Casts Shadows", &shadowCasting)) {
		dirlight->SetShadowCasting(shadowCasting);
		m_game->GetRenderer()->UpdateDirLighting();
	}
}

void EditorGUI::DetailSMesh(EXP_StaticMesh* smesh) {
	char name[50] = {};
	strcpy(name, smesh->GetNameTag().c_str());

	if (ImGui::InputText("Component Name Tag", name, 50)) {
		smesh->SetNameTag(name);
	}

	ImGui::Separator();

	vec3f pos = smesh->GetPosition();
	if (ImGui::InputFloat3("Position", pos.GetPTR())) {
		smesh->SetPosition(pos);
	}

	vec3f rot = smesh->GetRotation();
	if (ImGui::InputFloat3("Rotation", rot.GetPTR())) {
		smesh->SetRotation(rot);
	}

	vec3f scale = smesh->GetScale();
	if (ImGui::InputFloat3("Scale", scale.GetPTR())) {
		smesh->SetScale(scale);
	}

	ImGui::Separator();
}