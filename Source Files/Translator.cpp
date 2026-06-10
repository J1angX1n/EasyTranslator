#include "stdafx.h"
#include "Translator.h"
#include <QTextEdit>
#include <QPushButton>
#include <QBoxLayout>
#include <QNetworkAccessManager>
#include <QTimer>
#include "ScreenReader.h"
#include <QThread>
#include "APIKeySettingsDialog.h"
#include "AppSettings.h"

Translator::Translator(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    m_NetworkAccessManager = new QNetworkAccessManager(this);
    m_Timer = new QTimer(this);
    m_ScreenReader = new ScreenReader();
    m_LastContent = "";

    InitUI();
    InitMenuBar();

    m_ScreenReader->moveToThread(&m_ScreenReaderThread);
    connect(m_ScreenReader, &ScreenReader::OCRFinished, this, &Translator::Translate);
    m_ScreenReaderThread.start();

    TimedTranslate();
}

Translator::~Translator()
{
    m_ScreenReaderThread.quit();
    m_ScreenReaderThread.wait();
    delete m_ScreenReader;
}

void Translator::OnTranslateFinished(QNetworkReply* reply)
{
    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    QJsonArray choices = (doc.object()["choices"]).toArray();

    QJsonObject message = ((choices[0].toObject())["message"]).toObject();
    QString content = message["content"].toString();

    m_TextOutput->setText(content);
}

void Translator::Translate(QString text)
{
    if (m_LastContent != text)
    {
        m_LastContent = text;
        SendRequest(text);
    }
}

void Translator::SendRequest(QString Content, AIModel Model)
{
    QString selectedModel = GetNameByModel(Model);
    QString APIKey = AppSettings::GetAPIKey();
    if (APIKey.isEmpty())
    {
        m_TextOutput->setText("请先在 Settings > API Key Settings 中设置 API Key");
        return;
    }

    QJsonObject SysInfo{
        {"role", "system"},
        {"content", "You are an English-to-Chinese dictionary. From the input text, extract all \
        recognizable English words and common phrases (e.g. \"top up\", \"give up\", \"credit card\"). \
        Skip common function words (a, the, is, in, of, etc.) when they appear alone. Translate each entry to \
        Chinese. Output one entry per line, sorted alphabetically. Format: apple: 苹果, top up: 充值. Only \
        translations, no explanations."}
    };

    QJsonObject UserInfo{
        {"role", "user"},
        {"content", Content},
    };

    QJsonArray Message{ SysInfo, UserInfo };

    QJsonObject RequestedJson{
        {"model", selectedModel},
        {"messages", Message},
        {"temperature", 0},
    };

    m_Json = RequestedJson;
    m_NetRequest.setUrl(QUrl("https://api.deepseek.com/v1/chat/completions"));
    m_NetRequest.setRawHeader("Content-Type", "application/json");
    m_NetRequest.setRawHeader("Authorization", "Bearer " + APIKey.toUtf8());

    m_NetworkAccessManager->post(m_NetRequest, QJsonDocument(m_Json).toJson());
}

void Translator::InitUI()
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
    ui.centralWidget->setLayout(m_VerticalLayout);

    connect(m_CancelButton, &QPushButton::clicked, this, &QMainWindow::close);
    connect(m_NetworkAccessManager, &QNetworkAccessManager::finished, this, &Translator::OnTranslateFinished);
}

void Translator::InitMenuBar()
{
    QMenu* file = menuBar()->addMenu("File");
    QMenu* settings = menuBar()->addMenu("Settings");

    QAction* exit = file->addAction("exit");
    connect(exit, &QAction::triggered, this, &QMainWindow::close);

    QAction* apiKey = settings->addAction("API Key Settings");
    connect(apiKey, &QAction::triggered, this, &Translator::OnAPIKeySettingsClicked);
}

void Translator::TimedTranslate()
{
    connect(m_Timer, &QTimer::timeout, this, &Translator::ScreenReadTranslate);
    m_Timer->start(f_Interval * 1000);
}

void Translator::ScreenReadTranslate()
{
    m_ScreenReader->GetScreenshot();
    QMetaObject::invokeMethod(m_ScreenReader, "GetCurrentEngContent", Qt::QueuedConnection);
}

void Translator::OnAPIKeySettingsClicked()
{
    APIKeySettingsDialog* dialog = new APIKeySettingsDialog(this);
    dialog->show();
}