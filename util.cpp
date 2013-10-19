#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include "util.h"
#include "localcapso.h"
#include "globalcapso.h"

namespace util
{
    void loadSettings(CellularAutomaton* ca, CaType type, QString& currFilename)
    {
        QFile settingsFile;
        settingsFile.setFileName("settings.xml");

        if(!settingsFile.open(QIODevice::ReadOnly))
        {

        }

        QXmlStreamReader reader(&settingsFile);

        switch(type)
        {
        case LOCAL:
        {
            auto local = dynamic_cast<LocalCaPso*>(ca);

            while(!reader.atEnd() && !reader.hasError())
            {
                reader.readNext();

                if(reader.isStartElement())
                {
                    QString elementName = reader.name().toString();

                    if(elementName == "initialNumberOfPreys")
                    {
                        QString value = reader.readElementText();

                        local->setInitialAlivePreys(value.toFloat());
                    }
                    else if(elementName == "competitionFactor")
                    {
                        QString value = reader.readElementText();

                        local->setCompetitionFactor(value.toFloat());
                    }
                    else if(elementName == "preyReproductionRadius")
                    {
                        QString value = reader.readElementText();

                        local->setPreyReproductionRadius(value.toInt());
                    }
                    else if(elementName == "preyReproductiveCapacity")
                    {
                        QString value = reader.readElementText();

                        local->setPreyMeanOffspring(value.toInt());
                    }
                    else if(elementName == "fitnessRadius")
                    {
                        QString value = reader.readElementText();

                        local->setFitnessRadius(value.toInt());
                    }
                    else if(elementName == "initialNumberOfPredators")
                    {
                        QString value = reader.readElementText();

                        local->setInitialSwarmSize(value.toInt());
                    }
                    else if(elementName == "predatorCognitiveFactor")
                    {
                        QString value = reader.readElementText();

                        local->setCognitiveFactor(value.toFloat());
                    }
                    else if(elementName == "predatorSocialFactor")
                    {
                        QString value = reader.readElementText();

                        local->setSocialFactor(value.toFloat());
                    }
                    else if(elementName == "predatorMaximumSpeed")
                    {
                        QString value = reader.readElementText();

                        local->setMaximumSpeed(value.toInt());
                    }
                    else if(elementName == "predatorReproductiveCapacity")
                    {
                        QString value = reader.readElementText();

                        local->setPredatorMeanOffspring(value.toInt());
                    }
                    else if(elementName == "predatorReproductionRadius")
                    {
                        QString value = reader.readElementText();

                        local->setPredatorReproductionRadius(value.toInt());
                    }
                    else if(elementName == "predatorSocialRadius")
                    {
                        QString value = reader.readElementText();

                        local->setSocialRadius(value.toInt());
                    }
                    else if(elementName == "initialInertiaWeight")
                    {
                        QString value = reader.readElementText();

                        local->setInitialInertialWeight(value.toFloat());
                    }
                    else if(elementName == "finalInertiaWeight")
                    {
                        QString value = reader.readElementText();

                        local->setFinalInertiaWeight(value.toFloat());
                    }
                    else if(elementName == "resultsFilePath")
                    {
                        currFilename = reader.readElementText();
                    }
                }
            }
            break;
        }
        case GLOBAL:
            break;
        case MOVEMENT:
            break;
        }

        if(reader.hasError())
        {

        }

        settingsFile.close();
    }
}
