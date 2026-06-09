#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Translator.h"
#include <QNetworkReply>
#include "AIModel.h"

class QTextEdit;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class QNetworkAccessManager;
class QTimer;
class ScreenReader;
class QThread;

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
    void InitMenuBar();

    void TimedTranslate();
    void ScreenReadTranslate();

private slots:
    // MenuBar
    void OnAPIKeySettingsClicked();


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
    float f_Interval = 10.0; //second
    QTimer* m_Timer;

    // ScreenReader
    ScreenReader* m_ScreenReader;
    QString m_LastContent;

    // Thread
    QThread m_ScreenReaderThread;
};

