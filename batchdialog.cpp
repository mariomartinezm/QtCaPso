#include <QFileDialog>
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

    lineEditPath->setText(QCoreApplication::applicationDirPath() + QDir::separator());

    connect(buttonStart, SIGNAL(clicked()), this, SLOT(startBatch()));
    connect(buttonBrowse, SIGNAL(clicked()), this, SLOT(showFileDialog()));
}

void BatchDialog::showFileDialog()
{
    QString path = QFileDialog::getExistingDirectory(this,
                                                     tr("Select a folder"),
                                                     QCoreApplication::applicationDirPath() + QDir::separator(),
                                                     QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(!path.isEmpty())
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

void BatchDialog::startBatch()
{
    // Get the size of the lattice from the combo box
    QString currentItem = comboSize->itemText(comboSize->currentIndex());
    QStringList size = currentItem.split('x',
                                         QString::SplitBehavior::SkipEmptyParts,
                                         Qt::CaseSensitive);

    int width, height;
    width = size.at(0).toInt();
    height = size.at(1).toInt();

    switch(mType)
    {
    case LOCAL:
    {
        LocalCaPso* local = new LocalCaPso(width, height);
        QString path;

        // The path obtained here is ignored
        util::loadSettings(local, CaType::LOCAL, path);

        for (int simCount = 0; simCount < spinBoxSimulations->value(); simCount++)
        {
            // Create results file
            QFile resultsFile(lineEditPath->text() +
                              lineEditFilenamePrefix->text() + "_" +
                              QString::number(simCount) + ".txt");

            QTextStream resultsStream(&resultsFile);
            resultsFile.open(QIODevice::WriteOnly | QIODevice::Text |
                             QIODevice::Truncate);

            local->initialize();

            for(int genCount = 0; genCount < spinBoxSeasons->value() * 10; genCount++)
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

        break;
    }
    case GLOBAL:
        break;
    case MOVEMENT:
        break;
    }
}
