#pragma once

#include <QSqlDatabase>


class TranslationDB
{
	TranslationDB();
	~TranslationDB();



private:
	QSqlDatabase m_DataBase;
};

