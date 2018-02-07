#ifndef VOLUMEMANAGER_H
#define VOLUMEMANAGER_H

#include <QQuickItem>

#include <sstream>

class QQmlEngine;
class QJSEngine;

class VolumeManager : public QQuickItem {
    Q_OBJECT


public:

    static QObject* newInstance(QQmlEngine*, QJSEngine*) {
		return new VolumeManager;
	}

signals:

public slots:
    void setVolume(int percent);
};

#endif
