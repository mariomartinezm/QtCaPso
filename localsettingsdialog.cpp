#include <qfiledialog.h>
#include "localsettingsdialog.h"

LocalSettingsDialog::LocalSettingsDialog(QWidget* parent)
	: QDialog(parent)
{
	this->setupUi(this);

	// Set the application's path to the correspoinding lineEdit's text
	lineEditPath->setText(QCoreApplication::applicationDirPath() + "/results.txt");

	// Connect the browse button to its corresponding slot
	connect(pushButtonBrowse, SIGNAL(clicked()), this, SLOT(showFileDialog()));
}

LocalSettingsDialog::~LocalSettingsDialog()
{
	
}

void LocalSettingsDialog::showFileDialog()
{
	QString path = QFileDialog::getExistingDirectory(this, tr("Select a folder"),
		"/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

	if(!path.isEmpty())
	{
		lineEditPath->setText(path + "/results.txt");
	}
}

void LocalSettingsDialog::accept()
{
	// Insert prey data
	mSettings.insert("initialNumberOfPreys", lineEditInitialPreys->text().toFloat());
	mSettings.insert("competenceFactor", lineEditCompetenceFactor->text().toFloat());
	mSettings.insert("preyReproductionRadius", spinBoxPreyReproductionRadius->value());
	mSettings.insert("preyReproductiveCapacity", spinBoxPreyReproductiveCapacity->value());

	// Insert predator data
	mSettings.insert("initialNumberOfPredators", lineEditInitialPredators->text().toInt());
	mSettings.insert("predatorCognitiveFactor", lineEditCognitiveFactor->text().toFloat());
	mSettings.insert("predatorSocialFactor", lineEditSocialFactor->text().toFloat());
	mSettings.insert("predatorMaximumSpeed", spinBoxMaxSpeed->value());
	mSettings.insert("predatorReproductiveCapacity", spinBoxPredatorReproductiveCapacity->value());
	mSettings.insert("predatorReproductionRadius", spinBoxPredatorReproductionRadius->value());
	mSettings.insert("predatorSocialRadius", spinBoxSocialRadius->value());
	mSettings.insert("initialInertiaWeight", lineEditInitialWeight->text().toFloat());
	mSettings.insert("finalInertiaWeight", lineEditFinalWeight->text().toFloat());

	// Insert path to results file
	mSettings.insert("resultsFilePath", lineEditPath->text());

	emit settingsChanged(mSettings);

	QDialog::accept();
}