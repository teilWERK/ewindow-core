#ifndef CONTACTLISTMODEL_H
#define CONTACTLISTMODEL_H

#include <QAbstractListModel>

class ContactListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    ContactListModel();
    ~ContactListModel() {}


    enum ContactRoles {
        NameRole = Qt::UserRole + 1,
        URIRole,
        PresenceStatusRole,
    };

    Q_ENUMS(ContactRoles)

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

public slots:
    void update();
};

#endif // CONTACTLISTMODEL_H
