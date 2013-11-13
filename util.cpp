#include <QCoreApplication>
#include <QFileDialog>
#include <QFile>
#include <QDir>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include "util.h"
#include "localcapso.h"
#include "globalcapso.h"

namespace util
{
    bool loadSettings(CellularAutomaton* ca, CaType type, QString settingsFilename,
                      QString& currFilename)
    {
        QFile settingsFile;
        settingsFile.setFileName(settingsFilename);

        if(!settingsFile.open(QIODevice::ReadOnly))
        {
            return false;
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

                        local->setPreyInitialDensity(value.toFloat());
                    }
                    else if(elementName == "competitionFactor")
                    {
                        QString value = reader.readElementText();

                        local->setPreyCompetitionFactor(value.toFloat());
                    }
                    else if(elementName == "preyReproductionRadius")
                    {
                        QString value = reader.readElementText();

                        local->setPreyReproductionRadius(value.toInt());
                    }
                    else if(elementName == "preyReproductiveCapacity")
                    {
                        QString value = reader.readElementText();

                        local->setPreyReproductiveCapacity(value.toInt());
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

                        local->setPredatorReproductiveCapacity(value.toInt());
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
            return false;
        }

        settingsFile.close();

        return true;
    }


    bool writeSettings(QString filename, CaType type, float psi0, float alpha,
                       int ry, int ey, int rc, int z0, float k1, float k2,
                       int maxSpeed, int ez, int rz, int l, float omegaStart,
                       float omegaEnd)
    {
        QFile settingsFile;
        settingsFile.setFileName("settings.xml");

        if(!settingsFile.open(QIODevice::WriteOnly))
        {
            return false;
        }

        QXmlStreamWriter writer(&settingsFile);
        writer.setAutoFormatting(true);
        writer.writeStartDocument();
        writer.writeStartElement("project");

        switch(type)
        {
        case LOCAL:
            writer.writeAttribute("type", "LOCAL");
            break;
        case GLOBAL:
            writer.writeAttribute("type", "GLOBAL");
            break;
        case MOVEMENT:
            writer.writeAttribute("type", "MOVEMENT");
            break;
        }

        writer.writeTextElement("initialNumberOfPreys", QString::number(psi0));
        writer.writeTextElement("competitionFactor", QString::number(alpha));
        writer.writeTextElement("preyReproductionRadius", QString::number(ry));
        writer.writeTextElement("preyReproductiveCapacity", QString::number(ey));
        writer.writeTextElement("fitnessRadius", QString::number(rc));
        writer.writeTextElement("initialNumberOfPredators", QString::number(z0));
        writer.writeTextElement("predatorCognitiveFactor", QString::number(k1));
        writer.writeTextElement("predatorSocialFactor", QString::number(k2));
        writer.writeTextElement("predatorMaximumSpeed", QString::number(maxSpeed));
        writer.writeTextElement("predatorReproductiveCapacity", QString::number(ez));
        writer.writeTextElement("predatorReproductionRadius", QString::number(rz));
        writer.writeTextElement("predatorSocialRadius", QString::number(l));
        writer.writeTextElement("initialInertiaWeight", QString::number(omegaStart));
        writer.writeTextElement("finalInertiaWeight", QString::number(omegaEnd));
        writer.writeTextElement("resultsFilePath", filename);
        writer.writeEndElement();
        writer.writeEndDocument();

        settingsFile.close();

        return true;
    }

    bool getPathFromDialog(QString& path)
    {
        path = QFileDialog::getExistingDirectory(NULL,
                                                 QObject::tr("Select a folder"),
                                                 QCoreApplication::applicationDirPath() + QDir::separator(),
                                                 QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        if(path.isEmpty())
        {
            return false;
        }

        return true;
    }

    bool getFileFromDialog(QString& file, QString filter)
    {
        QString appDir = QCoreApplication::applicationDirPath() + QDir::separator();

        file = QFileDialog::getOpenFileName(NULL, QObject::tr("Select a file"),
                                            appDir, filter, 0, 0);

        if(file.isEmpty())
        {
            return false;
        }

        return true;
    }
}
