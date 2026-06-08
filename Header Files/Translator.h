#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Translator.h"
#include <qnetworkreply.h>

class QTextEdit;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class QNetworkAccessManager;
class QTimer;
class ScreenReader;
class QThread;

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
    void Translate(QString text);

private:
    void SendRequest(QString Content, AIModel Model = AIModel::DEEPSEEK_CHAT);
    void InitUI();
    void TimedTranslate();
    void ScreenReadTranslate();

private:
    Ui::TranslatorClass ui;

    // UI
    QTextEdit* m_TextOutput;
    QPushButton* m_PauseButton;
    QPushButton* m_CancelButton;
    QVBoxLayout* m_VerticalLayout;
    QHBoxLayout* m_HorizontalLayout;

    // Network
    QNetworkAccessManager* m_NetworkAccessManager;
    QNetworkRequest m_NetRequest;
    QJsonObject m_Json;

    // Timer
    float f_Interval = 5.0; //second
    QTimer* m_Timer;

    // ScreenReader
    ScreenReader* m_ScreenReader;

    // Thread
    QThread m_ScreenReaderThread;
};

