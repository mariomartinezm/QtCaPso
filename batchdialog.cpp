#include <QFile>
#include <QTextStream>
#include <QDir>
#include "batchdialog.h"
#include "cellularautomaton.h"
#include "localcapso.h"
#include "globalcapso.h"
#include "util.h"

BatchDialog::BatchDialog(QWidget *parent, CaType type) :
    QDialog(parent), mType(type)
{
    this->setupUi(this);

    lineEditPath->setText(QCoreApplication::applicationDirPath() +
                          QDir::separator());

    connect(buttonBrowse, SIGNAL(clicked()), this, SLOT(showFileDialog()));

    // This two lines must go after the connect statements
    buttonStart->setAutoDefault(false);
    buttonStart->setEnabled(false);
}

void BatchDialog::on_buttonBrowseSettings_clicked()
{
    QString file;

    if(util::getFileFromDialog(file, tr("xml (*.xml)")))
    {
        lineEditSettingsFile->setText(file);
    }
}

void BatchDialog::on_buttonAdd_clicked()
{
    if(!lineEditSettingsFile->text().isEmpty())
    {
        // Get the size of the lattice from the combo box
        QString currentItem = comboSize->itemText(comboSize->currentIndex());
        QStringList size = currentItem.split('x', QString::SkipEmptyParts,
                                         Qt::CaseSensitive);

        int width, height;
        width = size.at(0).toInt();
        height = size.at(1).toInt();

        // Create a new batch item an add it to the list
        BatchItem item(lineEditSettingsFile->text(), width, height,
                       spinBoxSimulations->value(), spinBoxSeasons->value(),
                       lineEditFilenamePrefix->text(), lineEditPath->text());

        batchItems << item;

        listWidgetJobs->addItem(lineEditSettingsFile->text());

        buttonStart->setEnabled(true);
    }
}

void BatchDialog::on_buttonRemove_clicked()
{
    // Delete the corresponding item from the list
    batchItems.removeAt(listWidgetJobs->row(listWidgetJobs->currentItem()));

    // Delete the currently selected item, since Qt does not delete the item
    // when it is removed, it must be manually deleted
    QListWidgetItem* item;
    item = listWidgetJobs->takeItem(listWidgetJobs->row(listWidgetJobs->currentItem()));
    delete item;

    // If the QListWidget is empty, disable buttonStart
    if(listWidgetJobs->count() == 0)
    {
        buttonStart->setEnabled(false);
    }
}

void BatchDialog::on_buttonClear_clicked()
{
    batchItems.clear();

    listWidgetJobs->clear();

    buttonStart->setEnabled(false);
}

void BatchDialog::showFileDialog()
{
    QString path;

    if(util::getPathFromDialog(path))
    {
        lineEditPath->setText(path + QDir::separator());
    }
}

void BatchDialog::on_lineEditPath_textChanged(QString text)
{
    QDir path(text);

    if(!path.exists(text))
    {
        buttonStart->setEnabled(false);
    }
    else
    {
        buttonStart->setEnabled(true);
    }
}

void BatchDialog::on_buttonStart_clicked()
{
    switch(mType)
    {
    case LOCAL:
    {
        for (int index = 0; index < batchItems.count(); index++)
        {
            const BatchItem& currentItem = batchItems.at(index);

            LocalCaPso* local = new LocalCaPso(currentItem.width(),
                                               currentItem.height());

            // Dummy variable to call load settings, since the obtained path is
            // ignored
            QString path;
            util::loadSettings(local, LOCAL, currentItem.settingsFile(), path);

            for (int simCount = 0; simCount < currentItem.numberOfSimulations(); simCount++)
            {
                // Create results file
                QFile resultsFile(currentItem.resultsPath() +
                                  currentItem.filenamePrefix() + "_" +
                                  QString::number(simCount) + ".txt");

                QTextStream resultsStream(&resultsFile);
                resultsFile.open(QIODevice::WriteOnly | QIODevice::Text |
                                 QIODevice::Truncate);

                local->initialize();

                for(int genCount = 0; genCount < currentItem.numberOfSeasons() * 10; genCount++)
                {
                    if(!(genCount % 10))
                    {
                        resultsStream << genCount / 10 << " " <<
                                         local->numberOfPreys() << " " <<
                                         local->numberOfPredators() << "\n";
                    }

                    local->nextGen();
                }

                resultsFile.close();
            }

            delete local;
        }

        break;
    }
    case GLOBAL:
        break;
    case MOVEMENT:
        break;
    }
}
