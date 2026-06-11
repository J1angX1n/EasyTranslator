#pragma once
#include <QWidget>

class QTextEdit;
class QVBoxLayout;
class QTimer;
class NetworkManager;
class QNetworkReply;

class CommonTranslateWidget : public QWidget
{
	Q_OBJECT

public:
	CommonTranslateWidget(QWidget* parent = nullptr);
	~CommonTranslateWidget();

	void InitWidget();

private slots:
	void SendTranslateRequest();
	void OnTranslateFinished(QNetworkReply* reply);
	
private:
	QTextEdit* m_Input;
	QTextEdit* m_Output;
	QVBoxLayout* m_VerticalLayout;
	
	QTimer* m_Debounce;
	float f_DebounceTime = 1.5;

	NetworkManager* m_NetworkManager;
};

