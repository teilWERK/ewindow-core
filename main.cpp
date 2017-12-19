#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QThread>

#include <QOpenGLContext>

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
	// Try EGLFS when X server not present
	if (!getenv("DISPLAY")) {
		setenv("QT_QPA_PLATFORM", "eglfs", 0);
	}

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
	qmlRegisterSingletonType<UserAgent>("org.ewindow.ua", 0, 1, "UserAgent", &UserAgent::newInstance);
	//qmlRegisterType<UserAgent>("org.ewindow", 0, 1, "UserAgent");
	qmlRegisterUncreatableType<BaresipVidisp>("org.ewindow", 0, 1, "VideoDisplay", "VideoDisplay is created by the backend, use the onNewVideo callback");
	qmlRegisterUncreatableType<ContactListModel>("org.ewindow", 0, 1, "ContactListModel", "ContactListModel is passed by the application through 'contactListModel'");

	engine.load(QUrl(QStringLiteral("gui/main.qml")));
	if (engine.rootObjects().isEmpty())
		return -1;

	return app.exec();
	}
