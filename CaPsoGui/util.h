#pragma once

#include <QString>
#include "capsosettings.h"
#include "cellularautomaton.h"

namespace util
{
    bool loadSettings(capso::Settings& settings, QString settingsFilename);

    bool writeSettings(capso::Settings& settings, capso::CaType type=capso::LOCAL);

    bool getPathFromDialog(QString& path);

    bool getFileFromDialog(QString& file, QString filter);
}
