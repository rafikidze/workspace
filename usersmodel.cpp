#include "usersmodel.h"
#include "application.h"
#include "qdebug.h"
#include <QHeaderView>
namespace
{
    const char DOMDOC_NAME[] = "Users_1";
    const char ROOT_NAME[] = "users";
    const char RECORD_NAME[] = "record";
    const char NAME_NAME[] = "name";
    const char HASH_NAME[] = "hash";
    const char FNSN_NAME[] = "fnsn";
    const char POSITION_NAME[] = "position";
    const char ACCESS_NAME[] = "access";
}

const char UsersModel::SUPERUSER[] = "root";
const char UsersModel::SUPERUSERHASH[] = "\xed\x1b\x73\x51\xa5\xfa\x99\xfd\xdd\x9c\x88\xbb\xbb\x2c\x89\xfb";

UsersModel::UsersModel(QObject *parent) :
QAbstractTableModel(parent)
{
    horizontalHeaderLabels.append("№");
    horizontalHeaderLabels.append("Имя польз.");
    horizontalHeaderLabels.append("ФИО");
    horizontalHeaderLabels.append("Должность");
    horizontalHeaderLabels.append("Права");

    UserRecord superuserRec;
    superuserRec.userName = SUPERUSER;
    superuserRec.hash = QByteArray(SUPERUSERHASH, sizeof SUPERUSERHASH - 1);
    superuserRec.accessLevel = Access::LVL_ADMIN;
    usersRecords.append(superuserRec);
}

UsersModel::~UsersModel()
{
}

QAbstractTableModel *UsersModel::model()
{
    return this;
}

int UsersModel::retUsersCount() const
{
    return static_cast<int>(usersRecords.size() - 1);
}

bool UsersModel::isUserExists(const QString &userName) const
{
    for(unsigned int i = 0; i < retUsersCount() + 1; i++)
    {
        if(usersRecords.at(i).userName == userName)
            return true;
    }
    return false;
}

QByteArray UsersModel::retUserHash(const QString &userName) const
{
    for(int i = 0; i < retUsersCount() + 1; i++)
    {
        if(usersRecords.at(i).userName == userName)
            return usersRecords.at(i).hash;
    }
    throw int(0);
}

QString UsersModel::retUserFNSN(const QString &userName) const
{
    for(int i = 0; i < retUsersCount() + 1; i++)
    {
        if(usersRecords.at(i).userName == userName)
            return usersRecords.at(i).fnsn;
    }
    throw int(0);
}

QString UsersModel::retUserPosition(const QString &userName) const
{
    for(int i = 0; i < retUsersCount() + 1; i++)
    {
        if(usersRecords.at(i).userName == userName)
            return usersRecords.at(i).position;
    }
    throw int(0);
}

Access::LEVEL UsersModel::retUserAccessLevel(const QString &userName) const
{
    for(int i = 0; i < retUsersCount() + 1; i++)
    {
        if(usersRecords.at(i).userName == userName)
            return usersRecords.at(i).accessLevel;
    }
    throw int(0);
}

void UsersModel::setUserInfo(const QString &userName, const QByteArray &hash, const QString &fnsn, const QString &position, Access::LEVEL accessLvl)
{
    for(int i = 1; i < retUsersCount() + 1; i++)
    {
        if(usersRecords.at(i).userName == userName)
        {
            beginInsertRows(QModelIndex(), rowCount(), rowCount());
            usersRecords[i].hash = hash;
            usersRecords[i].fnsn = fnsn;
            usersRecords[i].position = position;
            usersRecords[i].accessLevel = accessLvl;
            endInsertRows();
            beginRemoveColumns(QModelIndex(), rowCount() - 1, rowCount() - 1);
            beginRemoveRows(QModelIndex(), rowCount() - 1, rowCount() - 1);
            endRemoveRows();
        }
    }
    if(! isUserExists(userName))
    {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        UserRecord newRec;
        newRec.userName = userName;
        newRec.hash = hash;
        newRec.fnsn = fnsn;
        newRec.position = position;
        newRec.accessLevel = accessLvl;
        usersRecords.append(newRec);
        endInsertRows();
    }
    else
        ;//SuperUser
}

void UsersModel::removeUser(const QModelIndex& index)
{
    if(index.isValid() && hasIndex(index.row(), index.column(), index.parent()))
    {
        LogRecordDB *rec = new LogRecordDBAccount (LogRecordDBAccount::F_Remove,usersRecords.at(index.row() + 1).userName);
        Application::slot_send_msg_to_log(*rec);
        beginRemoveRows(QModelIndex(), index.row(), index.row());
        usersRecords.removeAt(index.row() + 1);
        endRemoveRows();
    }
}

QString UsersModel::retUserName(const QModelIndex& index) const
{
    if(index.isValid() && hasIndex(index.row(), index.column(), index.parent()))
        return usersRecords.at(index.row() + 1).userName;
    throw int(0);
}

QString UsersModel::retFNSN(const QModelIndex& index) const
{
    if(index.isValid() && hasIndex(index.row(), index.column(), index.parent()))
        return usersRecords.at(index.row() + 1).fnsn;
    throw int(0);
}

QString UsersModel::retPosition(const QModelIndex& index) const
{
    if(index.isValid() && hasIndex(index.row(), index.column(), index.parent()))
        return usersRecords.at(index.row() + 1).position;
    throw int(0);
}

Access::LEVEL UsersModel::retAccessLevel(const QModelIndex& index) const
{
    if(index.isValid() && hasIndex(index.row(), index.column(), index.parent()))
        return usersRecords.at(index.row() + 1).accessLevel;
    throw int(0);
}

void UsersModel::getUserInfo(int i, QString &name, QByteArray &hash, QString &fnsn, QString &position, Access::LEVEL &accessLevel) const
{
    if(i + 1 < usersRecords.size())
    {
        name = usersRecords.at(i + 1).userName;
        hash = usersRecords.at(i + 1).hash;
        fnsn = usersRecords.at(i + 1).fnsn;
        position = usersRecords.at(i + 1).position;
        accessLevel = usersRecords.at(i + 1).accessLevel;
    }
}

void UsersModel::clear()
{
    while(usersRecords.size() > 1)
        usersRecords.removeAt(1);
}

QModelIndex UsersModel::buddy(const QModelIndex& index) const
{
    return QAbstractTableModel::buddy(index);
}

bool UsersModel::canFetchMore(const QModelIndex& parent) const
{
    return QAbstractTableModel::canFetchMore(parent);
}

int UsersModel::columnCount(const QModelIndex& parent) const
{
    if(parent.isValid())
        return 0;
    return horizontalHeaderLabels.size();
}

QVariant UsersModel::data(const QModelIndex& index, int role) const
{
    if(index.isValid() && hasIndex(index.row(), index.column(), index.parent()))
    {
        switch(role)
        {
        case Qt::DisplayRole:
            switch(index.column())
            {
            case 0:
                return index.row() + 1;
            case 1:
                return usersRecords.at(index.row() + 1).userName;
            case 2:
                return usersRecords.at(index.row() + 1).fnsn;
            case 3:
                return usersRecords.at(index.row() + 1).position;
            case 4:
                return Access::LEVELS[usersRecords.at(index.row() + 1).accessLevel];
            }
        }
    }
    return QVariant();
}

bool UsersModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex& parent)
{
    return QAbstractTableModel::dropMimeData(data, action, row, column, parent);
}

void UsersModel::fetchMore(const QModelIndex& parent)
{
    return QAbstractTableModel::fetchMore(parent);
}

Qt::ItemFlags UsersModel::flags(const QModelIndex& index) const
{
    return QAbstractTableModel::flags(index);
}

QVariant UsersModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(section < horizontalHeaderLabels.size())
    {
        if(orientation == Qt::Horizontal)
        {
            if(role == Qt::DisplayRole)
                return QVariant(horizontalHeaderLabels.at(section));
        }
    }
    return QVariant::Invalid;
}

QModelIndex UsersModel::index(int row, int column, const QModelIndex& parent) const
{
    if(column < columnCount(parent) && row < rowCount(parent))
        return createIndex(row, column);
    return QModelIndex();
}

bool UsersModel::insertColumns(int column, int count, const QModelIndex& parent)
{
    return false;
}

bool UsersModel::insertRows(int row, int count, const QModelIndex& parent)
{
    return false;
}

QMap<int, QVariant> UsersModel::itemData(const QModelIndex& index) const
{
    return QAbstractTableModel::itemData(index);
}

QModelIndexList UsersModel::match(const QModelIndex& start, int role, const QVariant& value, int hits, Qt::MatchFlags flags) const
{
    return QAbstractTableModel::match(start, role, value, hits, flags);
}

QMimeData *UsersModel::mimeData(const QModelIndexList& indexes) const
{
    return QAbstractTableModel::mimeData(indexes);
}

QStringList UsersModel::mimeTypes() const
{
    return QAbstractTableModel::mimeTypes();
}

QModelIndex UsersModel::parent(const QModelIndex& child) const
{
    return QModelIndex();
}

bool UsersModel::removeColumns(int column, int count, const QModelIndex& parent)
{
    return false;
}

bool UsersModel::removeRows(int row, int count, const QModelIndex& parent)
{
    return false;
}

int UsersModel::rowCount(const QModelIndex& parent) const
{
    return retUsersCount();
}

bool UsersModel::setData(const QModelIndex &index, const QVariant& value, int role)
{
    return false;
}

bool UsersModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant& value, int role)
{
    return false;
}

bool UsersModel::setItemData(const QModelIndex& index, const QMap<int, QVariant>& roles)
{
    return false;
}

void UsersModel::sort(int column, Qt::SortOrder order)
{
    return QAbstractTableModel::sort(column, order);
}

QSize UsersModel::span(const QModelIndex& index) const
{
    return QAbstractTableModel::span(index);
}

Qt::DropActions UsersModel::supportedDropActions() const
{
    return QAbstractTableModel::supportedDropActions();
}


bool loadUsersFromFile(UsersModel &model, QFile &file)
{
    if(file.isOpen() && file.openMode() & QFile::ReadOnly)
    {
        QDomDocument domDoc;
        if(domDoc.setContent(&file))
        {
            if(domDoc.doctype().name() == DOMDOC_NAME)
            {
                model.clear();
                QDomElement root = domDoc.elementsByTagName(ROOT_NAME).at(0).toElement();
                for(int i = 0; i < root.childNodes().size(); i++)
                {
                    QString name = root.elementsByTagName(RECORD_NAME).at(i).toElement().attribute(NAME_NAME);
                    QByteArray hash = QByteArray::fromHex(root.elementsByTagName(RECORD_NAME).at(i).toElement().attribute(HASH_NAME).toLatin1());
                    QString fnsn = root.elementsByTagName(RECORD_NAME).at(i).toElement().attribute(FNSN_NAME);
                    QString position = root.elementsByTagName(RECORD_NAME).at(i).toElement().attribute(POSITION_NAME);
                    QString access = root.elementsByTagName(RECORD_NAME).at(i).toElement().attribute(ACCESS_NAME);
                    Access::LEVEL accessLevel;
                    if(access == Access::LEVELS[0])
                        accessLevel = Access::LVL_USER;
                    else if(access == Access::LEVELS[1])
                        accessLevel = Access::LVL_ADMIN;
                    else return false;
                    model.setUserInfo(name, hash, fnsn, position, accessLevel);
                }
                return true;
            }
        }
    }
  return false;
}

bool saveUsersToFile(UsersModel &model, QFile &file)
{
    if(file.isOpen() && file.openMode() & QFile::WriteOnly)
    {
        QDomDocument domDoc(DOMDOC_NAME);
        QDomElement root = domDoc.createElement(ROOT_NAME);
        for(int i = 0; i < model.retUsersCount(); i++)
        {
            QString name;
            QByteArray hash;
            QString fnsn;
            QString position;
            Access::LEVEL accessLevel;
            model.getUserInfo(i, name, hash, fnsn, position, accessLevel);
            QString access = Access::LEVELS[accessLevel];
            QDomElement record = domDoc.createElement(RECORD_NAME);
            record.setAttribute(NAME_NAME, name);
            record.setAttribute(HASH_NAME, QString(hash.toHex()));
            record.setAttribute(FNSN_NAME, fnsn);
            record.setAttribute(POSITION_NAME, position);
            record.setAttribute(ACCESS_NAME, access);
            root.appendChild(record);
        }
        domDoc.appendChild(root);
        file.seek(0);
        QTextStream stream(&file);
        stream << domDoc.toString(0);
        stream.flush();
        return true;
    }
    return false;
}


UsersView::UsersView(QWidget *parent) :
QTableView(parent)
{
    horizontalHeader()->setSectionsClickable(false);
    horizontalHeader()->setHighlightSections(false);

    setStyleSheet( "QTableView"
                   "{"
                   "    border: 2px solid #666666;"
                   "    border-radius: 5px;"
                   "    background: #FFFFFF;"
                   "}"
                   "QHeaderView"
                   "{"
                   "    color: #000000"
                   "}"
                   "QHeaderView::section { background-color: rgb(220, 220, 220) }"
                   "QScrollBar:vertical {background-color: rgb(194, 194, 194) }" );


    horizontalHeader()->setStyleSheet("color: #000000");

    verticalHeader()->hide();

    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

}

void UsersView::setModel(QAbstractItemModel *model)
{
    QTableView::setModel(model);
    setColumnWidth(0, 40);
    setColumnWidth(1, 140);
    setColumnWidth(3, 140);
    setColumnWidth(4, 140);
    horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
    horizontalHeader()->setSectionResizeMode(4, QHeaderView::Fixed);
}

void UsersView::mouseReleaseEvent(QMouseEvent *eventInfo)
{
    QTableView::mouseReleaseEvent(eventInfo);
    if(currentIndex().isValid())
        emit sigFocusChanged(true);
}

void UsersView::focusInEvent(QFocusEvent *eventInfo)
{
    QTableView::focusInEvent(eventInfo);
}

void UsersView::focusOutEvent(QFocusEvent *eventInfo)
{
    QTableView::focusOutEvent(eventInfo);
}
