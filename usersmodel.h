#ifndef USERSMODEL_H
#define USERSMODEL_H
#include <QFile>
#include <QSize>
#include <QObject>
#include <QTableView>
#include <QStringList>
#include <QAbstractItemModel>


#include "access.h"
class UsersModel;

bool loadUsersFromFile(UsersModel &model, QFile &file);
bool saveUsersToFile(UsersModel &model, QFile &file);
class UsersModel:
        public QAbstractTableModel
{
public:
    UsersModel(QObject *parent);
    virtual ~UsersModel();
    virtual QAbstractTableModel *model();
    virtual int retUsersCount() const;
    virtual bool isUserExists(const QString &userName) const;
    virtual QByteArray retUserHash(const QString &userName) const;
    virtual QString retUserFNSN(const QString &userName) const;
    virtual QString retUserPosition(const QString &userName) const;
    virtual Access::LEVEL retUserAccessLevel(const QString &userName) const;
    virtual void setUserInfo(const QString &userName, const QByteArray &hash, const QString &fnsn, const QString &position, Access::LEVEL accessLvl);
    virtual void removeUser(const QModelIndex& index);
    virtual QString retUserName(const QModelIndex& index) const;
    virtual QString retFNSN(const QModelIndex& index) const;
    virtual QString retPosition(const QModelIndex& index) const;
    virtual Access::LEVEL retAccessLevel(const QModelIndex& index) const;
    virtual void getUserInfo(int i, QString &name, QByteArray &hash, QString &fnsn, QString &position, Access::LEVEL &accessLevel) const;
    virtual void clear();
protected:
    virtual QModelIndex buddy(const QModelIndex& index) const;
    virtual bool canFetchMore(const QModelIndex& parent) const;
    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex& parent);
    virtual void fetchMore(const QModelIndex& parent);
    virtual Qt::ItemFlags flags(const QModelIndex& index) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
    virtual bool insertColumns(int column, int count, const QModelIndex& parent);
    virtual bool insertRows(int row, int count, const QModelIndex& parent);
    virtual QMap<int, QVariant> itemData(const QModelIndex& index) const;
    virtual QModelIndexList match(const QModelIndex& start, int role, const QVariant& value, int hits = 1, Qt::MatchFlags flags = Qt::MatchFlags(Qt::MatchStartsWith | Qt::MatchWrap)) const;
    virtual QMimeData *mimeData(const QModelIndexList& indexes) const;
    virtual QStringList mimeTypes() const;
    virtual QModelIndex parent(const QModelIndex& child) const;
    virtual bool removeColumns(int column, int count, const QModelIndex& parent = QModelIndex());
    virtual bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex());
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
    virtual bool setHeaderData(int section, Qt::Orientation orientation, const QVariant& value, int role = Qt::EditRole);
    virtual bool setItemData(const QModelIndex& index, const QMap<int, QVariant>& roles);
    virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);
    virtual QSize span(const QModelIndex& index) const;
    virtual Qt::DropActions supportedDropActions() const;


private:
    static const char SUPERUSER[];
    static const char SUPERUSERHASH[];
    class UserRecord
    {
    public:
        QString userName;
        QByteArray hash;
        QString fnsn;
        QString position;
        Access::LEVEL accessLevel;
    };

    QStringList horizontalHeaderLabels;
    QList<UserRecord> usersRecords;

    UsersModel(const UsersModel& src);
    UsersModel& operator =(const UsersModel& src);


};

class UsersView :
public QTableView
{
    Q_OBJECT
public:
    UsersView(QWidget *parent);
    virtual void setModel(QAbstractItemModel *model);
signals:
    void sigFocusChanged(bool focused);
protected:
    void mouseReleaseEvent(QMouseEvent *eventInfo);
    void focusInEvent(QFocusEvent *eventInfo);
    void focusOutEvent(QFocusEvent *eventInfo);
};



#endif // USERSMODEL_H
