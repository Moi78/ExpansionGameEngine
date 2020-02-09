#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MaterialEditor.h"

class MaterialEditor : public QMainWindow
{
	Q_OBJECT

public:
	MaterialEditor(QWidget *parent = Q_NULLPTR);

private:
	Ui::MaterialEditorClass ui;
};
