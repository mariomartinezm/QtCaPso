#pragma once

#include <QString>
#include "catype.h"
#include "cellularautomaton.h"

namespace util
{
    struct CaPsoSettings
    {
        float initialPreyDensity         = { 0.3F };
        float competitionFactor          = { 0.3F };
        int preyReproductionRadius       = { 2 };
        int preyReproductiveCapacity     = { 10 };
        int fitnessRadius                = { 3 };
        int initialPredatorSwarmSize     = { 3 };
        float predatorCognitiveFactor    = { 1.0F };
        float predatorSocialFactor       = { 2.0F };
        int predatorMaxSpeed             = { 10 };
        int predatorReproductiveCapacity = { 10 };
        int predatorReproductionRadius   = { 2 };
        int predatorSocialRadius         = { 3 };
        float initialInertiaWeight       = { 0.9F };
        float finalInertiaWeight         = { 0.2F };
    };

    bool loadSettings(CellularAutomaton* ca, CaType type, QString settingsFilename);

    bool writeSettings(CaPsoSettings& settings, CaType type=LOCAL);

    bool getPathFromDialog(QString& path);

    bool getFileFromDialog(QString& file, QString filter);
}
