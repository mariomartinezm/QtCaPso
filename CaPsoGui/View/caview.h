#ifndef CAVIEW_H
#define CAVIEW_H

#include <QWidget>
#include <QImage>

class CaView : public QWidget
{
	Q_OBJECT

public:
    explicit CaView(unsigned char* latticeData, const int& width,
                    const int& height, QWidget* parent = 0);

	~CaView();

	QSize sizeHint() const;

	const QImage& latticeImage() const;

protected:
	void paintEvent(QPaintEvent*);

private:
	QImage* mLatticeImage;
    const int mWidth, mHeight;
};

#endif // CAVIEW_H
