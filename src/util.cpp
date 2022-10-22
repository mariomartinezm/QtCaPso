#include <QCoreApplication>
#include <QFileDialog>
#include <QFile>
#include <QDir>
#include <QJsonObject>
#include <QJsonDocument>
#include "util.h"
#include "localcapso.h"
#include "globalcapso.h"

namespace util
{
    bool loadSettings(CaPsoSettings& settings, QString settingsFilename)
    {
        QFile settingsFile;
        settingsFile.setFileName(settingsFilename);

        if(!settingsFile.open(QIODevice::ReadOnly))
        {
            return false;
        }

        QByteArray fileData = settingsFile.readAll();
        QJsonDocument fileDoc(QJsonDocument::fromJson(fileData));
        QJsonObject json = fileDoc.object();

        QString key = "initialNumberOfPreys";
        if (json.contains(key) && json[key].isDouble())
        {
            settings.initialPreyDensity = json[key].toDouble();
        }

        key = "competitionFactor";
        if (json.contains(key) && json[key].isDouble())
        {
            settings.competitionFactor = json[key].toDouble();
        }

        key = "preyReproductionRadius";
        if (json.contains(key) && json[key].isDouble())
        {
            settings.preyReproductionRadius = json[key].toInt();
        }

        key = "preyReproductiveCapacity";
        if (json.contains(key) && json[key].isDouble())
        {
            settings.preyReproductiveCapacity = json[key].toInt();
        }

        key = "fitnessRadius";
        if (json.contains(key) && json[key].isDouble())
        {
            settings.fitnessRadius = json[key].toInt();
        }

        key = "initialNumberOfPredators";
        if (json.contains(key) && json[key].isDouble())
        {
            settings.predatorInitialSwarmSize = json[key].toInt();
        }

        key = "predatorCognitiveFactor";
        if (json.contains(key) && json[key].isDouble())
        {
            settings.predatorCognitiveFactor = json[key].toDouble();
        }

        key = "predatorSocialFactor";
        if (json.contains(key) && json[key].isDouble())
        {
            settings.predatorSocialFactor = json[key].toDouble();
        }

        key = "predatorMaximumSpeed";
        if (json.contains(key) && json[key].isDouble())
        {
            settings.predatorMaxSpeed = json[key].toInt();
        }

        key = "predatorReproductiveCapacity";
        if (json.contains(key) && json[key].isDouble())
        {
            settings.predatorReproductiveCapacity = json[key].toInt();
        }

        key = "predatorReproductionRadius";
        if (json.contains(key) && json[key].isDouble())
        {
            settings.predatorReproductionRadius = json[key].toInt();
        }

        key = "predatorSocialRadius";
        if (json.contains(key) && json[key].isDouble())
        {
            settings.predatorSocialRadius = json[key].toInt();
        }

        key = "initialInertiaWeight";
        if (json.contains(key) && json[key].isDouble())
        {
            settings.initialInertiaWeight = json[key].toDouble();
        }

        key = "finalInertiaWeight";
        if (json.contains(key) && json[key].isDouble())
        {
            settings.finalInertiaWeight = json[key].toDouble();
        }

        settingsFile.close();

        return true;
    }


    bool writeSettings(CaPsoSettings& settings, CaType type)
    {
        QFile settingsFile;
        settingsFile.setFileName("settings.json");

        if(!settingsFile.open(QIODevice::WriteOnly))
        {
            return false;
        }

        QJsonObject json;

        switch(type)
        {
        case LOCAL:
            json["type"] = "LOCAL";
            break;
        case GLOBAL:
            json["type"] = "GLOBAL";
            break;
        case MOVEMENT:
            json["type"] = "MOVEMENT";
            break;
        }

        json["initialNumberOfPreys"]         = settings.initialPreyDensity;
        json["competitionFactor"]            = settings.competitionFactor;
        json["preyReproductionRadius"]       = settings.preyReproductionRadius;
        json["preyReproductiveCapacity"]     = settings.preyReproductiveCapacity;
        json["fitnessRadius"]                = settings.fitnessRadius;
        json["initialNumberOfPredators"]     = settings.predatorInitialSwarmSize;
        json["predatorCognitiveFactor"]      = settings.predatorCognitiveFactor;
        json["predatorSocialFactor"]         = settings.predatorSocialFactor;
        json["predatorMaximumSpeed"]         = settings.predatorMaxSpeed;
        json["predatorReproductiveCapacity"] = settings.predatorReproductiveCapacity;
        json["predatorReproductionRadius"]   = settings.predatorReproductionRadius;
        json["predatorSocialRadius"]         = settings.predatorSocialRadius;
        json["initialInertiaWeight"]         = settings.initialInertiaWeight;
        json["finalInertiaWeight"]           = settings.finalInertiaWeight;

        settingsFile.write(QJsonDocument(json).toJson());

        settingsFile.close();

        return true;
    }

    bool getPathFromDialog(QString& path)
    {
        path = QFileDialog::getExistingDirectory(NULL,
                                                 QObject::tr("Select a folder"),
                                                 QCoreApplication::applicationDirPath() + "/",
                                                 QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        if(path.isEmpty())
        {
            return false;
        }

        return true;
    }

    bool getFileFromDialog(QString& file, QString filter)
    {
        QString appDir = QCoreApplication::applicationDirPath() + "/";

        file = QFileDialog::getOpenFileName(NULL, QObject::tr("Select a file"),
                                            appDir, filter, 0, 0);

        if(file.isEmpty())
        {
            return false;
        }

        return true;
    }
}
