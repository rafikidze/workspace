#ifndef DETALIBPRM_H
#define DETALIBPRM_H

#include <QLabel>
#include <QWidget>
#include "datatypes.h"

namespace Ui {
class DetaliBPRM;
}

class DetaliBPRM : public QWidget
{
    Q_OBJECT

public:
    explicit DetaliBPRM(QWidget *parent = nullptr);
    ~DetaliBPRM();
public slots:
    void set_leds(RTEState);
    void set_name(QString);
    void set_checked(bool);
    void fail_232_bprm(bool status);


private slots:
    void on_btn_show_panel_clicked(bool checked);
    void set_led_light(QLabel* label, unsigned char value, Color color);


private:
    Ui::DetaliBPRM *ui;
    QPixmap led_green;
    QPixmap led_yellow;
    QPixmap led_red;
    QPixmap led_gray;
signals:
    void sig_show_panel(bool);
    void sig_hide_panels();
};

#endif // DETALIBPRM_H
