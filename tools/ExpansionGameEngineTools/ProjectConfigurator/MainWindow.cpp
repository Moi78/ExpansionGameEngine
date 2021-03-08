#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent) : QWidget(parent) {
	ui.setupUi(this);

	QDir d("versions/");
	if ((!d.exists()) || d.count() == 0) {
		QMessageBox::critical(this, "FATAL ERROR", "Cannot find any version of the engine.");
		exit(0);
	}
	else {
		for (auto d : d.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
			ui.combo_engine_ver->addItem(d);
		}
	}
}

MainWindow::~MainWindow() {

}

void MainWindow::BrowseProjDir() {
	QString d = QFileDialog::getExistingDirectory(this, "Choose project dir...");
	ui.projdir->setText(d);
}

bool MainWindow::MakeWindows() {
	QString ver = ui.combo_engine_ver->currentText();
	QDir d("versions/" + ver + "/Bins/Win64");
	if (!d.exists()) {
		QMessageBox::critical(this, "Fatal ERROR", "Missing engine bins for target : Windows x64");
		ui.target_win64->setDisabled(true);
		ui.target_win64->setChecked(false);
		return false;
	}

	QString src = "versions/" + ver + "/Bins/Win64/";
	QString dst = ui.projdir->text() + "/" + ui.proj_name->text() + "/Bins/Win64/";

	std::filesystem::copy(src.toUtf8().constData(), dst.toUtf8().constData(), std::filesystem::copy_options::recursive);

	return true;
}

bool MainWindow::MakeLinux() {
	QString ver = ui.combo_engine_ver->currentText();
	QDir d("versions/" + ver + "/Bins/Linux64");
	if (!d.exists()) {
		QMessageBox::critical(this, "Fatal ERROR", "Missing engine bins for target : Linux x64");
		ui.target_linux64->setDisabled(true);
		ui.target_linux64->setChecked(false);
		return false;
	}

	QString src = "versions/" + ver + "/Bins/Linux64/";
	QString dst = ui.projdir->text() + "/" + ui.proj_name->text() + "/Bins/Linux64/";

	std::filesystem::copy(src.toUtf8().constData(), dst.toUtf8().constData(), std::filesystem::copy_options::recursive);

	return true;

	return true;
}

bool MainWindow::MakeProjectTree() {
	QDir d(ui.projdir->text());
	if (!d.exists() || ui.projdir->text().isEmpty()) {
		QMessageBox::critical(this, "ERROR", "Invalid project dir.");
		return false;
	}

	if (ui.proj_name->text().isEmpty()) {
		QMessageBox::critical(this, "ERROR", "Please enter a valid project name.");
		return false;
	}

	d.mkdir(ui.proj_name->text());
	d.cd(ui.proj_name->text());

	d.mkdir("Bins");
	d.mkdir("Game");

	QString content = ui.content_folder_name->text();
	if (content.isEmpty()) {
		content = QString("Content");
		ui.content_folder_name->setText("Content");
	}

	QString engine = ui.engine_folder_name->text();
	if (engine.isEmpty()) {
		engine = QString("Engine");
		ui.engine_folder_name->setText("Engine");
	}

	d.mkdir("Game/" + content);
	d.mkdir("Game/" + engine);

	d.mkdir(ui.proj_name->text());

	return true;
}

void MainWindow::CreateProject() {
	ui.create_button->setDisabled(true);

	if (!MakeProjectTree()) { 
		ui.create_button->setDisabled(false);
		return;
	}

	if (ui.target_win64->isChecked()) {
		if (!MakeWindows()) {
			ui.create_button->setDisabled(false);
		}
		else {
			CopyLibs();
		}
	}

	if (ui.target_linux64->isChecked()) {
		if (!MakeLinux()) {
			ui.create_button->setDisabled(false);
		}
	}

	CopyIncludes();
	CopyEngineData();

	ui.create_button->setDisabled(false);
}

bool MainWindow::CopyEngineData() {
	QString ver = ui.combo_engine_ver->currentText();
	QDir d("versions/" + ver + "/Engine");
	if (!d.exists()) {
		QMessageBox::critical(this, "ERROR", "Missing Engine Data files.");
		return false;
	}

	QString src = "versions/" + ver + "/Engine";
	QString dest = ui.projdir->text() + "/" + ui.proj_name->text() + "/Game/" + ui.engine_folder_name->text();
	std::filesystem::copy(src.toUtf8().constData(), dest.toUtf8().constData(), std::filesystem::copy_options::recursive);

	return true;
}

bool MainWindow::CopyIncludes() {
	QString ver = ui.combo_engine_ver->currentText();
	QDir d("versions/" + ver + "/Include");
	if (!d.exists()) {
		QMessageBox::critical(this, "ERROR", "Missing Includes files.");
		return false;
	}

	QString src = "versions/" + ver + "/Include";
	QString dest = ui.projdir->text() + "/" + ui.proj_name->text() + "/Include/";
	std::filesystem::copy(src.toUtf8().constData(), dest.toUtf8().constData(), std::filesystem::copy_options::recursive);

	return true;
}

bool MainWindow::CopyLibs() {
	QString ver = ui.combo_engine_ver->currentText();
	QDir d("versions/" + ver + "/Include");
	if (!d.exists()) {
		QMessageBox::critical(this, "ERROR", "Missing Lib files.");
		return false;
	}

	QString src = "versions/" + ver + "/Lib";
	QString dest = ui.projdir->text() + "/" + ui.proj_name->text() + "/Lib/";
	std::filesystem::copy(src.toUtf8().constData(), dest.toUtf8().constData(), std::filesystem::copy_options::recursive);

	return true;
}

void MainWindow::Cancel() {
	if (QMessageBox::question(this, "Cancel ?", "Do you really want to cancel ?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
		exit(0);
	}
}