#include "stdafx.h"
#include "CommonTranslateWidget.h"
#include <QTextEdit>
#include <QVBoxLayout>
#include <QTimer>
#include "NetworkManager.h"

CommonTranslateWidget::CommonTranslateWidget(QWidget* parent): QWidget(parent)
{
	InitWidget();
}

CommonTranslateWidget::~CommonTranslateWidget()
{
}

void CommonTranslateWidget::InitWidget()
{
	m_Input = new QTextEdit();
	m_Output = new QTextEdit();
	m_Output->setReadOnly(true);
	m_VerticalLayout = new QVBoxLayout();
	m_VerticalLayout->addWidget(m_Input);
	m_VerticalLayout->addWidget(m_Output);
	setLayout(m_VerticalLayout);

	m_Debounce = new QTimer(this);
	m_Debounce->setSingleShot(true);

	m_NetworkManager = new NetworkManager(this);

	connect(m_Input, &QTextEdit::textChanged, [this]() {
		m_Debounce->stop();
		m_Debounce->start(f_DebounceTime * 1000);
		});

	connect(m_Debounce, &QTimer::timeout, this, &CommonTranslateWidget::SendTranslateRequest);
	connect(m_NetworkManager, &NetworkManager::OnReceivedReply, this, &CommonTranslateWidget::OnTranslateFinished);
}

void CommonTranslateWidget::SendTranslateRequest()
{
	QString text = m_Input->toPlainText();
	if (!m_NetworkManager->SendRequest(text))
	{
		m_Output->setText("Error, please check settings");
	}
}

void CommonTranslateWidget::OnTranslateFinished(QNetworkReply* reply)
{
	QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
	QJsonArray choices = (doc.object()["choices"]).toArray();

	QJsonObject message = ((choices[0].toObject())["message"]).toObject();
	QString content = message["content"].toString();

	m_Output->setText(content);
}