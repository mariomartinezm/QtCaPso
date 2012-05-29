#include <QPainter>
#include <QColor>
#include "caview.h"

CaView::CaView(QWidget* parent, CellularAutomaton* ca) :
QWidget(parent), mDesiredWidth(ca->width() * 2), mDesiredHeight(ca->height() * 2)
{
	mCellularAutomaton = ca;
	mLatticeImage = new QImage(mCellularAutomaton->latticeData(), mCellularAutomaton->width(),
		mCellularAutomaton->height(),  QImage::Format_Indexed8);

	// Initialize the state table
	mLatticeImage->setColor(0, qRgb(0, 0, 0));			// Empty
	mLatticeImage->setColor(1, qRgb(0, 128, 0));		// Prey
	mLatticeImage->setColor(2, qRgb(0, 0, 255));		// Predator
	mLatticeImage->setColor(3, qRgb(255, 0, 0));		// Prey and predator

	// Do not allow this widget to resize
	setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
}

CaView::~CaView()
{
	delete mLatticeImage;
}

QSize CaView::sizeHint() const
{
	return QSize(mDesiredWidth, mDesiredHeight);
}

void CaView::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	painter.setViewport(0, 0, mDesiredWidth, mDesiredHeight);
	painter.drawImage(QPoint(0, 0), mLatticeImage->scaledToWidth(mDesiredWidth));
}