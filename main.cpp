#include <QApplication>
#include <QFile>
#include <iostream>

#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>

#include "gui/prod/MainWindow.h"
#include "gui/prod/EditorWindow.h"
#include "configManager.h"


using namespace std;


int main(int argc, char *argv[]) {
    loadConfig();

    //no use QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
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

    //MainWindow window;
    //window.setWindowIcon(QIcon("i.ico"));
    //window.show();
    // todo delete from here
    string file = "landscape.qrland";
    auto *x = new EditorWindow(ModelInitData(), false);
    x->presenter->facade->load(file);
    x->show();

    return app.exec();
}
