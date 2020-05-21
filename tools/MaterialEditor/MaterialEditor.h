#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MaterialEditor.h"

#include <qmessagebox.h>
#include <qpushbutton.h>
#include <qfiledialog.h>

#include <BulldozerFileManager.h>
#include <BD_MatRW.h>
#include <BD_StructMan.h>
#include <vec3.h>

#include <string>

class MaterialEditor : 
	public QMainWindow
{
	Q_OBJECT

public:
	MaterialEditor(QWidget *parent = Q_NULLPTR);

public slots:
	void Quit();
	void NewMaterial();
	void MarkChanged();
	void SaveMaterial();

	void UpdateColor();
	void UpdateSpecColor();

private:
	Ui::MaterialEditorClass ui;

	QPushButton* save;
	QPushButton* discard;

	QGraphicsScene* m_scene;
	QGraphicsScene* m_specScene;

	bool changed;
};
