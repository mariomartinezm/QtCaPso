#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QtGui/QMainWindow>
#include "ui_controller.h"

class Controller : public QMainWindow, private Ui::ControllerClass
{
	Q_OBJECT

public:
	Controller(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Controller();

private slots:
	void play();
	void pause();
	void step();
	void clear();
	void initialize();
	void showSettings();

private:
	void makeConnections();
};

#endif // CONTROLLER_H
