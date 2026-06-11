#pragma once

namespace TranslateMode
{
#define TRANSLATE_MODE_LIST(X) \
	X(COMMON, "common")\
	X(SCREEN, "screen")

#define TRANSLATE_MODE_ENUM_DEFINE(ENUM, NAME) ENUM,
#define CASE_RETURN_TRANSLATE_MODE_NAME(ENUM, NAME) case Type::ENUM: return NAME;
#define IF_RETURN_TRANSLATE_MODE(ENUM, NAME) if(NAME == name) return Type::ENUM;
#define TRANSLATE_MODE_NAME_LIST(ENUM, NAME) NAME,

    enum class Type
    {
        TRANSLATE_MODE_LIST(TRANSLATE_MODE_ENUM_DEFINE)
    };

    inline QString GetNameByMode(Type mode)
    {
        switch (mode)
        {
            TRANSLATE_MODE_LIST(CASE_RETURN_TRANSLATE_MODE_NAME)
        }

        return "common";
    }

    inline Type GetModeByName(QString name)
    {
        TRANSLATE_MODE_LIST(IF_RETURN_TRANSLATE_MODE);

        return Type::COMMON;
    }

    inline QVector<QString> GetModelNameList()
    {
        return QVector<QString>{TRANSLATE_MODE_LIST(TRANSLATE_MODE_NAME_LIST)};
    }

#undef TRANSLATE_MODE_LIST
#undef AI_MODEL_ENUM_DEFINE
#undef CASE_RETURN_TRANSLATE_MODE_NAME
#undef IF_RETURN_TRANSLATE_MODE
#undef TRANSLATE_MODE_NAME_LIST
}

