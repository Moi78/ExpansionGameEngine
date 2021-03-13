#pragma once

#include "ui_MainWindow.h"

#include <qwidget.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qdir.h>
#include <qfile.h>

#include <filesystem>
#include <fstream>

#include <json/json.h>

class MainWindow : public QWidget
{
	Q_OBJECT;

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();

	bool MakeWindows();
	bool MakeLinux();
	bool MakeProjectTree();

	bool CopyLibs();
	bool CopyIncludes();
	bool CopyEngineData();

	void MakeGameInfo();
	void MakeGameProj();

public slots:
	void BrowseProjDir();
	void CreateProject();
	void Cancel();

private:
	Ui::MainWindow ui;
};

