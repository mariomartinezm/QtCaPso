#include <QDebug>
#include <QCloseEvent>
#include <QFileDialog>
#include <QMessageBox>
#include "controller.h"
#include "localcapso.h"
#include "localsettingsdialog.h"
#include "globalsettingsdialog.h"
#include "batchdialog.h"
#include "globalcapso.h"
#include "util.h"

Controller::Controller(QWidget *parent)
    : QMainWindow(parent),
    mCurrentType(capso::LOCAL),
    mResultsFilename("capso.csv"),
    mResultsFile(mResultsFilename),
    mResultsStream(&mResultsFile),
    mTimerId(-1),
    mTimerCount(0),
    mSeasonLength(10),
    mPreyCountBeforeReproduction(0),
    mPredatorCountBeforeReproduction(0),
    mPredatorCountBeforePreyDeath(0),
    mPreyCountBeforePredatorDeath(0)
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
    if(mCurrentType == capso::LOCAL)
    {
        auto local = dynamic_cast<capso::Local*>(mCellularAutomaton);

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
        mResultsSaved = false;
    }

    statusBarGeneration->showMessage(QString::number(mTimerCount));

    mView->update();
}

void Controller::closeEvent(QCloseEvent* event)
{
    if(!mResultsSaved)
    {
        QMessageBox::StandardButton resButton =
                QMessageBox::warning(this, "QtCaPso",
                                     tr("Save results before closing?"),
                                     QMessageBox::Yes | QMessageBox::No);

        if(resButton != QMessageBox::No)
        {
            // The user pressed "Yes" we attempt to save the file
            save();
        }
        else
        {
            // The user pressed "No" so we can assume nothing will be lost
            mResultsSaved = true;
        }
    }

    if(!mResultsSaved)
    {
        event->ignore();
    }
    else
    {
        event->accept();
    }
}

void Controller::save()
{
    pause();

    QString error = "";
    QString filename = QFileDialog::getSaveFileName(this, "Save results file",
        QCoreApplication::applicationDirPath() + "/" +
        "capso.csv", tr("csv (*.csv)"));

    if(!filename.isEmpty())
    {
        if(QFile::exists(filename) && !QFile::remove(filename))
        {
            error = "Error removing file: " + filename;
        }
        else
        {
            if(!QFile::copy("capso.csv", filename))
            {
                error = "Cannot save file: " + filename;
            }
            else
            {
                mResultsSaved = true;
            }
        }

        if(!error.isEmpty())
        {
            QMessageBox::critical(this, "Error!", error);
        }
    }
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
        mTimerId = -1;

        mResultsStream.flush();
        mResultsSaved = false;
    }
}

void Controller::step()
{
    if(mTimerId != -1)
    {
        killTimer(mTimerId);
        mTimerId = -1;
    }

    if(mCurrentType == capso::LOCAL)
    {
        auto local = dynamic_cast<capso::Local*>(mCellularAutomaton);

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
        mResultsSaved = false;
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
        QCoreApplication::applicationDirPath() + "/" +
        "lattice.PNG", tr("PNG (*.PNG);;JPG (*.jpg);; TIFF (*.tiff)"));

    if(!fileName.isEmpty())
    {
        mView->latticeImage().save(fileName);
    }
}

void Controller::updateSettings()
{
    if(!util::writeSettings(mSettings))
    {
        QMessageBox::critical(this, "Error!", "Cannot write settings");
    }

    auto local = dynamic_cast<capso::Local*>(mCellularAutomaton);
    local->setSettings(mSettings);
}

void Controller::exportSettings()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Export current settings",
        QCoreApplication::applicationDirPath() + "/" + "settings.json", tr("json (*.json)"));

    if(!fileName.isEmpty())
    {
        if(QFile::exists(fileName))
        {
            if(!QFile::remove(fileName))
            {
                QMessageBox::critical(this, "Error!", "Cannot remove file");
            }
        }

        if(!QFile::copy("settings.json", fileName))
        {
            QMessageBox::critical(this, "Error!", "Cannot copy file");
        }
    }
}

void Controller::importSettings()
{
    QString filename = QFileDialog::getOpenFileName(this, "Import settings", "",
                                                    tr("json (*.json)"));

    if(!filename.isEmpty())
    {
        util::loadSettings(mSettings, filename);

        QString originalFile = "settings.json";

        if(!QFile::remove(originalFile))
        {
            QMessageBox::critical(this, "Error!", "Cannot remove file");
        }
        else
        {
            if(!QFile::copy(filename, originalFile))
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
    case capso::GLOBAL:
        break;
    case capso::LOCAL:
        break;
    case capso::MOVEMENT:
        break;
    }

    if(!QFile::exists("settings.json"))
    {
        if(!util::writeSettings(mSettings))
        {
            QMessageBox::critical(this, "Error!", "Cannot write settings");
        }
    }

    util::loadSettings(mSettings, "settings.json");

    // Initialize CA according to settings file
    auto local = dynamic_cast<capso::Local*>(mCellularAutomaton);
    local->setSettings(mSettings);
    mCellularAutomaton->initialize();
}

void Controller::makeConnections()
{
    connect(actionSave, SIGNAL(triggered()), this, SLOT(save()));
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
    case capso::GLOBAL:
        mCellularAutomaton = new capso::Global(512, 512);
        mSeasonLength = 10;
        break;

    case capso::LOCAL:
        mCellularAutomaton = new capso::Local(512, 512);
        mSeasonLength = 10;
        break;

    case capso::MOVEMENT:
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
    case capso::GLOBAL:
        {
            mSettingsDialog = new GlobalSettingsDialog(this);
            auto p = dynamic_cast<GlobalSettingsDialog*>(mSettingsDialog);
            connect(p, SIGNAL(settingsChanged(QMap<QString,QVariant>)),
                this, SLOT(updateSettings(QMap<QString,QVariant>)));
        }
        break;

    case capso::LOCAL:
        {
            mSettingsDialog = new LocalSettingsDialog(mSettings, this);
            auto p = dynamic_cast<LocalSettingsDialog*>(mSettingsDialog);
            connect(p, SIGNAL(settingsChanged()),
                this, SLOT(updateSettings()));
        }
        break;

    case capso::MOVEMENT:
        break;
    }
}

void Controller::initializeResultsFile()
{
    mResultsFile.close();
    mResultsFile.setFileName(mResultsFilename);
    mResultsFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);
    mResultsStream.seek(0);

    // Write header containing columns names
    mResultsStream << "Season," <<
                      "Preys," <<
                      "Predators," <<
                      "PreyCountBeforeReproduction," <<
                      "PreyBirthRate," <<
                      "PredatorCountBeforeReproduction," <<
                      "PredatorBirthRate," <<
                      "PreyCountBeforePredatorDeath," <<
                      "PredatorDeathProbability," <<
                      "PredatorCountBeforePreyDeath," <<
                      "PreyDeathProbability\n";
}

void Controller::writeResults()
{
    switch(mCurrentType)
    {
    case capso::LOCAL:
        {
            auto local = dynamic_cast<capso::Local*>(mCellularAutomaton);
            mResultsStream << mTimerCount / mSeasonLength << "," <<
                              local->numberOfPreys() << "," <<
                              local->numberOfPredators() << "," <<
                              mPreyCountBeforeReproduction << "," <<
                              local->preyBirthRate() << "," <<
                              mPredatorCountBeforeReproduction << "," <<
                              local->predatorBirthRate() << "," <<
                              mPreyCountBeforePredatorDeath << "," <<
                              local->predatorDeathProbability() << "," <<
                              mPredatorCountBeforePreyDeath << "," <<
                              local->preyDeathProbability() << "\n";
        }
        break;

    case capso::GLOBAL:
        break;

    case capso::MOVEMENT:
        break;
    }
}
