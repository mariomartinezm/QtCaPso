#ifndef UTIL_H
#define UTIL_H

#include "catype.h"
#include "cellularautomaton.h"

namespace util
{
    bool loadSettings(CellularAutomaton* ca, CaType type, QString& currFilename);

    bool writeSettings(CaType type, float psi0=0.3, float alpha=0.3, int ry=2,
                       int ey=10, int rc=10, int z0=3, float k1=1.0,
                       float k2=2.0, int maxSpeed=10, int ez=10, int rz=3,
                       int l=3, float omegaStart=0.9, float omegaEnd=0.2,
                       QString filename="results.txt");
}

#endif // UTIL_H
