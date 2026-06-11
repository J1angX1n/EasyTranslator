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
class QStackedWidget;
class ScreenTranslateWidget;

class Translator : public QMainWindow
{
    Q_OBJECT
public:
    Translator(QWidget *parent = nullptr);
    ~Translator();

private:
    void SendRequest(QString content, AIModel::Type model = AIModel::Type::DEEPSEEK_CHAT);
    void InitScreenModeUI();
    //void SetScreenMode();
    void InitCommonModeUI();
    void SetCommonMode();
    void InitMenuBar();

private slots:
    // MenuBar
    void OnAPIKeySettingsClicked();
    void OnTranslateSettingsClicked();

private:
    static constexpr int SCREEN_INDEX = 1;

private:
    Ui::TranslatorClass ui;

    // UI
    QStackedWidget* m_StackedWidget;
    QVBoxLayout* m_CentralLayout;
    QWidget* m_CommonPage;
    ScreenTranslateWidget* m_ScreenPage;
};

