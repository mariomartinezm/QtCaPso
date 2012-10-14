#include <qfiledialog.h>
#include "controller.h"
#include "localcapso.h"
#include "localsettingsdialog.h"
#include "globalsettingsdialog.h"
#include "globalcapso.h"

Controller::Controller(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags),
	mCurrentType(LOCAL),
	mCurrFileName("results.txt"),
	mResultsFile(new QFile(mCurrFileName, this)),
	mResultsStream(mResultsFile),
	mTimerId(-1),
	mTimerCount(0),
	mSeasonLength(10)
{
	this->setupUi(this);

	makeConnections();
	createCa();
	createView();
	createSettingsDialog();
	initializeResultsFile();
	writeResults();
}

Controller::~Controller()
{
	mResultsFile->close();

	delete mCellularAutomaton;
	delete mView;
}

void Controller::timerEvent(QTimerEvent* e)
{
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

	mTimerCount = 0;

	initializeResultsFile();

	mView->update();
}

void Controller::showSettings()
{
	mSettingsDialog->exec();
}

void Controller::exportBitmap()
{
	QString fileName = QFileDialog::getSaveFileName(this, "Export Bitmap",
		QCoreApplication::applicationDirPath() + "/lattice.PNG", tr("PNG (*.PNG);;JPG (*.jpg);; TIFF (*.tiff)"));

	if(!fileName.isEmpty())
	{
		mView->latticeImage().save(fileName);
	}
}

void Controller::updateSettings(QMap<QString, QVariant> settings)
{
	switch(mCurrentType)
	{
	case GLOBAL:
		{
			auto globalDialog = dynamic_cast<GlobalCaPso*>(mCellularAutomaton);

			globalDialog->setInitialPreyPercentage(settings["initialNumberOfPreys"].toFloat());
			globalDialog->setCompetenceFactor(settings["competenceFactor"].toFloat());
			globalDialog->setCompetenceRadius(settings["competenceRadius"].toInt());
			globalDialog->setPreyReproductionRadius(settings["preyReproductionRadius"].toInt());
			globalDialog->setPreyMeanOffspring(settings["preyReproductiveCapacity"].toInt());

			globalDialog->setInitialSwarmSize(settings["initialNumberOfPredators"].toInt());
			globalDialog->setCognitiveFactor(settings["predatorCognitiveFactor"].toFloat());
			globalDialog->setSocialFactor(settings["predatorSocialFactor"].toFloat());
			globalDialog->setMaximumSpeed(settings["predatorMaximumSpeed"].toInt());
			globalDialog->setPredatorMeanOffspring(settings["predatorReproductiveCapacity"].toInt());
			globalDialog->setPredatorReproductionRadius(settings["predatorReproductionRadius"].toInt());
			globalDialog->setInitialInertialWeight(settings["initialInertiaWeight"].toFloat());
			globalDialog->setFinalInertiaWeight(settings["finalInertiaWeight"].toFloat());
		}
		break;

	case LOCAL:
		{
			auto local = dynamic_cast<LocalCaPso*>(mCellularAutomaton);

			local->setInitialAlivePreys(settings["initialNumberOfPreys"].toFloat());
			local->setCompetenceFactor(settings["competenceFactor"].toFloat());
			local->setPreyReproductionRadius(settings["preyReproductionRadius"].toInt());
			local->setPreyMeanOffspring(settings["preyReproductiveCapacity"].toInt());

			local->setInitialSwarmSize(settings["initialNumberOfPredators"].toInt());
			local->setCognitiveFactor(settings["predatorCognitiveFactor"].toFloat());
			local->setSocialFactor(settings["predatorSocialFactor"].toFloat());
			local->setMaximumSpeed(settings["predatorMaximumSpeed"].toInt());
			local->setPredatorMeanOffspring(settings["predatorReproductiveCapacity"].toInt());
			local->setPredatorReproductionRadius(settings["predatorReproductionRadius"].toInt());
			local->setSocialRadius(settings["predatorSocialRadius"].toInt());
			local->setFitnessRadius(settings["fitnessRadius"].toInt());
			local->setInitialInertialWeight(settings["initialInertiaWeight"].toFloat());
			local->setFinalInertiaWeight(settings["finalInertiaWeight"].toFloat());
		}
		break;

	case MOVEMENT:
		break;
	}

	mCurrFileName = settings["resultsFilePath"].toString();
}

void Controller::makeConnections()
{
	connect(actionPlay, SIGNAL(triggered()), this, SLOT(play()));
	connect(actionPause, SIGNAL(triggered()), this, SLOT(pause()));
	connect(actionStep, SIGNAL(triggered()), this, SLOT(step()));
	connect(actionClear, SIGNAL(triggered()), this, SLOT(clear()));
	connect(actionInitialize, SIGNAL(triggered()), this, SLOT(initialize()));
	connect(actionSettings, SIGNAL(triggered()), this, SLOT(showSettings()));
	connect(actionExportBitmap, SIGNAL(triggered()), this, SLOT(exportBitmap()));
}

void Controller::createCa()
{
	switch(mCurrentType)
	{
	case GLOBAL:
		mCellularAutomaton = new GlobalCaPso(512, 512);
		mSeasonLength = 10;
		break;

	case LOCAL:
		mCellularAutomaton = new LocalCaPso(512, 512);
		mSeasonLength = 10;
		break;

	case MOVEMENT:
		break;
	}
}

void Controller::createView()
{
	mView = new CaView(this, mCellularAutomaton);
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
			connect(p, SIGNAL(settingsChanged(QMap<QString, QVariant>)),
				this, SLOT(updateSettings(QMap<QString, QVariant>)));
		}
		break;

	case MOVEMENT:
		break;
	}
}

void Controller::initializeResultsFile()
{
	mResultsFile->close();
	mResultsFile->setFileName(mCurrFileName);
	mResultsFile->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);
	mResultsStream.seek(0);
}

void Controller::writeResults()
{
	switch(mCurrentType)
	{
	case LOCAL:
		{
			auto local = dynamic_cast<LocalCaPso*>(mCellularAutomaton);
			mResultsStream << mTimerCount / mSeasonLength << " " << local->numberOfPreys() <<
				" " << local->numberOfPredators() << "\n";
		}
		break;
	}
}