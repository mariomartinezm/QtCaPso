#pragma once

#include <QString>
#include "capsosettings.h"
#include "cellularautomaton.h"

namespace util
{
    bool loadSettings(CaPsoSettings& settings, QString settingsFilename);

    bool writeSettings(CaPsoSettings& settings, CaType type=LOCAL);

    bool getPathFromDialog(QString& path);

    bool getFileFromDialog(QString& file, QString filter);
}
