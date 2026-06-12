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

bool NetworkManager::SendRequest(QString content, TranslateMode::Type mode, AIModel::Type model)
{
    QString selectedModel = AIModel::GetNameByModel(model);
    QString APIKey = AppSettings::GetAPIKey();
    if (APIKey.isEmpty())
    {
        //m_TextOutput->setText("请先在 Settings > API Key Settings 中设置 API Key");
        return false;
    }

    QJsonObject SysInfo;

    switch (mode)
    {
    case TranslateMode::Type::COMMON:
        SysInfo = QJsonObject{
            {"role", "system"},
            {"content", 
                "You are an English-to-Chinese translator. Translate the input text into accurate, natural Chinese. Adapt your output to the type of input: \
                - Single word : Provide a thorough dictionary entry.List most common Chinese meanings grouped by part of speech(n. / v. / adj. / adv. / etc.).\
                Include domain - specific senses with domain labels in parentheses.Include related phrasal verbs and compounds.After all meanings, leave a blank line, output \"例句:\" followed by one English example sentence and its Chinese translation.\
                - Phrase or idiom : Give the most accurate Chinese equivalent(s).If helpful, append a brief example.\
                - Sentence or paragraph : Translate into fluent, natural Chinese.Preserve tone and style.No dictionary formatting needed.\
                Output only the translation result.Do not add prefacing remarks like \"Here is the translation:\" or \"Translation:\". Use plain text only. Do not use Markdown formatting (no **bold**, no *italic*, no `code`, no lists with - or *)."}
        };
        break;
    case TranslateMode::Type::SCREEN:
        SysInfo = QJsonObject{
            {"role", "system"},
            {"content", "You are an English-to-Chinese dictionary. From the input text, extract all \
            recognizable English words and common phrases (e.g. \"top up\", \"give up\", \"credit card\"). \
            Skip common function words (a, the, is, in, of, etc.) when they appear alone. Translate each entry to \
            Chinese. Output one entry per line, sorted alphabetically. Format: apple: 苹果, top up: 充值. Only \
            translations, no explanations."}
        };
        break;
    default:
        break;
    }

    //QJsonObject SysInfo{
    //    {"role", "system"},
    //    {"content", "You are an English-to-Chinese dictionary. From the input text, extract all \
    //    recognizable English words and common phrases (e.g. \"top up\", \"give up\", \"credit card\"). \
    //    Skip common function words (a, the, is, in, of, etc.) when they appear alone. Translate each entry to \
    //    Chinese. Output one entry per line, sorted alphabetically. Format: apple: 苹果, top up: 充值. Only \
    //    translations, no explanations."}
    //};

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