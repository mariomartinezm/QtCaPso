#include "batchitem.h"

BatchItem::BatchItem(QString settingsFile, int width, int height,
                     int numberOfSimulations, int numberOfSeasons,
                     QString filenamePrefix, QString resultsPath) :
    mSettingsFile(settingsFile),
    mWidth(width),
    mHeight(height),
    mNumberOfSimulations(numberOfSimulations),
    mNumberOfSeasons(numberOfSeasons),
    mFilenamePrefix(filenamePrefix),
    mResultsPath(resultsPath)
{
}

const QString& BatchItem::settingsFile() const
{
    return mSettingsFile;
}

void BatchItem::setSettingsFile(QString file)
{
    mSettingsFile = file;
}

int BatchItem::width() const
{
    return mWidth;
}

void BatchItem::setWidth(int value)
{
    mWidth = value;
}

int BatchItem::height() const
{
    return mHeight;
}

void BatchItem::setHeight(int value)
{
    mHeight = value;
}

int BatchItem::numberOfSimulations() const
{
    return mNumberOfSimulations;
}

void BatchItem::setNumberOfSimulations(int value)
{
    mNumberOfSimulations = value;
}

int BatchItem::numberOfSeasons() const
{
    return mNumberOfSeasons;
}

const QString& BatchItem::filenamePrefix() const
{
    return mFilenamePrefix;
}

void BatchItem::setFilenamePrefix(QString prefix)
{
    mFilenamePrefix = prefix;
}

const QString& BatchItem::resultsPath() const
{
    return mResultsPath;
}

void BatchItem::setResultsPath(QString path)
{
    mResultsPath = path;
}
