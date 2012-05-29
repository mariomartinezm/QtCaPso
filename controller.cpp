#include "controller.h"
#include "localcapso.h"

Controller::Controller(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags),
	mCurrentType(LOCAL),
	mSettingsDialog(new LocalSettingsDialog()),
	mTimerId(-1),
	mTimerCount(0)
{
	this->setupUi(this);

	makeConnections();
	createCa();

	mView = new CaView(this, mCellularAutomaton);
	setCentralWidget(mView);
}

Controller::~Controller()
{
	delete mCellularAutomaton;
	delete mView;
}

void Controller::timerEvent(QTimerEvent* e)
{
	mCellularAutomaton->nextGen();

	mTimerCount++;

	/*if(!(mTimerCount % 10))
	{
		writeResults();
	}*/

	//mStatusBarString.setNum(mTimerCount);

	//mStatusBar->showMessage(mStatusBarString);

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

	if(!(mTimerCount % 5))
	{
		//writeResults();
	}

	update();
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

	// Reset the contents of the results file
	/*mResultsFile->close();
	mResultsFile->setFileName(mCurrentFileName);
	mResultsFile->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);
	mResultsStream.seek(0)*/

	mView->update();
}

void Controller::showSettings()
{
	mSettingsDialog->exec();
}

void Controller::updateSettings(QMap<QString, QVariant> settings)
{
	switch(mCurrentType)
	{
	case GLOBAL:
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
			local->setMaximumSpeed(settings["predatorMaximumSpeed"].toFloat());
			local->setPredatorMeanOffspring(settings["predatorReproductiveCapacity"].toInt());
			local->setPredatorReproductionRadius(settings["predatorReproductionRadius"].toInt());
			local->setSearchRadius(settings["predatorSocialRadius"].toInt());
			local->setInitialInertialWeight(settings["initialInertiaWeight"].toFloat());
			local->setFinalInertiaWeight(settings["finalInertiaWeight"].toFloat());
		}
		break;

	case MOVEMENT:
		break;
	}
}

void Controller::makeConnections()
{
	connect(actionPlay, SIGNAL(triggered()), this, SLOT(play()));
	connect(actionPause, SIGNAL(triggered()), this, SLOT(pause()));
	connect(actionStep, SIGNAL(triggered()), this, SLOT(step()));
	connect(actionClear, SIGNAL(triggered()), this, SLOT(clear()));
	connect(actionInitialize, SIGNAL(triggered()), this, SLOT(initialize()));
	connect(actionSettings, SIGNAL(triggered()), this, SLOT(showSettings()));

	connect(mSettingsDialog, SIGNAL(settingsChanged(QMap<QString, QVariant>)),
		this, SLOT(updateSettings(QMap<QString, QVariant>)));
}

void Controller::createCa()
{
	switch(mCurrentType)
	{
	case GLOBAL:
		break;

	case LOCAL:
		mCellularAutomaton = new LocalCaPso(512, 256);
		break;

	case MOVEMENT:
		break;
	}
}