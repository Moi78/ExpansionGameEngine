#include "FBX2MSH.h"

FBX2MSH::FBX2MSH(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
}

void FBX2MSH::ParcSrc() {
    m_src = QFileDialog::getOpenFileName(this, "Open FBX file", QStandardPaths::writableLocation(QStandardPaths::DesktopLocation), "*.fbx");
    ui.src->setText(m_src);
}

void FBX2MSH::ParcDest() {
    m_dst = QFileDialog::getSaveFileName(this, "MSH Save path", QStandardPaths::writableLocation(QStandardPaths::DesktopLocation), "*.msh");
    ui.dest->setText(m_dst);
}

void FBX2MSH::Export() {
    if (!CheckOptions())
        return;

	ImportScene();
}

void FBX2MSH::ImportScene() {
	std::string srcPath = m_src.toUtf8().constData();
	Assimp::Importer importer;

	const aiScene* scene;
	if (ui.opt_Trig->checkState()) {
		scene = importer.ReadFile(srcPath, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);
	}
	else {
		scene = importer.ReadFile(srcPath, aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);
	}

	if (!scene) {
		QMessageBox::critical(this, "ERROR !!!", "Cannot import scene.");
		return;
	}
	else {
		ExtractDataFromScene(scene);
	}
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
			writer->AppendVertex(ToVec3f(scene->mMeshes[i]->mVertices[v]));
		}

		//Indices
		for (int ind = 0; ind < scene->mMeshes[i]->mNumFaces; ind++) {
			writer->AppendIndices(scene->mMeshes[i]->mFaces[ind].mIndices[0]);
			writer->AppendIndices(scene->mMeshes[i]->mFaces[ind].mIndices[1]);
			writer->AppendIndices(scene->mMeshes[i]->mFaces[ind].mIndices[2]);
		}

		//Normals
		for (int n = 0; n < scene->mMeshes[i]->mNumVertices; n++) {
			writer->AppendNormal(ToVec3f(scene->mMeshes[i]->mNormals[n]));
		}


		//UV Coords
		if (scene->mMeshes[i]->HasTextureCoords(0) == true) {
			for (int uv = 0; uv < scene->mMeshes[i]->mNumVertices; uv++) {
				writer->AppendUVcoord(vec2f(scene->mMeshes[i]->mTextureCoords[0][uv].x, scene->mMeshes[i]->mTextureCoords[0][uv].y));
			}
		}
		else {
			for (int uv = 0; uv < scene->mMeshes[i]->mNumVertices; uv++) {
				writer->AppendUVcoord(vec2f(0.0f, 0.0f));
			}
		}

		//Vertex Weight
		std::vector<vec4f> vWeights;
		for (int w = 0; w < scene->mMeshes[i]->mNumVertices; w++) {
			vWeights.push_back(vec4f());
		}

		for (int bn = 0; bn < scene->mMeshes[i]->mNumBones; bn++) {
			for (int w = 0; w < scene->mMeshes[i]->mNumVertices; w++) {
				aiVertexWeight vw = scene->mMeshes[i]->mBones[bn]->mWeights[w];
				
				if (vWeights[vw.mVertexId].GetX() == 0) {
					vWeights[vw.mVertexId].SetX(vw.mWeight);
				}
				else if (vWeights[vw.mVertexId].GetY() == 0) {
					vWeights[vw.mVertexId].SetY(vw.mWeight);
				}
				else if (vWeights[vw.mVertexId].GetZ() == 0) {
					vWeights[vw.mVertexId].SetZ(vw.mWeight);
				}
				else if (vWeights[vw.mVertexId].GetW() == 0) {
					vWeights[vw.mVertexId].SetW(vw.mWeight);
				}
			}
		}

		for (auto w : vWeights) {
			writer->AppendVertexWeight(w);
		}

		QFileInfo finfo(m_dst);
		QString fpath = finfo.absolutePath() + "/";

		std::string dest;
		if (scene->mNumMeshes > 1) {
			QString fname = finfo.fileName().split(".", QString::SkipEmptyParts).at(0) + "_";
			dest = fname.toUtf8().constData() + std::string(scene->mMeshes[i]->mName.C_Str());
		}
		else {
			QString fname = finfo.fileName().split(".", QString::SkipEmptyParts).at(0);
			dest = fname.toUtf8().constData();
		}

		writer->ToBinary(fpath.toUtf8().constData(), dest);
		writer->ClearWriter();
	}
}

bool FBX2MSH::CheckOptions() {
    if (ui.src->text() == "" || !QFile::exists(ui.src->text())) {
        QMessageBox::critical(this, "ERROR !!!", "You didn't specified a valid source file.");
        return false;
    }

    if (ui.dest->text() == "") {
        QMessageBox::critical(this, "ERROR !!!", "You didn't specified a valid destination file.");
        return false;
    }

    return true;
}

vec3f FBX2MSH::ToVec3f(aiVector3D vec) {
	return vec3f(vec.x, vec.y, vec.z);
}