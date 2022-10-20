#include "globalsettingsdialog.h"

GlobalSettingsDialog::GlobalSettingsDialog(QWidget* parent)
    : QDialog(parent)
{
    this->setupUi(this);
}

GlobalSettingsDialog::~GlobalSettingsDialog()
{

}

void GlobalSettingsDialog::accept()
{
    // Insert prey data
    mSettings.insert("initialNumberOfPreys", lineEditInitialPreys->text().toFloat());
    mSettings.insert("competenceFactor", lineEditCompetitionFactor->text().toFloat());
    mSettings.insert("preyReproductionRadius", spinBoxPreyReproductionRadius->value());
    mSettings.insert("preyReproductiveCapacity", spinBoxPreyReproductiveCapacity->value());
    mSettings.insert("competenceRadius", spinBoxCompetitionRadius->value());

    // Insert predator data
    mSettings.insert("initialNumberOfPredators", lineEditInitialPredators->text().toInt());
    mSettings.insert("predatorCognitiveFactor", lineEditCognitiveFactor->text().toFloat());
    mSettings.insert("predatorSocialFactor", lineEditSocialFactor->text().toFloat());
    mSettings.insert("predatorMaximumSpeed", spinBoxMaxSpeed->value());
    mSettings.insert("predatorReproductiveCapacity", spinBoxPredatorReproductiveCapacity->value());
    mSettings.insert("predatorReproductionRadius", spinBoxPredatorReproductionRadius->value());
    mSettings.insert("initialInertiaWeight", lineEditInitialWeight->text().toFloat());
    mSettings.insert("finalInertiaWeight", lineEditFinalWeight->text().toFloat());

    emit settingsChanged(mSettings);

    QDialog::accept();
}
