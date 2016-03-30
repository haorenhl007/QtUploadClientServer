#include <QGuiApplication>
#include <QQmlApplicationEngine>
//#include <QQmlContext>

#include "application.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
//    app.setApplicationName(Application::APP_NAME);
//    app.setApplicationVersion(Application::APP_VERSION);
//    app.setOrganizationDomain(Application::ORG_DOMAIN);
//    app.setOrganizationName(Application::ORG_NAME);

    Application::registerInstance();

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
