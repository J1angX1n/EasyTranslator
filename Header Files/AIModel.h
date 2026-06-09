#pragma once
#include <QString>
#include <QVector>

#define AI_MODEL_LIST(X)\
	X(DEEPSEEK_CHAT, "deepseek-chat")\
	X(CHATGPT, "chat-gpt")\
    X(NONE, "None")

#define ENUM_DEFINE(ENUM, NAME) ENUM,
#define CASE_RETURN_NAME(ENUM, NAME) case AIModel::ENUM: return NAME;
#define IF_RETURN_MODEL(ENUM, NAME) if(NAME == name) return AIModel::ENUM;
#define MODEL_NAME_LIST(ENUM, NAME) NAME,

enum class AIModel
{
	AI_MODEL_LIST(ENUM_DEFINE)
};

inline QString GetNameByModel(AIModel model)
{
    switch (model)
    {
        AI_MODEL_LIST(CASE_RETURN_NAME)
    }

    return "None";
}

inline AIModel GetModelByName(QString name)
{
    AI_MODEL_LIST(IF_RETURN_MODEL);

    return AIModel::NONE;
}

inline QVector<QString> GetModelNameList()
{
    return QVector<QString>{AI_MODEL_LIST(MODEL_NAME_LIST)};
}