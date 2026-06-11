#pragma once
#include <QWidget>
#include "AIModel.h"
#include <QThread>

class QTextEdit;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class ScreenReader;
class QTimer;
class NetworkManager;
class QNetworkReply;

class ScreenTranslateWidget : public QWidget
{
	Q_OBJECT

public:
	ScreenTranslateWidget(QWidget* parent = nullptr);
	~ScreenTranslateWidget();

public:
    void SetEnabled(bool enabled) { b_Enabled = enabled; }
    bool GetEnabled() { return b_Enabled; }

private:
    void InitWidget();
    void InitThread();
    void TimedReadScreen();

private slots:
    void SendTranslateRequest(QString text);
    void OnTranslateFinished(QNetworkReply* reply);

signals:
    // void OnPauseButtonClicked();
    void OnCancelButtonClicked();

private:
    QTextEdit* m_TextOutput;
    QPushButton* m_PauseButton;
    QPushButton* m_CancelButton;
    QVBoxLayout* m_VerticalLayout;
    QHBoxLayout* m_HorizontalLayout;

    // ScreenReader
    ScreenReader* m_ScreenReader;
    QString m_LastContent;

    // Thread
    QThread m_ScreenReaderThread;

    // Timer
    float f_Interval = 10.0; //second
    QTimer* m_Timer;

    NetworkManager* m_NetworkManager;

    // Control
    bool b_Enabled = true;
};

