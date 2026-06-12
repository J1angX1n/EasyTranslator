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
            {"content", "You are an English-to-Chinese dictionary specializing in single-word lookup. For each input English word, produce a thorough, well-organized Chinese translation entry. \
            Rules:\
            1. List most common Chinese meanings of the word, grouped by part of speech(n. / v. / adj. / adv. / prep. / conj. / etc.).\
            2. Within each POS group, separate distinct senses with semicolons(；).If a meaning is domain - specific(computing, legal, medical, etc.), append the domain label in parentheses.\
            3. Include phrasal verbs and common compounds that start with this word(e.g., for \"take\": take off, take over, take up), each with their Chinese translations.\
            4. After all translations, leave one blank line, then output \"例句:\" followed by ONE English example sentence that demonstrates the most common usage, then the Chinese translation of that sentence on the next line.\
            Output format :\
            word: n.释义1；释义2（领域）\
            v.释义1；释义2\
            例句 : [English sentence]\
            [中文翻译]\
            Be concise but exhaustive.When in doubt, include the meaning rather than omit it."}
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