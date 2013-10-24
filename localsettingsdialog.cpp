#pragma warning(push, 3)
#include <QFileDialog>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#pragma warning(pop)
#include <QMessageBox>
#include "localsettingsdialog.h"
#include "util.h"

LocalSettingsDialog::LocalSettingsDialog(QWidget *parent)
    : QDialog(parent)
{
    this->setupUi(this);

    connect(pushButtonBrowse, SIGNAL(clicked()), this, SLOT(showFileDialog()));
}

LocalSettingsDialog::~LocalSettingsDialog()
{

}

void LocalSettingsDialog::showEvent(QShowEvent*)
{
    // Populate the form
    QFile settingsFile;
    settingsFile.setFileName("settings.xml");

    if(!settingsFile.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(this, "Error!", "Cannot open file: " +
                              settingsFile.fileName());

        exit(1);
    }

    QXmlStreamReader reader(&settingsFile);

    while(!reader.atEnd() && !reader.hasError())
    {
        reader.readNext();

        if(reader.isStartElement())
        {
            QString elementName = reader.name().toString();

            if(elementName == "initialNumberOfPreys")
            {
                QString value = reader.readElementText();

                lineEditInitialPreys->setText(value);
            }
            else if(elementName == "competitionFactor")
            {
                QString value = reader.readElementText();

                lineEditCompetenceFactor->setText(value);
            }
            else if(elementName == "preyReproductionRadius")
            {
                QString value = reader.readElementText();

                spinBoxPreyReproductionRadius->setValue(value.toInt());
            }
            else if(elementName == "preyReproductiveCapacity")
            {
                QString value = reader.readElementText();

                spinBoxPreyReproductiveCapacity->setValue(value.toInt());
            }
            else if(elementName == "fitnessRadius")
            {
                QString value = reader.readElementText();

                spinBoxFitnessRadius->setValue(value.toInt());
            }
            else if(elementName == "initialNumberOfPredators")
            {
                QString value = reader.readElementText();

                lineEditInitialPredators->setText(value);
            }
            else if(elementName == "predatorCognitiveFactor")
            {
                QString value = reader.readElementText();

                lineEditCognitiveFactor->setText(value);
            }
            else if(elementName == "predatorSocialFactor")
            {
                QString value = reader.readElementText();

                lineEditSocialFactor->setText(value);
            }
            else if(elementName == "predatorMaximumSpeed")
            {
                QString value = reader.readElementText();

                spinBoxMaxSpeed->setValue(value.toInt());
            }
            else if(elementName == "predatorReproductiveCapacity")
            {
                QString value = reader.readElementText();

                spinBoxPredatorReproductiveCapacity->setValue(value.toInt());
            }
            else if(elementName == "predatorReproductionRadius")
            {
                QString value = reader.readElementText();

                spinBoxPredatorReproductionRadius->setValue(value.toInt());
            }
            else if(elementName == "predatorSocialRadius")
            {
                QString value = reader.readElementText();

                spinBoxSocialRadius->setValue(value.toInt());
            }
            else if(elementName == "initialInertiaWeight")
            {
                QString value = reader.readElementText();

                lineEditInitialWeight->setText(value);
            }
            else if(elementName == "finalInertiaWeight")
            {
                QString value = reader.readElementText();

                lineEditFinalWeight->setText(value);
            }
            else if(elementName == "resultsFilePath")
            {
                QString value = reader.readElementText();

                lineEditPath->setText(value);
            }
        }
    }

    if(reader.hasError())
    {
        QMessageBox::critical(this, "Error reading settings file",
                              reader.errorString());
    }

    settingsFile.close();
}

void LocalSettingsDialog::showFileDialog()
{
    QString path = QFileDialog::getExistingDirectory(this, tr("Select a folder"),
        QDir::separator() + QString("home"), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if(!path.isEmpty())
    {
        lineEditPath->setText(path + QDir::separator() + "results.txt");
    }
}

void LocalSettingsDialog::accept()
{
    if(!util::writeSettings(lineEditPath->text(), CaType::LOCAL,
                        lineEditInitialPreys->text().toFloat(),
                        lineEditCompetenceFactor->text().toFloat(),
                        spinBoxPreyReproductionRadius->value(),
                        spinBoxPreyReproductiveCapacity->value(),
                        spinBoxFitnessRadius->value(),
                        lineEditInitialPredators->text().toInt(),
                        lineEditCognitiveFactor->text().toFloat(),
                        lineEditSocialFactor->text().toFloat(),
                        spinBoxMaxSpeed->value(),
                        spinBoxPredatorReproductiveCapacity->value(),
                        spinBoxPredatorReproductionRadius->value(),
                        spinBoxSocialRadius->value(),
                        lineEditInitialWeight->text().toFloat(),
                        lineEditFinalWeight->text().toFloat()))
    {
         QMessageBox::critical(this, "Error!", "Settings cannot be written");
    }

    emit settingsChanged();

    QDialog::accept();
}
