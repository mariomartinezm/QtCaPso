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

    connect(buttonStart, SIGNAL(clicked()), this, SLOT(startBatch()));
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

        util::loadSettings(local, CaType::LOCAL, path);

        // Remove filename from path
        path.truncate(path.lastIndexOf(QDir::separator()) + 1);

        for (int simCount = 0; simCount < spinBoxSimulations->value(); simCount++)
        {
            // Create results file
            QFile resultsFile(path + lineEditFilenamePrefix->text() + "_" +
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
