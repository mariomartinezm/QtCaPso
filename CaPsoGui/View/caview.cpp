#include <QPainter>
#include <QColor>
#include <QWidget>
#include "caview.h"

CaView::CaView(unsigned char* latticeData, const int& width,
               const int& height, QWidget* parent)
    : QWidget(parent),
      mWidth(width),
      mHeight(height)

{
    mLatticeImage = new QImage(latticeData, width, height, QImage::Format_Indexed8);

    // Initialize the state table
    mLatticeImage->setColor(0, qRgb(0, 0, 0));		// Empty
    mLatticeImage->setColor(1, qRgb(38, 127, 0));	// Prey
    mLatticeImage->setColor(2, qRgb(255, 0, 0));	// Predator
    mLatticeImage->setColor(3, qRgb(255, 216, 0));	// Prey and predator
    mLatticeImage->setColor(4, qRgb(255, 255, 255));	// Global Best
    mLatticeImage->setColor(5, qRgb(255, 255, 255));	// Global Best
    mLatticeImage->setColor(6, qRgb(255, 255, 255));	// Global Best
    mLatticeImage->setColor(7, qRgb(255, 255, 255));	// Global Best

    // Do not allow this widget to resize
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
}

CaView::~CaView()
{
	delete mLatticeImage;
}

QSize CaView::sizeHint() const
{
    return QSize(mWidth, mHeight);
}

const QImage& CaView::latticeImage() const
{
	return *mLatticeImage;
}

void CaView::paintEvent(QPaintEvent*)
{
	QPainter painter(this);

	QWidget* parentWidget = dynamic_cast<QWidget*>(this->parent());
    painter.drawImage(QPoint(0, 0), mLatticeImage->scaled(parentWidget->width(), parentWidget->height()));
}
