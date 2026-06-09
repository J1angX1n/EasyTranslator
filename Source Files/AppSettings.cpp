#include "stdafx.h"
#include "AppSettings.h"
#include <QSettings>

QSettings* AppSettings::m_Settings = nullptr;

AppSettings::AppSettings()
{
	
}

AppSettings::~AppSettings()
{
	delete m_Settings;
}

QSettings* AppSettings::GetSettings()
{
	if (m_Settings)
	{
		return m_Settings;
	}

	m_Settings = new QSettings("JXAPP", "EasyTranslator");
	return m_Settings;
}

void AppSettings::SetModel(AIModel model)
{
	GetSettings()->setValue(SettingsKeys::AIModel, GetNameByModel(model));
}

AIModel AppSettings::GetModel()
{
	return GetModelByName(GetSettings()->value(SettingsKeys::AIModel).toString());
}

void AppSettings::SetAPIKey(QString key)
{
	GetSettings()->setValue(SettingsKeys::APIKey, key);
}

QString AppSettings::GetAPIKey()
{
	return GetSettings()->value(SettingsKeys::APIKey).toString();
}
