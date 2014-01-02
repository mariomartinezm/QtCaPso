#pragma warning(push, 3)
#include <QFileDialog>
#pragma warning(pop)
#include <QMessageBox>
#include "controller.h"
#include "localcapso.h"
#include "localsettingsdialog.h"
#include "globalsettingsdialog.h"
#include "batchdialog.h"
#include "globalcapso.h"
#include "util.h"

Controller::Controller(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags),
    mCurrentType(LOCAL),
    mCurrFileName("results.txt"),
    mResultsFile(mCurrFileName),
    mResultsStream(&mResultsFile),
    mTimerId(-1),
    mTimerCount(0),
    mSeasonLength(10),
    mPreyCountBeforeReproduction(0),
    mPredatorCountBeforeReproduction(0),
    mPreyCountBeforePredatorDeath(0),
    mPredatorCountBeforePreyDeath(0)
{
    this->setupUi(this);

    createCa();
    initializeSettings();
    makeConnections();
    createView();
    createSettingsDialog();

    initializeResultsFile();
    writeResults();
}

Controller::~Controller()
{
    mResultsFile.close();

    delete mCellularAutomaton;
    delete mView;
}

void Controller::timerEvent(QTimerEvent*)
{
    if(mCurrentType == LOCAL)
    {
        auto local = dynamic_cast<LocalCaPso*>(mCellularAutomaton);

        switch(local->currentStage())
        {
        case 2:
            mPredatorCountBeforeReproduction = local->numberOfPredators();
            break;
        case 3:
            mPreyCountBeforePredatorDeath = local->numberOfPreys();
            break;
        case 4:
            mPredatorCountBeforePreyDeath = local->numberOfPredators();
            break;
        case 5:
            mPreyCountBeforeReproduction = local->numberOfPreys();
            break;
        }
    }

    mCellularAutomaton->nextGen();

    mTimerCount++;

    if(!(mTimerCount % mSeasonLength))
    {
        writeResults();
    }

    statusBarGeneration->showMessage(QString::number(mTimerCount));

    mView->update();
}

void Controller::play()
{
    if(mTimerId == -1)
    {
        mTimerId = startTimer(0);
    }
}

void Controller::pause()
{
    if(mTimerId != -1)
    {
        killTimer(mTimerId);

        mResultsStream.flush();

        mTimerId = -1;
    }
}

void Controller::step()
{
    if(mTimerId != -1)
    {
        killTimer(mTimerId);

        mTimerId = -1;
    }

    if(mCurrentType == LOCAL)
    {
        auto local = dynamic_cast<LocalCaPso*>(mCellularAutomaton);

        switch(local->currentStage())
        {
        case 2:
            mPredatorCountBeforeReproduction = local->numberOfPredators();
            break;
        case 3:
            mPreyCountBeforePredatorDeath = local->numberOfPreys();
            break;
        case 4:
            mPredatorCountBeforePreyDeath = local->numberOfPredators();
            break;
        case 5:
            mPreyCountBeforeReproduction = local->numberOfPreys();
            break;
        }
    }

    mCellularAutomaton->nextGen();

    mTimerCount++;

    if(!(mTimerCount % mSeasonLength))
    {
        writeResults();
    }

    statusBarGeneration->showMessage(QString::number(mTimerCount));

    mView->update();
}

void Controller::clear()
{
    if(mTimerId != -1)
    {
        killTimer(mTimerId);

        mTimerId = -1;
    }

    mCellularAutomaton->clear();

    mView->update();
}

void Controller::initialize()
{
    if(mTimerId != -1)
    {
        killTimer(mTimerId);

        mTimerId = -1;
    }

    mCellularAutomaton->initialize();

    mPreyCountBeforeReproduction = 0;
    mPredatorCountBeforeReproduction = 0;
    mPreyCountBeforePredatorDeath = 0;
    mPredatorCountBeforePreyDeath = 0;

    mTimerCount = 0;

    initializeResultsFile();

    writeResults();

    mView->update();
}

void Controller::showSettings()
{
    mSettingsDialog->exec();
}

void Controller::showBatchDialog()
{
    BatchDialog* batchDialog = new BatchDialog(this);
    batchDialog->exec();
}

void Controller::exportBitmap()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Export Bitmap",
        QCoreApplication::applicationDirPath() + QDir::separator() +
        "lattice.PNG", tr("PNG (*.PNG);;JPG (*.jpg);; TIFF (*.tiff)"));

    if(!fileName.isEmpty())
    {
        mView->latticeImage().save(fileName);
    }
}

void Controller::updateSettings()
{
    if(!util::loadSettings(mCellularAutomaton, mCurrentType, "settings.xml", mCurrFileName))
    {
        QMessageBox::critical(this, "Error!", "The settings file cannot be loaded");
    }
}

void Controller::exportSettings()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Export current settings",
        QCoreApplication::applicationDirPath() + QDir::separator() + "settings.xml", tr("xml (*.xml)"));

    if(!fileName.isEmpty())
    {
        if(QFile::exists(fileName))
        {
            if(!QFile::remove(fileName))
            {
                QMessageBox::critical(this, "Error!", "Cannot remove file");
            }
        }

        if(!QFile::copy("settings.xml", fileName))
        {
            QMessageBox::critical(this, "Error!", "Cannot copy file");
        }
    }
}

void Controller::importSettings()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Import settings", "",
                                                    tr("xml (*.xml)"));

    if(!fileName.isEmpty())
    {
        QString originalFile = "settings.xml";

        if(!QFile::remove(originalFile))
        {
            QMessageBox::critical(this, "Error!", "Cannot remove file");
        }
        else
        {
            if(!QFile::copy(fileName, originalFile))
            {
                QMessageBox::critical(this, "Error!", "Cannot copy new file");
            }
            else
            {
                updateSettings();
            }
        }
    }
}

void Controller::initializeSettings()
{
    switch(mCurrentType)
    {
    case GLOBAL:
        break;
    case LOCAL:
        break;
    case MOVEMENT:
        break;
    }

    if(!QFile::exists("settings.xml"))
    {
        if(!util::writeSettings(QCoreApplication::applicationDirPath() +
                            QDir::separator() + "results.txt"))
        {
            QMessageBox::critical(this, "Error!", "Cannot write settings");
        }
    }

    updateSettings();

    // Initialize CA according to the updated settings file
    mCellularAutomaton->initialize();
}

void Controller::makeConnections()
{
    connect(actionPlay, SIGNAL(triggered()), this, SLOT(play()));
    connect(actionPause, SIGNAL(triggered()), this, SLOT(pause()));
    connect(actionStep, SIGNAL(triggered()), this, SLOT(step()));
    connect(actionClear, SIGNAL(triggered()), this, SLOT(clear()));
    connect(actionInitialize, SIGNAL(triggered()), this, SLOT(initialize()));
    connect(actionSettings, SIGNAL(triggered()), this, SLOT(showSettings()));
    connect(actionBatch, SIGNAL(triggered()), this, SLOT(showBatchDialog()));
    connect(actionExportBitmap, SIGNAL(triggered()), this, SLOT(exportBitmap()));
    connect(actionImportSettings, SIGNAL(triggered()), this, SLOT(importSettings()));
    connect(actionExportSettings, SIGNAL(triggered()), this, SLOT(exportSettings()));
    connect(actionExit, SIGNAL(triggered()), this, SLOT(close()));
}

void Controller::createCa()
{
    switch(mCurrentType)
    {
    case GLOBAL:
        mCellularAutomaton = new GlobalCaPso(512, 256);
        mSeasonLength = 10;
        break;

    case LOCAL:
        mCellularAutomaton = new LocalCaPso(512, 256);
        mSeasonLength = 10;
        break;

    case MOVEMENT:
        break;
    }
}

void Controller::createView()
{
    mView = new CaView(mCellularAutomaton->latticeData(),
                       mCellularAutomaton->width(),
                       mCellularAutomaton->height(),
                       this);

    setCentralWidget(mView);
}

void Controller::createSettingsDialog()
{
    switch(mCurrentType)
    {
    case GLOBAL:
        {
            mSettingsDialog = new GlobalSettingsDialog(this);
            auto p = dynamic_cast<GlobalSettingsDialog*>(mSettingsDialog);
            connect(p, SIGNAL(settingsChanged(QMap<QString, QVariant>)),
                this, SLOT(updateSettings(QMap<QString, QVariant>)));
        }
        break;

    case LOCAL:
        {
            mSettingsDialog = new LocalSettingsDialog(this);
            auto p = dynamic_cast<LocalSettingsDialog*>(mSettingsDialog);
            connect(p, SIGNAL(settingsChanged()),
                this, SLOT(updateSettings()));
        }
        break;

    case MOVEMENT:
        break;
    }
}

void Controller::initializeResultsFile()
{
    mResultsFile.close();
    mResultsFile.setFileName(mCurrFileName);
    mResultsFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);
    mResultsStream.seek(0);
}

void Controller::writeResults()
{
    switch(mCurrentType)
    {
    case LOCAL:
        {
            auto local = dynamic_cast<LocalCaPso*>(mCellularAutomaton);
            mResultsStream << mTimerCount / mSeasonLength << " " <<
                              local->numberOfPreys() << " " <<
                              local->numberOfPredators() << " " <<
                              mPreyCountBeforeReproduction << " " <<
                              local->preyBirthRate() << " " <<
                              mPredatorCountBeforeReproduction << " " <<
                              local->predatorBirthRate() << " " <<
                              mPreyCountBeforePredatorDeath << " " <<
                              local->predatorDeathProbability() << " " <<
                              mPredatorCountBeforePreyDeath << " " <<
                              local->preyDeathProbability() << "\n";
        }
        break;
    }
}
