#include "stdafx.h"
#include "ScreenTranslateWidget.h"
#include "ScreenReader.h"
#include "NetworkManager.h"
#include <QTextEdit>
#include <QPushButton>
#include <QBoxLayout>

ScreenTranslateWidget::ScreenTranslateWidget(QWidget* parent): QWidget(parent)
{
    InitWidget();
    InitThread();
}

ScreenTranslateWidget::~ScreenTranslateWidget()
{
    m_ScreenReaderThread.quit();
    m_ScreenReaderThread.wait();
    delete m_ScreenReader;
}

void ScreenTranslateWidget::InitWidget()
{
    m_TextOutput = new QTextEdit();
    m_PauseButton = new QPushButton();
    m_CancelButton = new QPushButton();
    m_VerticalLayout = new QVBoxLayout();
    m_HorizontalLayout = new QHBoxLayout();

    m_TextOutput->setReadOnly(true);
    m_PauseButton->setText("Pause");
    m_CancelButton->setText("Cancel");
    m_VerticalLayout->addWidget(m_TextOutput);

    m_VerticalLayout->addLayout(m_HorizontalLayout);
    m_HorizontalLayout->addWidget(m_PauseButton);
    m_HorizontalLayout->addWidget(m_CancelButton);

    setLayout(m_VerticalLayout);

    connect(m_CancelButton, &QPushButton::clicked, this, &ScreenTranslateWidget::OnCancelButtonClicked);
}

void ScreenTranslateWidget::InitThread()
{
    m_NetworkAccessManager = new NetworkManager(this);
    m_Timer = new QTimer(this);
    m_ScreenReader = new ScreenReader();
    m_LastContent = "";

    if (b_Enable)
    {
        m_ScreenReader->moveToThread(&m_ScreenReaderThread);
        connect(m_ScreenReader, &ScreenReader::OCRFinished, this, &ScreenTranslateWidget::SendTranslateRequest);
        m_ScreenReaderThread.start();
        
        TimedReadScreen();
        connect(m_Timer, &QTimer::timeout, this, &ScreenTranslateWidget::TimedReadScreen);
        m_Timer->start(f_Interval * 1000);
    }

    connect(m_NetworkAccessManager, &NetworkManager::OnReceivedReply, this, &ScreenTranslateWidget::OnTranslateFinished);
}

void ScreenTranslateWidget::OnTranslateFinished(QNetworkReply* reply)
{
    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    QJsonArray choices = (doc.object()["choices"]).toArray();

    QJsonObject message = ((choices[0].toObject())["message"]).toObject();
    QString content = message["content"].toString();

    m_TextOutput->setText(content);
}

void ScreenTranslateWidget::SendTranslateRequest(QString text)
{
    if (m_LastContent != text)
    {
        m_LastContent = text;
        m_NetworkAccessManager->SendRequest(text);
    }
}

void ScreenTranslateWidget::TimedReadScreen()
{
    m_ScreenReader->GetScreenshot();
    QMetaObject::invokeMethod(m_ScreenReader, "GetCurrentEngContent", Qt::QueuedConnection);
}