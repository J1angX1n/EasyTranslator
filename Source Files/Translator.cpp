#include "stdafx.h"
#include "Translator.h"
#include <qtextedit.h>
#include <qpushbutton.h>
#include <qboxlayout.h>
#include <qnetworkaccessmanager.h>

QString DS_APIKey = "sk-d98de50e686a4b909e3e376916d858e2";

Translator::Translator(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    m_NetworkAccessManager = new QNetworkAccessManager(this);

    InitUI();
    InitJson("less is more \n i love you");
}

Translator::~Translator()
{}

void Translator::OnTranslateFinished(QNetworkReply* reply)
{
    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    QJsonArray choices = (doc.object()["choices"]).toArray();

    QJsonObject message = ((choices[0].toObject())["message"]).toObject();
    QString content = message["content"].toString();

    m_TextOutput->setText(content);

}

void Translator::InitJson(QString Content, AIModel Model)
{
    QString SelectedModel = "deepseek-chat";
    switch (Model)
    {
    case AIModel::DEEPSEEK_CHAT:
        SelectedModel = "deepseek-chat";
        break;
    default:
        break;
    }

    QJsonObject SysInfo{
        {"role", "system"},
        {"content", "You are a translator. Translate user input to Chinese.\
              For each input, output in this format:\
              原文：{original text}\
              译文：{translated text}\
              If there are multiple inputs, separate each block with a blank line.\
              Output only the formatted text, no explanations."}
    };

    QJsonObject UserInfo{
        {"role", "user"},
        {"content", Content},
    };

    QJsonArray Message{ SysInfo, UserInfo };

    QJsonObject RequestedJson{
        {"model", SelectedModel},
        {"messages", Message},
        {"temperature", 0.3},
    };

    m_Json = RequestedJson;
    m_NetRequest.setUrl(QUrl("https://api.deepseek.com/v1/chat/completions"));
    m_NetRequest.setRawHeader("Content-Type", "application/json");
    m_NetRequest.setRawHeader("Authorization", "Bearer " + DS_APIKey.toUtf8());

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
