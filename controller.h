#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QtGui/QMainWindow>
#include "ui_controller.h"

class Controller : public QMainWindow
{
	Q_OBJECT

public:
	Controller(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Controller();

private:
	Ui::ControllerClass ui;
};

#endif // CONTROLLER_H
