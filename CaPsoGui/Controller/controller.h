#pragma once

#include <QMainWindow>
#include <QDialog>
#include <QFile>
#include <QTextStream>
#include "ui_controller.h"
#include "capsosettings.h"
#include "cellularautomaton.h"
#include "caview.h"

class Controller : public QMainWindow, private Ui::ControllerClass
{
    Q_OBJECT

public:
    Controller(QWidget *parent = 0);
    ~Controller();

protected:
    // put events here
    void timerEvent(QTimerEvent*);
    void closeEvent(QCloseEvent* event);

private slots:
    void save();
    void play();
    void pause();
    void step();
    void clear();
    void initialize();
    void showSettings();
    void showBatchDialog();
    void exportBitmap();
    void updateSettings();
    void importSettings();
    void exportSettings();

private:
    void createCa();
    void initializeSettings();
    void makeConnections();
    void createView();
    void createSettingsDialog();
    void initializeResultsFile();
    void writeResults();

    CaType mCurrentType;
    CellularAutomaton* mCellularAutomaton;
    CaView* mView;
    QDialog* mSettingsDialog;
    CaPsoSettings mSettings;

    // Support for a results file
    QString     mResultsFilename;
    QFile       mResultsFile;
    QTextStream mResultsStream;
    bool        mResultsSaved = { false };

    int mTimerId;
    int mTimerCount;
    int mSeasonLength;
    int mPreyCountBeforeReproduction;
    int mPredatorCountBeforeReproduction;
    int mPredatorCountBeforePreyDeath;
    int mPreyCountBeforePredatorDeath;
};
