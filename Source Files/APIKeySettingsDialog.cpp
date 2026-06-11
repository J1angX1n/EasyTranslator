#include "stdafx.h"
#include "APIKeySettingsDialog.h"
#include "AIModel.h"
#include "AppSettings.h"
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QVector>
#include <QBoxLayout>
#include <QDialogButtonBox>

APIKeySettingsDialog::APIKeySettingsDialog(QWidget* parent): QDialog(parent)
{
	InitUI();
	setAttribute(Qt::WA_DeleteOnClose);
}

APIKeySettingsDialog::~APIKeySettingsDialog()
{
}

void APIKeySettingsDialog::InitUI()
{
	m_DialogLayout = new QVBoxLayout();
	m_ModelLayout = new QHBoxLayout();
	m_APIKeyLayout = new QHBoxLayout();
	m_ModelComboBox = new QComboBox();
	m_APIKeyLineEdit = new QLineEdit();
	
	m_ButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok
		| QDialogButtonBox::Cancel);

	connect(m_ButtonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(m_ButtonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

	m_ModelLabel = new QLabel(this);
	m_ModelLabel->setText("Model:");
	m_ModelLabel->setMinimumWidth(80);

	m_APIKeyLabel = new QLabel(this);
	m_APIKeyLabel->setText("API Key:");
	m_APIKeyLabel->setMinimumWidth(80);

	QVector<QString> modelNameList = AIModel::GetModelNameList();
	for (QString name : modelNameList)
	{
		m_ModelComboBox->addItem(name);
	}
	m_ModelComboBox->setCurrentText(AIModel::GetNameByModel(AppSettings::GetModel()));
	m_APIKeyLineEdit->setText(AppSettings::GetAPIKey());

	m_ModelLayout->addWidget(m_ModelLabel);
	m_ModelLayout->addWidget(m_ModelComboBox);
	m_APIKeyLayout->addWidget(m_APIKeyLabel);
	m_APIKeyLayout->addWidget(m_APIKeyLineEdit);
	m_DialogLayout->addLayout(m_ModelLayout);
	m_DialogLayout->addLayout(m_APIKeyLayout);
	m_DialogLayout->addWidget(m_ButtonBox);

	setLayout(m_DialogLayout);
}

void APIKeySettingsDialog::accept()
{
	AppSettings::SetModel(AIModel::GetModelByName(m_ModelComboBox->currentText()));
	AppSettings::SetAPIKey(m_APIKeyLineEdit->text());

	QDialog::accept();
}
