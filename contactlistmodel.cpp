#include "contactlistmodel.h"

#include <QTimer>
#include <QDebug>

#include <re.h>
#include <baresip.h>

void ContactListModel::update() {
    //qInfo() << "ContactListModel::update" << rowCount() << topleft << bottomright;
    //emit dataChanged(topleft, bottomright);


    /*pl foo;
    pl_set_str(&foo, "\"cool dname\" <sip:test@example.com>");
    contact_add(baresip_contacts(), 0, &foo);
    */

    beginResetModel();
    endResetModel();
}

ContactListModel::ContactListModel()
{
    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));

    timer->start(1000);
}

int ContactListModel::rowCount(const QModelIndex &parent) const
{
    struct contacts* c = baresip_contacts();
    struct list* cl = contact_list(c);
    return list_count(cl);
}

QVariant ContactListModel::data(const QModelIndex &index, int role) const
{
    Q_ASSERT(index.column() == 0);
    struct contacts* c = baresip_contacts();
    struct list* cl = contact_list(c);

    struct le* entry = cl->head;

    for (int i = 0; i < index.row(); i++) {
        entry = entry->next;

        if (!entry) {
            qWarning() << "Accessing invalid index " << index.row() << "of contact list";
            return 0;
        }
    }

    switch (role) {
        case NameRole: {
            struct pl dname = contact_addr((contact*) entry)->dname;
            return QLatin1String(dname.p, dname.l);
        }
        case URIRole: {
            struct pl uri = contact_addr((contact*) entry)->auri;
            return QLatin1String(uri.p, uri.l);

        }
        case PresenceStatusRole:
            return contact_presence((contact*) entry);
        default:
            qWarning() << "ContactListModel: Accessing unknown role" << role;
            return 0;
    }
}

QHash<int, QByteArray> ContactListModel::roleNames() const
{
       QHash<int, QByteArray> roles;
       roles[NameRole] = "dname";
       roles[PresenceStatusRole] = "presence";
       roles[URIRole] = "uri";
       return roles;
   }
