#include "stdafx.h"
#include "Translator.h"
#include <QTextEdit>
#include <QPushButton>
#include <QBoxLayout>
#include <QNetworkAccessManager>
#include <QTimer>
#include "ScreenReader.h"
#include <QThread>
#include <QStackedWidget>
#include "APIKeySettingsDialog.h"
#include "AppSettings.h"
#include "TranslateSettingsDialog.h"
#include "ScreenTranslateWidget.h"

Translator::Translator(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    m_StackedWidget = new QStackedWidget();
    m_CentralLayout = new QVBoxLayout();
    m_CentralLayout->addWidget(m_StackedWidget);
    ui.centralWidget->setLayout(m_CentralLayout);

    InitMenuBar();
    InitScreenModeUI();
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
}

void Translator::SetCommonMode()
{
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