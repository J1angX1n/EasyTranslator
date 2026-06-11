#include "stdafx.h"
#include "AppSettings.h"
#include <QSettings>

std::unique_ptr<QSettings> AppSettings::m_Settings = nullptr;

AppSettings::AppSettings()
{
}

AppSettings::~AppSettings()
{
}

QSettings* AppSettings::GetSettings()
{
	if (m_Settings)
	{
		return m_Settings.get();
	}

	m_Settings = std::unique_ptr<QSettings>(new QSettings("JXAPP", "EasyTranslator"));
	return m_Settings.get();
}

void AppSettings::SetModel(AIModel::Type model)
{
	GetSettings()->setValue(SettingsKeys::AIModel, AIModel::GetNameByModel(model));
}

AIModel::Type AppSettings::GetModel()
{
	return AIModel::GetModelByName(GetSettings()->value(SettingsKeys::AIModel).toString());
}

void AppSettings::SetAPIKey(QString key)
{
	GetSettings()->setValue(SettingsKeys::APIKey, key);
}

QString AppSettings::GetAPIKey()
{
	return GetSettings()->value(SettingsKeys::APIKey).toString();
}
