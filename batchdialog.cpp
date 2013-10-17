#include <QFile>
#include <QTextStream>
#include "batchdialog.h"
#include "cellularautomaton.h"
#include "localcapso.h"
#include "globalcapso.h"

BatchDialog::BatchDialog(QWidget *parent, CaType type) :
    QDialog(parent), mType(type)
{
    this->setupUi(this);

    connect(buttonStart, SIGNAL(clicked()), this, SLOT(startBatch()));
}

void BatchDialog::startBatch()
{
    switch(mType)
    {
    case LOCAL:
    {
        LocalCaPso local(512, 256);

        for (int simCount = 0; simCount < spinBoxSimulations->value(); simCount++)
        {
            // Create results file
            QFile resultsFile("D:\\data_" + QString("%1").arg(simCount) + ".txt");
            QTextStream resultsStream(&resultsFile);
            resultsFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);

            local.clear();

            for(int seasonsCount = 0; seasonsCount < spinBoxSeasons->value() * 10; seasonsCount++)
            {
                local.nextGen();

                resultsStream << seasonsCount << "\n";
            }

            resultsFile.close();
        }
    }
        break;
    case GLOBAL:
        break;
    case MOVEMENT:
        break;
    }
}
