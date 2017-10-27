#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QThread>

#include <QQuickWindow>

#include <QAbstractListModel>

extern "C" {
#include <re.h>
#include <baresip.h>
}


#include "baresipcore.h"
#include "baresipvidisp.h"

#include "contactlistmodel.h"
#include "useragent.h"

#include <QAbstractAnimation>

int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_UseOpenGLES);
    QGuiApplication app(argc, argv);

    if (!QOpenGLContext::supportsThreadedOpenGL()) {
        qInfo("Platform has no GL threading support!!!");
        //exit(1);
    }

    BaresipCore::instance().start();
    QQmlApplicationEngine engine;

    ContactListModel clModel;

    //qmlRegisterInterface<Call>("Call");
    qRegisterMetaType<Call>();

    engine.rootContext()->setContextProperty("contactListModel", &clModel);
    engine.rootContext()->setContextProperty("baresipCore", &BaresipCore::instance());

    //qmlRegisterType<BaresipCore>("org.ewindow", 0, 1, "BaresipCore");
    qmlRegisterInterface<BaresipCore>("BaresipCore");

    //qmlRegisterSingletonType<BaresipThread>("org.ewindow.phone", 0, 1, "PhoneBackend", baresipthread_singleton_provider);
    //qmlRegisterSingletonType<BaresipThread>("org.ewindow.phone", 0, 1, "PhoneBackend", 0);
    qmlRegisterType<UserAgent>("org.ewindow", 0, 1, "UserAgent");
    qmlRegisterUncreatableType<BaresipVidisp>("org.ewindow", 0, 1, "VideoDisplay", "VideoDisplay is created by the backend, use the onNewVideo callback");
    //qmlRegisterInterface<BaresipVidisp>("VideoDisplay");

    engine.load(QUrl(QStringLiteral("./main.qml")));
	if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
