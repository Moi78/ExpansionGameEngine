#pragma once

#include <QtWidgets/QWidget>
#include "ui_FBX2MSH.h"
#include <qmessagebox.h>
#include <qfiledialog.h>
#include <qstandardpaths.h>

#include <string>
#include <filesystem>
#include <vector>

#include <fbxsdk.h>
#include <fbxsdk/fileio/fbxiosettings.h>
#include <fbxsdk/fbxsdk_version.h>

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
	void SetupFBXSDK();
	void QueryIOSettings();
	void ImportScene();
	void ExtractDataFromScene(FbxScene*);

	vec3d ToVec3(FbxVector4*);

	bool CheckSelectedOptions();

	//FBX SDK
	FbxManager* m_fbxmanager;
	FbxIOSettings* m_iosettings;
	FbxImporter* m_importer;

	Ui::FBX2MSHClass ui;
};
