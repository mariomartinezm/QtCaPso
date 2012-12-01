#ifndef CAVIEW_H
#define CAVIEW_H

#pragma warning(push, 3)
#pragma warning(disable:4800)
#include <QWidget>
#include <QImage>
#pragma warning(pop)
#include "cellularautomaton.h"

class CaView : public QWidget
{
	Q_OBJECT

public:
	explicit CaView(QWidget* parent = 0, CellularAutomaton* ca = 0);

	~CaView();

	QSize sizeHint() const;

	const QImage& latticeImage() const;

protected:
	void paintEvent(QPaintEvent*);

private:
	CellularAutomaton* mCellularAutomaton;
	QImage* mLatticeImage;
	int mDesiredWidth, mDesiredHeight;
	int mTimerId, mGenerationCount;
};

#endif // CAVIEW_H