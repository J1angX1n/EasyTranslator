#pragma once
#include "AIModel.h"
#include <QString>
#include <memory>

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
	static void SetModel(AIModel::Type model);
	static AIModel::Type GetModel();

	static void SetAPIKey(QString key);
	static QString GetAPIKey();

private:
	static std::unique_ptr<QSettings> m_Settings;
};

