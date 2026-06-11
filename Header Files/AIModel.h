#pragma once
#include <QString>
#include <QVector>

namespace AIModel
{

#define AI_MODEL_LIST(X)\
	X(DEEPSEEK_CHAT, "deepseek-chat")\
	X(CHATGPT, "chat-gpt")\
    X(NONE, "None")

#define TRANSLATE_MODE_ENUM_DEFINE(ENUM, NAME) ENUM,
#define CASE_RETURN_AI_MODEL_NAME(ENUM, NAME) case Type::ENUM: return NAME;
#define IF_RETURN_AI_MODEL(ENUM, NAME) if(NAME == name) return Type::ENUM;
#define AI_MODEL_NAME_LIST(ENUM, NAME) NAME,

    enum class Type
    {
        AI_MODEL_LIST(TRANSLATE_MODE_ENUM_DEFINE)
    };

    inline QString GetNameByModel(Type model)
    {
        switch (model)
        {
            AI_MODEL_LIST(CASE_RETURN_AI_MODEL_NAME)
        }

        return "None";
    }

    inline Type GetModelByName(QString name)
    {
        AI_MODEL_LIST(IF_RETURN_AI_MODEL);

        return Type::NONE;
    }

    inline QVector<QString> GetModelNameList()
    {
        return QVector<QString>{AI_MODEL_LIST(AI_MODEL_NAME_LIST)};
    }

#undef AI_MODEL_LIST
#undef AI_MODEL_ENUM_DEFINE
#undef CASE_RETURN_AI_MODEL_NAME
#undef IF_RETURN_AI_MODEL
#undef AI_MODEL_NAME_LIST
}

