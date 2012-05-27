#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QtGui/QMainWindow>
#include "ui_controller.h"
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

private:
	void makeConnections();
	void createCa(CaType);

	CellularAutomaton* mCellularAutomaton;
	CaView* mView;
	int mTimerId;
	int mTimerCount;
};

#endif // CONTROLLER_H
