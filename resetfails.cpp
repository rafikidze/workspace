#include "resetfails.h"
#include "ui_resetfails.h"

ResetFails::ResetFails(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResetFails)
{
    ui->setupUi(this);
    is_reset_fail = false;
}

ResetFails::~ResetFails()
{
    delete ui;
}
void ResetFails::set_name_up(QString name)
{
    QString res("Сброс аварии\n");
    res.append(name);
    ui->btn_reset_fail_up->setText(res);
}
void ResetFails::set_name_down(QString name)
{
    QString res("Сброс аварии\n");
    res.append(name);
    ui->btn_reset_fail_down->setText(res);
}

void ResetFails::on_btn_reset_fail_up_clicked()
{
    if(is_reset_fail)
    {
        is_reset_fail = false;
        ui->btn_reset_fail_up->setEnabled(true);
        ui->btn_reset_fail_up->setChecked(false);
    }
    else
    {
        ui->btn_reset_fail_up->setEnabled(false);
        is_reset_fail = true;
        QTimer::singleShot(2000, this, SLOT(on_btn_reset_fail_up_clicked()));

    }
    emit sig_reset_up(is_reset_fail);
}

void ResetFails::on_btn_reset_fail_down_clicked()
{
    if(is_reset_fail)
    {
        is_reset_fail = false;
        ui->btn_reset_fail_down->setEnabled(true);
        ui->btn_reset_fail_down->setChecked(false);
    }
    else
    {
        ui->btn_reset_fail_down->setEnabled(false);
        is_reset_fail = true;
        QTimer::singleShot(2000, this, SLOT(on_btn_reset_fail_down_clicked()));

    }


    emit sig_reset_down(is_reset_fail);
}
void ResetFails::set_button_up_visible(bool value)
{
    ui->btn_reset_fail_up->setVisible(value);
}
void ResetFails::set_button_down_visible(bool value)
{
    ui->btn_reset_fail_down->setVisible(value);
}

