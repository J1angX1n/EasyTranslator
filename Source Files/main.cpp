#include "stdafx.h"
#include "Translator.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Translator window;
    window.show();
    return app.exec();
}
