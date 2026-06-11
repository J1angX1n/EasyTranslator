#pragma once
#include <QThread>
#include <QNetworkReply>
#include "AIModel.h"

class NetworkManager : public QObject
{
    Q_OBJECT

public:
    NetworkManager(QObject* parent = nullptr);
    ~NetworkManager();

    bool SendRequest(QString content, AIModel::Type model = AIModel::Type::DEEPSEEK_CHAT);

signals:
    void OnReceivedReply(QNetworkReply* reply);

private:

    // Network
    QNetworkAccessManager* m_NetworkAccessManager;
    QNetworkRequest m_NetRequest;
    QJsonObject m_Json;
};

