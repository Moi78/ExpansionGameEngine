#include "FBX2MSH.h"

FBX2MSH::FBX2MSH(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	SetupFBXSDK();
}

void FBX2MSH::SetupFBXSDK() {

}

void FBX2MSH::QueryIOSettings() {

}

void FBX2MSH::ImportScene() {
	std::string filepath = ui.filepath->text().toUtf8().constData();

	bool file_exists = std::filesystem::exists(std::filesystem::path(filepath));

	if (file_exists) {
		
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

void FBX2MSH::ExtractDataFromScene() {
	
}