#include "stdafx.h"
#include "Translator.h"
#include <QTextEdit>
#include <QBoxLayout>
#include <QStackedWidget>
#include "APIKeySettingsDialog.h"
#include "AppSettings.h"
#include "TranslateSettingsDialog.h"
#include "ScreenTranslateWidget.h"
#include "CommonTranslateWidget.h"

Translator::Translator(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    m_StackedWidget = new QStackedWidget();
    m_CentralLayout = new QVBoxLayout();
    m_CentralLayout->addWidget(m_StackedWidget);
    ui.centralWidget->setLayout(m_CentralLayout);

    e_TranslateMode = TranslateMode::Type::COMMON;

    InitMenuBar();
    InitScreenModeUI();
    InitCommonModeUI();
    //SetScreenMode();
}

Translator::~Translator()
{
}

void Translator::InitScreenModeUI()
{
    m_ScreenPage = new ScreenTranslateWidget(this);
    connect(m_ScreenPage, &ScreenTranslateWidget::OnCancelButtonClicked, this, &QMainWindow::close);
    
    m_StackedWidget->insertWidget(SCREEN_INDEX, m_ScreenPage);
    m_StackedWidget->setCurrentIndex(SCREEN_INDEX);
}

void Translator::InitCommonModeUI()
{
    m_CommonPage = new CommonTranslateWidget(this);

    m_StackedWidget->insertWidget(COMMON_INDEX, m_CommonPage);
    m_StackedWidget->setCurrentIndex(COMMON_INDEX);
}

void Translator::SetScreenMode()
{
    m_StackedWidget->setCurrentIndex(SCREEN_INDEX);
    m_ScreenPage->SetEnabled(true);
}

void Translator::SetCommonMode()
{
    m_StackedWidget->setCurrentIndex(COMMON_INDEX);
    m_ScreenPage->SetEnabled(false);
}

void Translator::InitMenuBar()
{
    QMenu* file = menuBar()->addMenu("File");
    QMenu* settings = menuBar()->addMenu("Settings");

    QAction* exit = file->addAction("exit");
    connect(exit, &QAction::triggered, this, &QMainWindow::close);

    QAction* apiKey = settings->addAction("API Key Settings");
    connect(apiKey, &QAction::triggered, this, &Translator::OnAPIKeySettingsClicked);

    QAction* translateSettings = settings->addAction("Translate Settings");
    connect(translateSettings, &QAction::triggered, this, &Translator::OnTranslateSettingsClicked);
}

void Translator::SetTranslateMode(TranslateMode::Type mode)
{
    switch (mode)
    {
    case TranslateMode::Type::COMMON:
        SetCommonMode();
        break;
    case TranslateMode::Type::SCREEN:
        SetScreenMode();
        break;
    default:
        SetCommonMode();
        break;
    }
}


void Translator::OnAPIKeySettingsClicked()
{
    APIKeySettingsDialog* dialog = new APIKeySettingsDialog(this);
    dialog->show();
}

void Translator::OnTranslateSettingsClicked()
{
    TranslateSettingsDialog* dialog = new TranslateSettingsDialog(this);
    dialog->show();
}