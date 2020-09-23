#ifndef USEREDITGUI_H
#define USEREDITGUI_H
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QComboBox>

class UserEditWndGui :
public QDialog
{
protected:
    QVBoxLayout layoutMain;
    QGridLayout layoutTop;
    QHBoxLayout layoutBottom;
    QLabel lblUserName;
    QLabel lblPassword1;
    QLabel lblPassword2;
    QLabel lblFNSN;
    QLabel lblPosition;
    QLabel lblAccess;
    QLabel lblInfo;
    QLineEdit edtUserName;
    QLineEdit edtPassword1;
    QLineEdit edtPassword2;
    QLineEdit edtFNSN;
    QLineEdit edtPosition;
    QComboBox cmbAccess;
    QPushButton btnAccept;
    QPushButton btnReject;

    UserEditWndGui(QWidget *parent);
};

#endif // USEREDITGUI_H
