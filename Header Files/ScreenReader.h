#pragma once
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <QPixmap>
#include <QString>
#include <QImage>

class QScreen;
class QAtomicInt;

class ScreenReader : public QObject
{
	Q_OBJECT

public:
	ScreenReader(QObject* parent = nullptr);
	~ScreenReader();

	void GetScreenshot();
	bool IsAvailable() { return b_Available; }

public slots:
	void GetCurrentEngContent();

signals:
	void OCRFinished(QString text);

private:
	QScreen* m_Screen;
	QImage m_CurrentScreenshot;
	tesseract::TessBaseAPI* m_Tesseract;
	bool b_Available = true;
	QAtomicInt m_Processing;
};

