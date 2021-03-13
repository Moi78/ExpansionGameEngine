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
		QMessageBox::critical(this, "WARNING", "Missing engine bins for target : Windows x64");
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
		QMessageBox::warning(this, "WARNING", "Missing engine bins for target : Linux x64");
		return false;
	}

	QString src = "versions/" + ver + "/Bins/Linux64/";
	QString dst = ui.projdir->text() + "/" + ui.proj_name->text() + "/Bins/Linux64/";

	std::filesystem::copy(src.toUtf8().constData(), dst.toUtf8().constData(), std::filesystem::copy_options::recursive);

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
	MakeGameInfo();
	MakeGameProj();

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

void MainWindow::MakeGameInfo() {
	std::ofstream file;
	file.open(
		std::string(ui.projdir->text().toUtf8().constData()) +
		std::string("/") +
		std::string(ui.proj_name->text().toUtf8().constData()) +
		std::string("/Game/GameInfo.json")
	);

	Json::Value root;

	if (ui.target_win64->isChecked()) {
		Json::Value root_win;

		root_win["RootEngineContentFolder"] = ui.engine_folder_name->text().toUtf8().constData();
		root_win["RootGameContentFolder"] = ui.content_folder_name->text().toUtf8().constData();

		root_win["GameBaseResolution"][0] = ui.res_w->value();
		root_win["GameBaseResolution"][1] = ui.res_h->value();

		root_win["RenderingPipeline"] = ui.combo_pipeline->currentText().toLower().toUtf8().constData();

		root_win["GameBin"] = std::string(ui.proj_name->text().toUtf8().constData()) + ".dll";

		root_win["GameName"] = ui.proj_name->text().toUtf8().constData();
		root_win["StartupMap"] = ui.startup_map->text().toUtf8().constData();

		root["windows"] = root_win;
	}

	if (ui.target_linux64->isChecked()) {
		Json::Value root_nux;

		root_nux["RootEngineContentFolder"] = ui.engine_folder_name->text().toUtf8().constData();
		root_nux["RootGameContentFolder"] = ui.content_folder_name->text().toUtf8().constData();

		root_nux["GameBaseResolution"][0] = ui.res_w->value();
		root_nux["GameBaseResolution"][1] = ui.res_h->value();

		root_nux["RenderingPipeline"] = ui.combo_pipeline->currentText().toLower().toUtf8().constData();

		root_nux["GameBin"] = std::string(ui.proj_name->text().toUtf8().constData()) + ".so";

		root_nux["GameName"] = ui.proj_name->text().toUtf8().constData();
		root_nux["StartupMap"] = ui.startup_map->text().toUtf8().constData();

		root["linux"] = root_nux;
	}

	file << root;
	file.close();
}

void MainWindow::MakeGameProj() {
	std::ofstream file;
	file.open(
		std::string(ui.projdir->text().toUtf8().constData()) +
		std::string(ui.proj_name->text().toUtf8().constData()) +
		std::string("/GameProj.json")
	);

	Json::Value root;

	root["EngineVersion"] = ui.combo_engine_ver->currentText().toUtf8().constData();
	root["GameInfo"] =  std::string("./Game/GameInfo.json");

	root["Source"] = std::string("./") + std::string(ui.proj_name->text().toUtf8().constData()) + std::string("/");
	root["GameName"] = ui.proj_name->text().toUtf8().constData();

	file << root;
	file.close();
}