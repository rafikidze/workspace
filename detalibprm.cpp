#include "detalibprm.h"
#include "ui_detalibprm.h"

DetaliBPRM::DetaliBPRM(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DetaliBPRM)
{
    ui->setupUi(this); 
    led_green.load(":/img/green_round_small.png");
    led_yellow.load(":/img/yellow_round_small.png");
    led_red.load(":/img/red_round_small.png");
    led_gray.load(":/img/black_round_small.png");

}

DetaliBPRM::~DetaliBPRM()
{
    delete ui;
}

void DetaliBPRM::on_btn_show_panel_clicked(bool checked)
{
    emit sig_hide_panels();
    emit sig_show_panel(checked);
}
void DetaliBPRM::set_leds(RTEState state)
{
    // perhaps, u should watch only for changing state
    // not always, like this

    if(state._AV_PAR)  set_led_light(ui->small_led_drive_bprm,1,Color::red);
    else if(state._Rez_PAR && state._P200) set_led_light(ui->small_led_drive_bprm,1,Color::yellow);
    else if((state._1k || state._2k) && state._P200) set_led_light(ui->small_led_drive_bprm,1,Color::green);
    else set_led_light(ui->small_led_drive_bprm,1,Color::gray);

    if(state._AV_MRM) set_led_light(ui->small_led_mrm_bprm,1,Color::red);
    else if(state._Rez_MRM && (state._P200_MRM1 || state._P200_MRM2 )) set_led_light(ui->small_led_mrm_bprm,1,Color::yellow);
    else if((state._MRM1 || state._MRM2) && (state._P200_MRM1 || state._P200_MRM2 )) set_led_light(ui->small_led_mrm_bprm,1,Color::green);
    else set_led_light(ui->small_led_mrm_bprm,1,Color::gray);


    if(state._AV_DES) set_led_light(ui->small_led_dea_bprm,1,Color::red);
    else if(state._Zapr_puska) set_led_light(ui->small_led_dea_bprm,1,Color::yellow);
    else if(state._Rab_DES) set_led_light(ui->small_led_dea_bprm,1,Color::green);
    else set_led_light(ui->small_led_dea_bprm,1,Color::gray);
}


void DetaliBPRM::fail_232_bprm(bool status)
{

    // если авания линии связи rs-232, то следует зажечь красным
    // 1 - большие светодиоды на главной панели: ПРИВОД, МРМ, ДЭА
    // 2 - маленькие круглые светодиоды
    // 3 - светодиод "РПДУ исправен"
    // в больших светодиодах в черных квадратиках следует убрать цифры о включенных
    // полукомплектах, если они были
    // если снят сигнал аварии линии связи rs-232, то следует вернуть все состояния назад
    // (все, что в запросах вернется само)

     if (status){
            set_led_light(ui->small_led_drive_bprm,1,Color::red);
            set_led_light(ui->small_led_mrm_bprm,1,Color::red);
            set_led_light(ui->small_led_dea_bprm,1,Color::red);

        // sound
    }


}
void DetaliBPRM::set_led_light(QLabel* label, unsigned char value, Color color)
{
    if(value)
    {
        switch(color)
        {
        case green: label->setPixmap(led_green);   break;
        case yellow: label->setPixmap(led_yellow); break;
        case red: label->setPixmap(led_red); break;
        case gray: label->setPixmap(led_gray); break;
        case black: label->setStyleSheet(" background-color: rgb(0, 0, 0)");break;
        default: label->setPixmap(led_gray); break;
        }
    }
    else label->setPixmap(led_gray);
}

void DetaliBPRM::set_name(QString name)
{
    ui->label_name->setText(name);
}
void DetaliBPRM::set_checked(bool checked)
{
    ui->btn_show_panel->setChecked(checked);
}
