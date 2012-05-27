#include "controller.h"

Controller::Controller(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	this->setupUi(this);

	makeConnections();
	createCa(CaType::LOCAL);
}

Controller::~Controller()
{

}

void Controller::play()
{

}

void Controller::pause()
{

}

void Controller::step()
{

}

void Controller::clear()
{

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