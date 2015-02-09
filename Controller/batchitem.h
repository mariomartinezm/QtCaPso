#ifndef BATCHITEM_H
#define BATCHITEM_H

#include <QString>

class BatchItem
{
public:
    BatchItem(QString settingsFile, int width, int height,
              int numberOfSimulations, int numberOfSeasons,
              QString filenamePrefix, QString resultsPath);

    const QString& settingsFile() const;
    void setSettingsFile(QString file);

    int width() const;
    void setWidth(int value);

    int height() const;
    void setHeight(int value);

    int numberOfSimulations() const;
    void setNumberOfSimulations(int value);

    int numberOfSeasons() const;
    void setNumberOfSeasons(int value);

    const QString& filenamePrefix() const;
    void setFilenamePrefix(QString prefix);

    const QString& resultsPath() const;
    void setResultsPath(QString path);

private:
    QString mSettingsFile;
    int mWidth;
    int mHeight;
    int mNumberOfSimulations;
    int mNumberOfSeasons;
    QString mFilenamePrefix;
    QString mResultsPath;
};

#endif // BATCHITEM_H
