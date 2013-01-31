#pragma warning(push, 3)
#include <QFileDialog>
#pragma warning(pop)
#include "localsettingsdialog.h"

LocalSettingsDialog::LocalSettingsDialog(QMap<QString, QVariant> settings,
                                         QWidget *parent)
    : QDialog(parent)
{
    this->setupUi(this);

    mSettings = settings;

    // Populate form
    lineEditInitialPreys->setText(mSettings["initialNumberOfPreys"].toString());
    lineEditCompetenceFactor->setText(mSettings["competenceFactor"].toString());
    spinBoxPreyReproductionRadius->setValue(mSettings["preyReproductionRadius"].toInt());
    spinBoxPreyReproductiveCapacity->setValue(mSettings["preyReproductiveCapacity"].toInt());
    lineEditInitialPredators->setText(mSettings["initialNumberOfPredators"].toString());
    lineEditCognitiveFactor->setText(mSettings["predatorCognitiveFactor"].toString());
    lineEditSocialFactor->setText(mSettings["predatorSocialFactor"].toString());
    spinBoxMaxSpeed->setValue(mSettings["predatorMaximumSpeed"].toInt());
    spinBoxPredatorReproductiveCapacity->setValue(mSettings["predatorReproductiveCapacity"].toInt());
    spinBoxPredatorReproductionRadius->setValue(mSettings["predatorReproductionRadius"].toInt());
    spinBoxSocialRadius->setValue(mSettings["predatorSocialRadius"].toInt());
    spinBoxFitnessRadius->setValue(mSettings["fitnessRadius"].toInt());
    lineEditInitialWeight->setText(mSettings["initialInertiaWeight"].toString());
    lineEditFinalWeight->setText(mSettings["finalInertiaWeight"].toString());

    lineEditPath->setText(mSettings["resultsFilePath"].toString());

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
    mSettings["initialNumberOfPreys"] =  lineEditInitialPreys->text().toFloat();
    mSettings["competenceFactor"] =  lineEditCompetenceFactor->text().toFloat();
    mSettings["preyReproductionRadius"] = spinBoxPreyReproductionRadius->value();
    mSettings["preyReproductiveCapacity"] = spinBoxPreyReproductiveCapacity->value();

	// Insert predator data
    mSettings["initialNumberOfPredators"] = lineEditInitialPredators->text().toInt();
    mSettings["predatorCognitiveFactor"] = lineEditCognitiveFactor->text().toFloat();
    mSettings["predatorSocialFactor"] = lineEditSocialFactor->text().toFloat();
    mSettings["predatorMaximumSpeed"] = spinBoxMaxSpeed->value();
    mSettings["predatorReproductiveCapacity"] = spinBoxPredatorReproductiveCapacity->value();
    mSettings["predatorReproductionRadius"] = spinBoxPredatorReproductionRadius->value();
    mSettings["predatorSocialRadius"] = spinBoxSocialRadius->value();
    mSettings["fitnessRadius"] = spinBoxFitnessRadius->value();
    mSettings["initialInertiaWeight"] = lineEditInitialWeight->text().toFloat();
    mSettings["finalInertiaWeight"] = lineEditFinalWeight->text().toFloat();

	// Insert path to results file
    mSettings["resultsFilePath"] = lineEditPath->text();

	emit settingsChanged(mSettings);

	QDialog::accept();
}
