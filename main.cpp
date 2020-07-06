#include <QApplication>
#include <QFile>
#include <iostream>

#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>

#include "gui/wip1/MainWindow.h"

using namespace std;


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QFile styleF;
    styleF.setFileName("gui/style.css");
    styleF.open(QFile::ReadOnly);
    QString qssStr = styleF.readAll();
    app.setStyleSheet(qssStr);

    QString translatorFileName = QLatin1String("qt_");
    translatorFileName += QLocale::system().name();
    QTranslator *translator = new QTranslator(&app);
    if (translator->load(translatorFileName, QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        app.installTranslator(translator);

    MainWindow window;
    window.setWindowIcon(QIcon("i.ico"));
    window.show();

    return app.exec();
}
