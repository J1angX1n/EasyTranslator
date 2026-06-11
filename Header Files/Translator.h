#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Translator.h"
#include <QNetworkReply>
#include "AIModel.h"
#include "TranslateMode.h"

class QTextEdit;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class QNetworkAccessManager;
class QTimer;
class ScreenReader;
class QStackedWidget;
class ScreenTranslateWidget;
class CommonTranslateWidget;

class Translator : public QMainWindow
{
    Q_OBJECT
public:
    Translator(QWidget *parent = nullptr);
    ~Translator();

private:
    void SendRequest(QString content, AIModel::Type model = AIModel::Type::DEEPSEEK_CHAT);
    void InitScreenModeUI();
    void InitCommonModeUI();
    void SetScreenMode();
    void SetCommonMode();
    void InitMenuBar();

public:
    void SetTranslateMode(TranslateMode::Type mode);

private slots:
    // MenuBar
    void OnAPIKeySettingsClicked();
    void OnTranslateSettingsClicked();

private:
    static constexpr int COMMON_INDEX = 0;
    static constexpr int SCREEN_INDEX = 1;

private:
    Ui::TranslatorClass ui;

    // UI
    QStackedWidget* m_StackedWidget;
    QVBoxLayout* m_CentralLayout;
    CommonTranslateWidget* m_CommonPage;
    ScreenTranslateWidget* m_ScreenPage;

    // Mode
    TranslateMode::Type e_TranslateMode;
};

