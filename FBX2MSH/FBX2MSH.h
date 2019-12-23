#pragma once

#include <QtWidgets/QWidget>
#include "ui_FBX2MSH.h"
#include <qmessagebox.h>
#include <qfiledialog.h>
#include <qstandardpaths.h>

#include <string>
#include <filesystem>
#include <vector>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <vec3.h>
#include <BulldozerFileManager.h>
#include <BD_Writer.h>

class FBX2MSH : public QWidget
{
	Q_OBJECT

public:
	FBX2MSH(QWidget *parent = Q_NULLPTR);

public slots:
	void Filepath();
	void Outpath();
	void Export();

private:
	void ImportScene();
	void ExtractDataFromScene(const aiScene*);

	bool CheckSelectedOptions();

	vec3d ToVec3d(aiVector3D);

	Ui::FBX2MSHClass ui;
};
