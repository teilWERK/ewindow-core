#include <rtc_base/thread.h>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QOpenGLContext>
#include <QLibraryInfo>

#include <QDebug>

#include <unistd.h>
#include <libgen.h>

#include <cassert>


#include "baresipcore.h"
#include "baresipvidisp.h"


int main(int argc, char *argv[])
{
	// Try EGLFS when X server not present
	if (!getenv("DISPLAY")) {
		setenv("QT_QPA_PLATFORM", "eglfs", 0);
	}

	QGuiApplication::setAttribute(Qt::AA_UseOpenGLES);
	QGuiApplication app(argc, argv);

	QQmlApplicationEngine engine;

	//	qRegisterMetaType<Call>();

	// two mean hacks to get data inside QML
	//engine.rootContext()->setContextProperty("contactListModel", &clModel);
	engine.rootContext()->setContextProperty("baresipCore", &BaresipCore::instance());

	qmlRegisterInterface<BaresipCore>("BaresipCore");
	//qmlRegisterSingletonType<UserAgent>("org.ewindow.ua", 0, 1, "UserAgent", &UserAgent::newInstance);
	//qmlRegisterSingletonType<UserAgent>("org.ewindow", 0, 1, "VolumeManager", &VolumeManager::newInstance);
	qmlRegisterUncreatableType<BaresipVidisp>("org.ewindow", 0, 1, "VideoDisplay", "VideoDisplay is created by the backend, use the onNewVideo callback");
	//qmlRegisterUncreatableType<ContactListModel>("org.ewindow", 0, 1, "ContactListModel", "ContactListModel is passed by the application through 'contactListModel'");


	//rtc::AutoSocketServerThread thread(0);

	// This needs to be instantianted before, for WebRTC's internal threading to work
	rtc::AutoThread thread;
	//thread.Run();
	
	// Change working dir to exe's dir, so it will find the .qml's
	
	chdir(dirname(argv[0]));
	engine.load(QUrl(QStringLiteral("gui/main.qml")));
	if (engine.rootObjects().isEmpty())
		return -1;

	return app.exec();
	}
