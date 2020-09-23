#include "usereditgui.h"
#include "access.h"

UserEditWndGui::UserEditWndGui(QWidget *parent) :
QDialog(parent),
lblUserName(this),
lblPassword1(this),
lblPassword2(this),
lblFNSN(this),
lblPosition(this),
lblInfo(this),
edtUserName(this),
edtPassword1(this),
edtPassword2(this),
edtFNSN(this),
edtPosition(this),
cmbAccess(this),
btnAccept(this),
btnReject(this)
{
    setWindowFlags(Qt::Dialog | Qt::WindowSystemMenuHint | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    setWindowTitle("Редактирование учетной записи");
    setLayout(&layoutMain);
    layoutMain.addLayout(&layoutTop);
    layoutMain.addLayout(&layoutBottom);
    layoutTop.addWidget(&lblUserName, 0, 0);
    layoutTop.addWidget(&lblPassword1, 1, 0);
    layoutTop.addWidget(&lblPassword2, 2, 0);
    layoutTop.addWidget(&lblFNSN, 3, 0);
    layoutTop.addWidget(&lblPosition, 4, 0);
    layoutTop.addWidget(&lblAccess, 5, 0);
    layoutTop.addWidget(&edtUserName, 0, 1);
    layoutTop.addWidget(&edtPassword1, 1, 1);
    layoutTop.addWidget(&edtPassword2, 2, 1);
    layoutTop.addWidget(&edtFNSN, 3, 1);
    layoutTop.addWidget(&edtPosition, 4, 1);
    layoutTop.addWidget(&cmbAccess, 5, 1);
    layoutBottom.addWidget(&lblInfo);
    layoutBottom.addStretch();
    layoutBottom.addWidget(&btnAccept);
    layoutBottom.addWidget(&btnReject);
    lblUserName.setText("Имя пользователя*:");
    lblPassword1.setText("Пароль*:");
    lblPassword2.setText("Подтверждение пароля*:");
    lblFNSN.setText("ФИО*:");
    lblPosition.setText("Должность:");
    lblAccess.setText("Права:");
    lblInfo.setStyleSheet("font: normal 14px");
    lblInfo.setText("* - поля обязательные для заполнения");
    btnAccept.setText("Сохранить");
    btnReject.setText("Отменить");
    edtPassword1.setEchoMode(QLineEdit::Password);
    edtPassword2.setEchoMode(QLineEdit::Password);
    edtFNSN.setMinimumWidth(410);
    QMargins mainMargins = layoutMain.contentsMargins();
    mainMargins.setBottom(layoutMain.spacing());
    layoutMain.setContentsMargins(mainMargins);
    for(unsigned int i = 0; i < Access::LEVELSNUMBER; i++)
        cmbAccess.addItem(Access::LEVELS[i]);
    layoutMain.setSizeConstraint(QLayout::SetFixedSize);
}
