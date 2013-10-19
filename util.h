#ifndef UTIL_H
#define UTIL_H

#include "catype.h"
#include "cellularautomaton.h"
#include "localcapso.h"
#include "globalcapso.h"

namespace util
{
    void loadSettings(CellularAutomaton& ca, CaType type);
}

#endif // UTIL_H
