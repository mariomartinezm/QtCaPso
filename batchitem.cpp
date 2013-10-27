#include "batchitem.h"

BatchItem::BatchItem() :
    mSettingsFile(""),
    mWidth(0),
    mHeight(0),
    mNumberOfSimulations(0),
    mNumberOfSeasons(0),
    mFilenamePrefix(""),
    mResultsPath("")
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
