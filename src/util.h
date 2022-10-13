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

    bool writeSettings(CaType type=LOCAL,
                       float psi0=0.3, float alpha=0.3, int ry=2,
                       int ey=10, int rc=3, int z0=3, float k1=1.0,
                       float k2=2.0, int maxSpeed=10, int ez=10, int rz=2,
                       int l=3, float omegaStart=0.9, float omegaEnd=0.2);

    bool getPathFromDialog(QString& path);

    bool getFileFromDialog(QString& file, QString filter);
}
