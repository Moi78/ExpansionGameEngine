#pragma once

#include <QtWidgets/QWidget>
#include "ui_FBX2MSH.h"

#include <qmessagebox.h>
#include <qfiledialog.h>
#include <qstandardpaths.h>

#include <BulldozerFileManager.h>
#include <BD_Writer.h>

#include <vec3.h>
#include <vec2.h>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

class FBX2MSH : public QWidget
{
    Q_OBJECT

public:
    FBX2MSH(QWidget *parent = Q_NULLPTR);

public slots:
    void ParcSrc();
    void ParcDest();
    void Export();

private:
    bool CheckOptions();
    void ExtractDataFromScene(const aiScene* scene);
    void ImportScene();

    vec3f ToVec3f(aiVector3D);

    Ui::FBX2MSHClass ui;

    QString m_src;
    QString m_dst;
};
