#include "FBX2MSH.h"

FBX2MSH::FBX2MSH(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	SetupFBXSDK();
}

void FBX2MSH::SetupFBXSDK() {
	m_fbxmanager = FbxManager::Create();
	m_iosettings = FbxIOSettings::Create(m_fbxmanager, IOSROOT);

	m_fbxmanager->SetIOSettings(m_iosettings);
}

void FBX2MSH::QueryIOSettings() {
	m_fbxmanager->GetIOSettings()->SetBoolProp(IMP_FBX_MATERIAL, ui.imaterial);
	m_fbxmanager->GetIOSettings()->SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, ui.iglobal);
	m_fbxmanager->GetIOSettings()->SetBoolProp(IMP_FBX_TEXTURE, ui.itex);
	m_fbxmanager->GetIOSettings()->SetBoolProp(IMP_FBX_ANIMATION, ui.ianim);

	m_fbxmanager->GetIOSettings()->SetBoolProp(IMP_FBX_LINK, false);
	m_fbxmanager->GetIOSettings()->SetBoolProp(IMP_FBX_SHAPE, false);
	m_fbxmanager->GetIOSettings()->SetBoolProp(IMP_FBX_GOBO, false);
}

void FBX2MSH::ImportScene() {
	m_importer = FbxImporter::Create(m_fbxmanager, "");

	std::string filepath = ui.filepath->text().toUtf8().constData();

	bool file_exists = std::filesystem::exists(std::filesystem::path(filepath));

	if (file_exists) {
		bool importStatus = m_importer->Initialize(filepath.c_str(), -1, m_fbxmanager->GetIOSettings());

		if (!importStatus) {
			QMessageBox::critical(this, "ERROR", "Can't import file \"" + ui.filepath->text() + "\".");
			return;
		}

		FbxScene* scene = FbxScene::Create(m_fbxmanager, "scene");
		m_importer->Import(scene);

		m_importer->Destroy();

		ExtractDataFromScene(scene);
	}
	else {
		QMessageBox::critical(this, "ERROR", "File \"" + ui.filepath->text() + "\" not found.");
		return;
	}
}

void FBX2MSH::Filepath() {
	QString filepath = QFileDialog::getOpenFileName(this, "Open FBX file", QStandardPaths::writableLocation(QStandardPaths::DesktopLocation), "*.fbx");

	ui.filepath->setText(filepath);
}

void FBX2MSH::Outpath() {
	QString filepath = QFileDialog::getExistingDirectory(this, "Choose output directory", QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));

	ui.outpath->setText(filepath);
}

void FBX2MSH::Export() {
	if (!CheckSelectedOptions()) {
		return;
	}

	ImportScene();
}

bool FBX2MSH::CheckSelectedOptions() {
	if (ui.filepath->text() == "") {
		QMessageBox::critical(this, "ERROR", "Can't export, please specify a source file.");
		return false;
	}

	if (ui.outpath->text() == "") {
		QMessageBox::critical(this, "ERROR", "Can't export, please specify a destination path.");
		return false;
	}

	if (ui.filname->text() == "") {
		QMessageBox::critical(this, "ERROR", "Can't export, please specify a file name.");
		return false;
	}

	return true;
}

void FBX2MSH::ExtractDataFromScene(FbxScene* scene) {
	FbxGeometryConverter* triangulate = new FbxGeometryConverter(m_fbxmanager);
	triangulate->Triangulate(scene, true, true);

	delete triangulate;

	int node_count = scene->GetRootNode()->GetChildCount();

	std::vector<FbxMesh*> meshes;

	for (int i = 0; i < node_count; i++) {
		if (scene->GetRootNode()->GetChild(i)->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh) {
			meshes.push_back(scene->GetRootNode()->GetChild(i)->GetMesh());
		}
	}

	BD_Writer* writer;

	int meshnbr = 0;

	for (auto mesh : meshes) {
		writer = new BD_Writer();

		int vCount = mesh->GetControlPointsCount();
		int iCount = mesh->GetPolygonCount();
		int nCount = mesh->GetElementNormalCount();

		//Vertices
		for (int i = 0; i < vCount; i++) {
			writer->AppendVertex(vec3d(mesh->GetControlPointAt(i).mData[0], mesh->GetControlPointAt(i).mData[1], mesh->GetControlPointAt(i).mData[2]));
		}

		//Indices
		for (int i = 0; i < iCount; i++) {
			writer->AppendIndices(mesh->GetPolygonVertex(i, 0));
			writer->AppendIndices(mesh->GetPolygonVertex(i, 2));
			writer->AppendIndices(mesh->GetPolygonVertex(i, 3));
		}

		//Normals
		FbxGeometryElementNormal* normals = mesh->GetElementNormal();
		if (normals) {
			for (int i = 0; i < vCount; i++) {
				int nIndex = 0;

				if (normals->GetReferenceMode() == FbxGeometryElement::eDirect) {
					nIndex = i;
				}

				if (normals->GetReferenceMode() == FbxGeometryElement::eIndexToDirect) {
					nIndex = normals->GetIndexArray().GetAt(i);
				}

				FbxVector4 Normal = normals->GetDirectArray().GetAt(nIndex);
				writer->AppendNormal(vec3d(Normal[0], Normal[1], Normal[2]));
			}
		}
		else {
			std::cerr << "ERROR : Failed to get normals from the FBX file." << std::endl;
		}

		//Write
		std::string basePath = ui.outpath->text().toUtf8().constData();
		std::string filename = ui.filname->text().toUtf8().constData();

		writer->ToBinary(basePath + "/", filename + "_" + std::to_string(meshnbr));

		meshnbr++;
	}
}

vec3d FBX2MSH::ToVec3(FbxVector4* fbvec) {
	return vec3d(fbvec->mData[0], fbvec->mData[1], fbvec->mData[2]);
}