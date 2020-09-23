#include <QMessageBox>
#include "application.h"
#include "user_edit_wnd.h"

UserEditWnd::UserEditWnd(QWidget *parent) :
UserEditWndGui(parent),
edit(false)
{
    edtUserName.setFocus();
    setWindowTitle("Редактирование новой учетной записи");
    connect(&btnAccept, SIGNAL(clicked()), this, SLOT(accept()));
    connect(&btnReject, SIGNAL(clicked()), this, SLOT(reject()));
}

UserEditWnd::UserEditWnd(QWidget *parent, const QString &userName, const QString &fnsn, const QString &position, Access::LEVEL accessLvl) :
UserEditWndGui(parent),
edit(true)
{
    edtUserName.setText(userName);
    edtUserName.setDisabled(true);
    edtFNSN.setText(fnsn);
    edtPosition.setText(position);
    cmbAccess.setCurrentIndex(cmbAccess.findText(Access::LEVELS[accessLvl]));
    edtPassword1.setFocus();
    connect(&btnAccept, SIGNAL(clicked()), this, SLOT(accept()));
    connect(&btnReject, SIGNAL(clicked()), this, SLOT(reject()));
}

UserEditWnd::~UserEditWnd()
{
    disconnect(&btnReject, SIGNAL(clicked()), this, SLOT(reject()));
    disconnect(&btnAccept, SIGNAL(clicked()), this, SLOT(accept()));
}

QString UserEditWnd::retUserName() const
{
    return edtUserName.text();
}

QString UserEditWnd::retPassword() const
{
    return edtPassword1.text();
}

QString UserEditWnd::retFNSN() const
{
    return edtFNSN.text();
}

QString UserEditWnd::retPosition() const
{
    return edtPosition.text();
}

Access::LEVEL UserEditWnd::retAccessLevel() const
{
    if(cmbAccess.currentText() == Access::LEVELS[Access::LVL_USER])
        return Access::LVL_USER;
    else if(cmbAccess.currentText() == Access::LEVELS[Access::LVL_ADMIN])
        return Access::LVL_ADMIN;
    throw int(0);
}

void UserEditWnd::showEvent(QShowEvent *eventInfo)
{
    QApplication::setActiveWindow(this);
    QRect rect = geometry();
    rect.moveCenter(parentWidget()->contentsRect().center());
    setGeometry(rect);
}

void UserEditWnd::accept()
{
    if(! edit)
    {
        if(retUserName().size() == 0 ||
           retUserName().contains(' '))
        {
            QMessageBox::critical(this, "Ошибка", "Недопустимое имя пользователя");
            edtUserName.setFocus();
            edtUserName.selectAll();
            return;
        }
        else if(Application::retUsersModel().isUserExists(edtUserName.text()))
        {
            QMessageBox::critical(this, "Ошибка", "Пользователь с этим именем уже существует");
            edtUserName.setFocus();
            edtUserName.selectAll();
            return;
        }
        else
            ;
    }
    if(edtPassword1.text().size() == 0)
    {
        QMessageBox::critical(this, "Ошибка", "Пароль не может быть пустым");
        edtPassword1.setFocus();
        return;
    }
    else
        ;
    if(edtPassword2.text() != edtPassword1.text())
    {
        QMessageBox::critical(this, "Ошибка", "Введенные пароли не совпадают");
        edtPassword2.setFocus();
        edtPassword2.selectAll();
        return;
    }
    else
        ;
    if(edtFNSN.text().size() == 0)
    {
        QMessageBox::critical(this, "Ошибка", "ФИО не может быть пустым");
        edtFNSN.setFocus();
        return;
    }
    else
        ;
    UserEditWndGui::accept();
}
