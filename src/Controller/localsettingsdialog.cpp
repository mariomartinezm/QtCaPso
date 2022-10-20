#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include "localsettingsdialog.h"

LocalSettingsDialog::LocalSettingsDialog(CaPsoSettings& settings, QWidget *parent) :
    QDialog(parent),
    mSettings(settings)
{
    this->setupUi(this);
}

LocalSettingsDialog::~LocalSettingsDialog()
{

}

void LocalSettingsDialog::showEvent(QShowEvent*)
{
    lineEditInitialPreys->setText(QString::number(mSettings.initialPreyDensity));
    lineEditCompetitionFactor->setText(QString::number(mSettings.competitionFactor));
    spinBoxPreyReproductionRadius->setValue(mSettings.preyReproductionRadius);
    spinBoxPreyReproductiveCapacity->setValue(mSettings.preyReproductiveCapacity);
    spinBoxFitnessRadius->setValue(mSettings.fitnessRadius);
    lineEditInitialPredators->setText(QString::number(mSettings.predatorInitialSwarmSize));
    lineEditCognitiveFactor->setText(QString::number(mSettings.predatorCognitiveFactor));
    lineEditSocialFactor->setText(QString::number(mSettings.predatorSocialFactor));
    spinBoxMaxSpeed->setValue(mSettings.predatorMaxSpeed);
    spinBoxPredatorReproductiveCapacity->setValue(mSettings.predatorReproductiveCapacity);
    spinBoxPredatorReproductionRadius->setValue(mSettings.predatorReproductionRadius);
    spinBoxSocialRadius->setValue(mSettings.predatorSocialRadius);
    lineEditInitialWeight->setText(QString::number(mSettings.initialInertiaWeight));
    lineEditFinalWeight->setText(QString::number(mSettings.finalInertiaWeight));
}

void LocalSettingsDialog::accept()
{
    // Load new values into CaPso settings
    mSettings.initialPreyDensity           = lineEditInitialPreys->text().toFloat();
    mSettings.competitionFactor            = lineEditCompetitionFactor->text().toFloat();
    mSettings.preyReproductionRadius       = spinBoxPreyReproductionRadius->value();
    mSettings.preyReproductiveCapacity     = spinBoxPreyReproductiveCapacity->value();
    mSettings.fitnessRadius                = spinBoxFitnessRadius->value();
    mSettings.predatorInitialSwarmSize     = lineEditInitialPredators->text().toInt();
    mSettings.predatorCognitiveFactor      = lineEditCognitiveFactor->text().toFloat();
    mSettings.predatorSocialFactor         = lineEditSocialFactor->text().toFloat();
    mSettings.predatorMaxSpeed             = spinBoxMaxSpeed->value();
    mSettings.predatorReproductiveCapacity = spinBoxPredatorReproductiveCapacity->value();
    mSettings.predatorReproductionRadius   = spinBoxPredatorReproductionRadius->value();
    mSettings.predatorSocialRadius         = spinBoxSocialRadius->value();
    mSettings.initialInertiaWeight         = lineEditInitialWeight->text().toFloat();
    mSettings.finalInertiaWeight           = lineEditFinalWeight->text().toFloat();

    emit settingsChanged();

    QDialog::accept();
}
