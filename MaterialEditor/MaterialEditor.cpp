#include "MaterialEditor.h"

MaterialEditor::MaterialEditor(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	changed = false;
	
	m_scene = new QGraphicsScene();
	ui.prevc->setScene(m_scene);
	ui.prevc->setBackgroundBrush(QBrush(QColor(0, 0, 0, 255)));

	m_specScene = new QGraphicsScene();
	ui.prevs->setScene(m_specScene);
	ui.prevs->setBackgroundBrush(QBrush(QColor(0, 0, 0, 255)));

	connect(ui.actionQuit, SIGNAL(triggered()), SLOT(Quit()));
	connect(ui.actionNew_Material, SIGNAL(triggered()), SLOT(NewMaterial()));
	connect(ui.actionSave_Material, SIGNAL(triggered()), SLOT(SaveMaterial()));
}


void MaterialEditor::Quit() {
	QCoreApplication::quit();
}

void MaterialEditor::NewMaterial() {
	if (changed) {
		QMessageBox msg;
		msg.setText("Your work is not saved, do you want to save it ?");
		msg.setWindowTitle("WARNING");
		
		save = msg.addButton(QMessageBox::Save);
		discard = msg.addButton(QMessageBox::Discard);

		connect(save, SIGNAL(clicked()), SLOT(SaveMaterial()));

		msg.exec();
	}

	ui.cred->setValue(0.0);
	ui.cgreen->setValue(0.0);
	ui.cblue->setValue(0.0);

	ui.sred->setValue(0.0);
	ui.sgreen->setValue(0.0);
	ui.sblue->setValue(0.0);

	ui.sexp->setValue(0.0);

	changed = false;
}

void MaterialEditor::MarkChanged() {
	changed = true;
}

void MaterialEditor::SaveMaterial() {
	BD_MatWrite* mw = new BD_MatWrite();

	BD_MatDef mdef = {};
	mdef.Color = vec3f(ui.cred->value(), ui.cgreen->value(), ui.cblue->value());
	mdef.SpecularColor = vec3f(ui.sred->value(), ui.sgreen->value(), ui.sblue->value());
	mdef.SpecularExp = ui.sexp->value();

	QString saveloc = QFileDialog::getSaveFileName(this, "Select save destination", NULL, "*.exmtl");

	if (saveloc == "") {
		delete mw;

		return;
	}

	std::string path = saveloc.toUtf8().constData();

	mw->WriteMaterialToFile(mdef, path);

	delete mw;
}

void MaterialEditor::UpdateColor() {
	ui.prevc->setBackgroundBrush(QBrush(
		QColor(
			ui.cred->value() * 255,
			ui.cgreen->value() * 255,
			ui.cblue->value() * 255,
			255
		)
	));
}

void MaterialEditor::UpdateSpecColor() {
	ui.prevs->setBackgroundBrush(QBrush(
		QColor(
			ui.sred->value() * 255,
			ui.sgreen->value() * 255,
			ui.sblue->value() * 255,
			255
		)
	));
}