#include <QGuiApplication>
#include <QQmlApplicationEngine>
//#include <QQmlContext>

#include "application.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    Application::registerInstance();

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
