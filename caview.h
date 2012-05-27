#ifndef CAVIEW_H
#define CAVIEW_H

#include <QWidget>
#include <QImage>
#include "cellularautomaton.h"

class CaView : public QWidget
{
	Q_OBJECT

public:
	explicit CaView(QWidget* parent = 0, CellularAutomaton* ca = 0);
	~CaView();

	QSize sizeHint() const;

protected:
	void paintEvent(QPaintEvent*);

private:
	CellularAutomaton* mCellularAutomaton;
	QImage* mLatticeImage;
	int mDesiredWidth, mDesiredHeight;
	int mTimerId, mGenerationCount;
};

#endif // CAVIEW_H