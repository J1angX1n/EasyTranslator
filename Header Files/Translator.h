#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Translator.h"
#include <qnetworkreply.h>

class QTextEdit;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class QNetworkAccessManager;

enum class AIModel
{
    DEEPSEEK_CHAT,
};


class Translator : public QMainWindow
{
    Q_OBJECT

public:
    Translator(QWidget *parent = nullptr);
    ~Translator();

private slots:
    void OnTranslateFinished(QNetworkReply* reply);


private:
    void InitJson(QString Content, AIModel Model = AIModel::DEEPSEEK_CHAT);
    void InitUI();

private:
    Ui::TranslatorClass ui;

    QTextEdit* m_TextOutput;
    QPushButton* m_PauseButton;
    QPushButton* m_CancelButton;
    QVBoxLayout* m_VerticalLayout;
    QHBoxLayout* m_HorizontalLayout;

    QNetworkAccessManager* m_NetworkAccessManager;
    QNetworkRequest m_NetRequest;
    QJsonObject m_Json;
};

