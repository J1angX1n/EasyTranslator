#include "stdafx.h"
#include "ScreenReader.h"

ScreenReader::ScreenReader()
{
	m_Screen = QGuiApplication::primaryScreen();
	m_CurrentScreenshot = QPixmap()/*m_Screen->grabWindow(0)*/;
	m_Tesseract = new tesseract::TessBaseAPI();
	QString path = QCoreApplication::applicationDirPath() + "/../../Resources";

	if (m_Tesseract->Init(path.toUtf8().constData(), "eng")) {
		fprintf(stderr, "Could not initialize tesseract.\n");
		b_Available = false;
	}
}

ScreenReader::~ScreenReader()
{
	m_Tesseract->End();
	delete m_Tesseract;
}

QString ScreenReader::GetCurrentEngContent()
{
	m_CurrentScreenshot = m_Screen->grabWindow(0);
	QImage Image = m_CurrentScreenshot.toImage();
	Image = Image.convertToFormat(QImage::Format_RGB888);
	m_Tesseract->SetImage(
		Image.bits(),
		Image.width(),
		Image.height(),
		3,
		Image.bytesPerLine()
	);

	char* outText = m_Tesseract->GetUTF8Text();
	QString ret = outText;
	delete[] outText;

	return ret;
}
