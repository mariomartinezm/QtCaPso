#ifndef CONTROLLER_H
#define CONTROLLER_H

#pragma warning(push, 3)
#pragma warning(disable:4251)
#pragma warning(disable:4800)
#include <QMainWindow>
#include <QDialog>
#include <QFile>
#include <QTextStream>
#include "ui_controller.h"
#pragma warning(pop)
#include "catype.h"
#include "cellularautomaton.h"
#include "caview.h"

class Controller : public QMainWindow, private Ui::ControllerClass
{
    Q_OBJECT

public:
    Controller(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~Controller();

protected:
    // put events here
    void timerEvent(QTimerEvent*);

private slots:
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

    // Support for a results file
    QString mCurrFileName;
    QFile mResultsFile;
    QTextStream mResultsStream;

    int mTimerId;
    int mTimerCount;
    int mSeasonLength;
    int mPreyCountBeforeReproduction;
    int mPredatorCountBeforeReproduction;
};

#endif // CONTROLLER_H
