#pragma once
#include "AIModel.h"
#include <QString>

class QSettings;

namespace SettingsKeys
{
	constexpr auto AIModel = "AIModel";
	constexpr auto APIKey = "APIKey";
}

class AppSettings
{
public:
	AppSettings();
	~AppSettings();
	static QSettings* GetSettings();

public:
	static void SetModel(AIModel model);
	static AIModel GetModel();

	static void SetAPIKey(QString key);
	static QString GetAPIKey();

private:
	static QSettings* m_Settings;
};

