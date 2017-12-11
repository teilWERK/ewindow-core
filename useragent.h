#ifndef USERAGENT_H
#define USERAGENT_H

#include <QQuickItem>

#include <re.h>
#include <baresip.h>

#include "contactlistmodel.h"

class Call {
    Q_GADGET

    struct call* m_call;
public:
    Call() : m_call(0) {}
    Call(call* c) : m_call(c) {}

    operator call*() { return m_call; }
    operator const call*() const {return m_call; }
};

class UserAgent : public QQuickItem {
    Q_OBJECT

    static void ua_callback(struct ua*, ua_event, call*, const char*, void* arg);

public:
    UserAgent() {
        qInfo() << "UserAgent constructed";

        uag_event_register(ua_callback, this);
    }

    ~UserAgent() {
        uag_event_unregister(ua_callback);
    }

signals:
    //void ringing(struct call* call);
    void ringing(Call call);
    void incoming(Call call);
    void connected(Call call);
    void interrupted(Call call);
    void disconnected(Call call);

public slots:
    void connect(QString target_uri);
    void hangup(Call call);
    void accept(Call call);

    // Parameter type should be enum presence_status, but due to Qt Bug #19741, int has to be used as a workaround
    void setPresence(int status);

    void startVidloop() { ui_input_str("/vidloop h264"); }
    void stopVidloop() { ui_input_str("/vidloop_stop"); }
};

#endif // USERAGENT_H
