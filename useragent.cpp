#include "useragent.h"

Q_DECLARE_METATYPE(Call);

void UserAgent::ua_callback(ua *, ua_event event, call *c, const char *msg, void *arg) {
	UserAgent* _this = (UserAgent*) arg;

	switch (event) {
		case UA_EVENT_CALL_INCOMING:
			emit _this->incoming(c);
			break;
		case UA_EVENT_CALL_RINGING:
			emit _this->ringing(c);
			break;
		case UA_EVENT_CALL_ESTABLISHED:
			// HACK: Maybe not the best place to set this
			call_enable_rtp_timeout(c, 4000);
			emit _this->connected(c);
			break;
		case UA_EVENT_CALL_CLOSED:
			if (strstr(msg, "rtp error")) {
				emit _this->interrupted(c);
			} else {
				emit _this->disconnected(c);
			}
			break;
		case UA_EVENT_CALL_PROGRESS:
			qInfo() << "UA_EVENT_CALL_PROGRESS" << msg;
			break;
		default:
			qInfo() << "UA_EVENT_OTHER" << event << msg;
			break;
    //    emit _this->disconnected(c);
    //    break;

        //case UA_
    }
}

void UserAgent::connect(QString target_uri) {
    struct call* callp;
    ua_connect(uag_current(), &callp, 0, target_uri.toLatin1().data(), 0, VIDMODE_ON);
}

void UserAgent::accept(Call c) {
    ua_answer(uag_current(), c);
}

void UserAgent::hangup(Call c) {
    ua_hangup(uag_current(), c, 0, 0);
}

// Workaround for presence module not noticing new presence
// Note: will only work with statically built libbaresip
extern "C" void notifier_update_status(ua*);

void UserAgent::setPresence(int status) {
    ua_presence_status_set(uag_current(), (::presence_status)status);
    notifier_update_status(uag_current());
}
