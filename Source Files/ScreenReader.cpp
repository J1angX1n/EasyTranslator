#include "stdafx.h"
#include "ScreenReader.h"
#include <QDebug>
#include <QScreen>
#include <QAtomicInt>

ScreenReader::ScreenReader(QObject* parent): QObject(parent)
{
	m_Screen = QGuiApplication::primaryScreen();
	m_CurrentScreenshot = QImage()/*m_Screen->grabWindow(0)*/;
	m_Tesseract = new tesseract::TessBaseAPI();
	m_Processing = 0;

	QString path = QDir::toNativeSeparators(
		QCoreApplication::applicationDirPath() + "/../../Resources/tessdata"
	);

	if (int ret = m_Tesseract->Init(path.toUtf8().constData(), "eng")) 
	{
		b_Available = false;
	}

	m_Tesseract->SetVariable("tessedit_char_whitelist",
		"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 .,!?@-\n");
}

ScreenReader::~ScreenReader()
{
	m_Tesseract->End();
	delete m_Tesseract;
}

void ScreenReader::GetScreenshot()
{
	if (!b_Available)
	{
		return;
	}

	m_CurrentScreenshot = m_Screen->grabWindow(0).toImage();
	if (m_CurrentScreenshot.isNull())
	{
		qWarning() << "Screenshot is null";
	}

	return;
}

void ScreenReader::GetCurrentEngContent()
{
	if (!m_Processing.testAndSetOrdered(0, 1))
	{
		return;
	}

	if (!b_Available)
	{
		emit OCRFinished("");
		m_Processing.storeRelease(0);
		return;
	}

	QImage Image = m_CurrentScreenshot;
	if (Image.isNull())
	{
		qWarning() << "Image is null";
		emit OCRFinished("");
		m_Processing.storeRelease(0);
		return;
	}

	Image = Image.convertToFormat(QImage::Format_RGB888);
	m_Tesseract->SetImage(
		Image.bits(),
		Image.width(),
		Image.height(),
		3,
		Image.bytesPerLine()
	);

	char* outText = m_Tesseract->GetUTF8Text();
	QString ret = QString::fromUtf8(outText);
	delete[] outText;
	m_Tesseract->Clear();

	emit OCRFinished(ret);
	m_Processing.storeRelease(0);

	return;
}
