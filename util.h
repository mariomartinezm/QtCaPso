#ifndef UTIL_H
#define UTIL_H

#include "catype.h"
#include "cellularautomaton.h"

namespace util
{
    void loadSettings(CellularAutomaton* ca, CaType type, QString& currFilename);
}

#endif // UTIL_H
