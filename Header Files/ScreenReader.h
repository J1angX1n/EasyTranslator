#pragma once
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <qpixmap.h>
#include <qstring.h>
#include <qimage.h>
#include <qscreen.h>

//class QScreen;
//class TessBaseAPI;

class ScreenReader
{
public:
	ScreenReader();
	~ScreenReader();

	QString GetCurrentEngContent();

	bool IsAvailable() { return b_Available; }

private:
	QScreen* m_Screen;
	QPixmap m_CurrentScreenshot;
	float f_Interval = 3.0;
	tesseract::TessBaseAPI* m_Tesseract;
	bool b_Available = true;
};

