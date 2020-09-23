#ifndef USER_EDIT_WND_H
#define USER_EDIT_WND_H
#include "usereditgui.h"
#include "access.h"

class UserEditWnd :
public UserEditWndGui
{
    Q_OBJECT
public:
    UserEditWnd(QWidget *parent);
    UserEditWnd(QWidget *parent, const QString &userName, const QString &fnsn, const QString &position, Access::LEVEL accessLvl);
    ~UserEditWnd();
    QString retUserName() const;
    QString retPassword() const;
    QString retFNSN() const;
    QString retPosition() const;
    Access::LEVEL retAccessLevel() const;
protected:
    bool edit;

    virtual void showEvent(QShowEvent *eventInfo);
    virtual void accept();
};


#endif // USER_EDIT_WND_H
