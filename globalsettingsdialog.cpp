#pragma warning(push, 3)
#include <QFileDialog>
#pragma warning(pop)
#include "globalsettingsdialog.h"

GlobalSettingsDialog::GlobalSettingsDialog(QWidget* parent)
	: QDialog(parent)
{
	this->setupUi(this);

	// Set the lineEdit's text to the application's path
	lineEditPath->setText(QCoreApplication::applicationDirPath() + "/results.txt");

	// Connect the browse button to its corresponding slot
	connect(pushButtonBrowse, SIGNAL(clicked()), this, SLOT(showFileDialog()));
}

GlobalSettingsDialog::~GlobalSettingsDialog()
{

}

void GlobalSettingsDialog::showFileDialog()
{
	QString path = QFileDialog::getExistingDirectory(this, tr("Select a folder"),
		"/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

	if(!path.isEmpty())
	{
		lineEditPath->setText(path + "/results.txt");
	}
}

void GlobalSettingsDialog::accept()
{
	// Insert prey data
	mSettings.insert("initialNumberOfPreys", lineEditInitialPreys->text().toFloat());
	mSettings.insert("competenceFactor", lineEditCompetenceFactor->text().toFloat());
	mSettings.insert("preyReproductionRadius", spinBoxPreyReproductionRadius->value());
	mSettings.insert("preyReproductiveCapacity", spinBoxPreyReproductiveCapacity->value());
	mSettings.insert("competenceRadius", spinBoxCompetenceRadius->value());

	// Insert predator data
	mSettings.insert("initialNumberOfPredators", lineEditInitialPredators->text().toInt());
	mSettings.insert("predatorCognitiveFactor", lineEditCognitiveFactor->text().toFloat());
	mSettings.insert("predatorSocialFactor", lineEditSocialFactor->text().toFloat());
	mSettings.insert("predatorMaximumSpeed", spinBoxMaxSpeed->value());
	mSettings.insert("predatorReproductiveCapacity", spinBoxPredatorReproductiveCapacity->value());
	mSettings.insert("predatorReproductionRadius", spinBoxPredatorReproductionRadius->value());
	mSettings.insert("initialInertiaWeight", lineEditInitialWeight->text().toFloat());
	mSettings.insert("finalInertiaWeight", lineEditFinalWeight->text().toFloat());

	// Insert path to results file
	mSettings.insert("resultsFilePath", lineEditPath->text());

	emit settingsChanged(mSettings);

	QDialog::accept();
}