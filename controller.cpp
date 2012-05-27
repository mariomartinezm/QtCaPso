#include "controller.h"

Controller::Controller(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags),
	mTimerId(-1),
	mTimerCount(0)
{
	this->setupUi(this);

	makeConnections();
	createCa(CaType::LOCAL);
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

}

void Controller::showSettings()
{

}

void Controller::makeConnections()
{
	connect(actionPlay, SIGNAL(triggered()), this, SLOT(play()));
	connect(actionPause, SIGNAL(triggered()), this, SLOT(pause()));
	connect(actionStep, SIGNAL(triggered()), this, SLOT(step()));
	connect(actionClear, SIGNAL(triggered()), this, SLOT(clear()));
	connect(actionInitialize, SIGNAL(triggered()), this, SLOT(initialize()));
}

void Controller::createCa(CaType type)
{
	switch(type)
	{
	case GLOBAL:
		break;

	case LOCAL:
		break;

	case MOVEMENT:
		break;
	}
}