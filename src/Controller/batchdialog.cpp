#include <QProgressDialog>
#include <QtConcurrentMap>
#include <QFutureWatcher>
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

    lineEditPath->setText(QCoreApplication::applicationDirPath() + "/");

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

void BatchDialog::on_buttonBrowse_clicked()
{
    QString path;

    if(util::getPathFromDialog(path))
    {
        lineEditPath->setText(path + "/");
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

void BatchDialog::processItem(BatchItem& batchItem)
{
    LocalCaPso* localCaPso = new LocalCaPso(batchItem.width(), batchItem.height());

    util::loadSettings(localCaPso, LOCAL, batchItem.settingsFile());

    for (int simIndex = 0, fileIndex = 0; simIndex < batchItem.numberOfSimulations(); ++simIndex, ++fileIndex)
    {
        // Initialize filename for results file
        QString filename = batchItem.resultsPath() + batchItem.filenamePrefix() +
                "_" + QString::number(fileIndex) + ".csv";

        // If file exists increase index and build a new filename
        while(QFile::exists(filename))
        {
            ++fileIndex;
            filename = batchItem.resultsPath() + batchItem.filenamePrefix() +
                "_" + QString::number(fileIndex) + ".csv";
        }

        QFile resultsFile(filename);

        QTextStream resultsStream(&resultsFile);
        resultsFile.open(QIODevice::WriteOnly | QIODevice::Text |
                         QIODevice::Truncate);

        localCaPso->initialize();

        int preyCountBeforeReproduction = 0;
        int predatorCountBeforeReproduction = 0;
        int preyCountBeforePredatorDeath = 0;
        int predatorCountBeforePreyDeath = 0;

        // Write header containing columns names
        resultsStream << "Season," <<
                          "Preys," <<
                          "Predators," <<
                          "PreyCountBeforeReproduction," <<
                          "PreyBirthRate," <<
                          "PredatorCountBeforeReproduction," <<
                          "PredatorBirthRate," <<
                          "PreyCountBeforePredatorDeath," <<
                          "PredatorDeathProbability," <<
                          "PredatorCountBeforePreyDeath," <<
                          "PreyDeathProbability\n";

        for(int genCount = 0; genCount < batchItem.numberOfSeasons() * 10; genCount++)
        {
            switch(localCaPso->currentStage())
            {
            case 2:
                predatorCountBeforeReproduction = localCaPso->numberOfPredators();
                break;
            case 3:
                preyCountBeforePredatorDeath = localCaPso->numberOfPreys();
                break;
            case 4:
                predatorCountBeforePreyDeath = localCaPso->numberOfPredators();
                break;
            case 5:
                preyCountBeforeReproduction = localCaPso->numberOfPreys();
                break;
            }

            if(!(genCount % 10))
            {
                resultsStream << genCount / 10 << "," <<
                                 localCaPso->numberOfPreys() << "," <<
                                 localCaPso->numberOfPredators() << "," <<
                                 preyCountBeforeReproduction << "," <<
                                 localCaPso->preyBirthRate() << "," <<
                                 predatorCountBeforeReproduction << "," <<
                                 localCaPso->predatorBirthRate() << "," <<
                                 preyCountBeforePredatorDeath << "," <<
                                 localCaPso->predatorDeathProbability() << "," <<
                                 predatorCountBeforePreyDeath << "," <<
                                 localCaPso->preyDeathProbability() << "\n";
            }

            localCaPso->nextGen();
        }

        resultsFile.close();
    }

    delete localCaPso;
}

void BatchDialog::on_buttonStart_clicked()
{
    QProgressDialog progressDialog;
    progressDialog.setLabelText("Processing jobs. Please wait.");

    QFutureWatcher<void> futureWatcher;
    connect(&futureWatcher, SIGNAL(finished()),
            &progressDialog, SLOT(reset()));
    connect(&futureWatcher, SIGNAL(progressRangeChanged(int,int)),
            &progressDialog, SLOT(setRange(int,int)));
    connect(&futureWatcher, SIGNAL(progressValueChanged(int)),
            &progressDialog, SLOT(setValue(int)));
    connect(&progressDialog, SIGNAL(canceled()),
            &futureWatcher, SLOT(cancel()));

    // Start the computation. To be able to use a member function in the call to
    // map(), an instance to the containing class is needed, i.e., the 'this'
    // pointer. A lambda is used hwere to provide such pointer.
    futureWatcher.setFuture(QtConcurrent::map(batchItems, [this](BatchItem& batchItem)
    {
        processItem(batchItem);
    }));


    // Display the dialog and start the event loop
    progressDialog.exec();

    futureWatcher.waitForFinished();

    // Query the future to check if was canceled
    qDebug() << "Canceled?" << futureWatcher.future().isCanceled();
}
