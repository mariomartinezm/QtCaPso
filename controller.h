#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QtGui/QMainWindow>
#include <qfile.h>
#include <qtextstream.h>
#include "ui_controller.h"
#include "catype.h"
#include "cellularautomaton.h"
#include "caview.h"
#include "localsettingsdialog.h"

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
	void updateSettings(QMap<QString, QVariant> settigns);

private:
	void makeConnections();
	void createCa();
	void createView();
	void initializeResultsFile();
	void writeResults();

	CaType mCurrentType;
	CellularAutomaton* mCellularAutomaton;
	CaView* mView;
	LocalSettingsDialog* mSettingsDialog;

	// Support for a results file
	QString mCurrFileName;
	QFile* mResultsFile;
	QTextStream mResultsStream;

	int mTimerId;
	int mTimerCount;
	int mSeasonLength;
};

#endif // CONTROLLER_H
