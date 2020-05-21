#include "FBX2MSH.h"

FBX2MSH::FBX2MSH(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

void FBX2MSH::ImportScene() {
	std::string filepath = ui.filepath->text().toUtf8().constData();

	bool file_exists = std::filesystem::exists(std::filesystem::path(filepath));

	if (file_exists) {
		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(filepath, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);

		if (!scene) {
			QMessageBox::critical(this, "ERROR", "Cannot import scene. See console for details.");
			std::cerr << importer.GetErrorString() << std::endl;
		}
		else {
			ExtractDataFromScene(scene);
		}
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

void FBX2MSH::ExtractDataFromScene(const aiScene* scene) {
	if (!scene->HasMeshes()) {
		QMessageBox::critical(this, "ERROR", "This scene doesn't have any mesh, can't convert it to msh.");
		return;
	}

	BD_Writer* writer = new BD_Writer();

	for (int i = 0; i < scene->mNumMeshes; i++) {
		if (!scene->mMeshes[i]->HasNormals() && !scene->mMeshes[i]->HasFaces()) {
			QMessageBox::critical(this, "ERROR", "This mesh has no normals or no faces, it is unusable with Expansion Game Engine");
			return;
		}

		//Vertices
		for (int v = 0; v < scene->mMeshes[i]->mNumVertices; v++) {
			writer->AppendVertex(vec3d(ToVec3d(scene->mMeshes[i]->mVertices[v])));
		}

		//Indices
		for (int ind = 0; ind < scene->mMeshes[i]->mNumFaces; ind++) {
			writer->AppendIndices(scene->mMeshes[i]->mFaces[ind].mIndices[0]);
			writer->AppendIndices(scene->mMeshes[i]->mFaces[ind].mIndices[1]);
			writer->AppendIndices(scene->mMeshes[i]->mFaces[ind].mIndices[2]);
		}

		//Normals
		for (int n = 0; n < scene->mMeshes[i]->mNumVertices; n++) {
			writer->AppendNormal(vec3d(ToVec3d(scene->mMeshes[i]->mNormals[n])));
		}


		//UV Coords
		if (scene->mMeshes[i]->HasTextureCoords(0) == true) {
			for (int uv = 0; uv < scene->mMeshes[i]->mNumVertices; uv++) {
				writer->AppendUVcoord(vec2d(scene->mMeshes[i]->mTextureCoords[0][uv].x, scene->mMeshes[i]->mTextureCoords[0][uv].y));
			}
		}
		else {
			std::cout << "No UV coords on this mesh" << std::endl;
		}

		std::string outpath = ui.outpath->text().toUtf8().constData() + std::string("/");
		std::string base_filename = ui.filname->text().toUtf8().constData();
		std::string filename = base_filename + "_" + scene->mMeshes[i]->mName.C_Str();

		writer->ToBinary(outpath, filename);
		writer->ClearWriter();
	}
}

vec3d FBX2MSH::ToVec3d(aiVector3D vec) {
	return vec3d(vec.x, vec.y, vec.z);
}