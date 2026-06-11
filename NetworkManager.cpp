#include "stdafx.h"
#include "NetworkManager.h"
#include <QNetworkAccessManager>
#include <QTimer>
#include <QThread>
#include "AppSettings.h"

NetworkManager::NetworkManager(QObject* parent): QObject(parent)
{
    m_NetworkAccessManager = new QNetworkAccessManager(this);
    connect(m_NetworkAccessManager, &QNetworkAccessManager::finished, this, &NetworkManager::OnReceivedReply);
}

NetworkManager::~NetworkManager()
{
}

bool NetworkManager::SendRequest(QString content, AIModel::Type model)
{
    QString selectedModel = AIModel::GetNameByModel(model);
    QString APIKey = AppSettings::GetAPIKey();
    if (APIKey.isEmpty())
    {
        //m_TextOutput->setText("请先在 Settings > API Key Settings 中设置 API Key");
        return false;
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
        {"content", content},
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

    return true;
}