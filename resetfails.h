#ifndef RESETFAILS_H
#define RESETFAILS_H

#include <QTimer>
#include <QWidget>

namespace Ui {
class ResetFails;
}

class ResetFails : public QWidget
{
    Q_OBJECT

public:
    explicit ResetFails(QWidget *parent = nullptr);
    ~ResetFails();
public slots:
    void set_name_up(QString);
    void set_name_down(QString);
    void set_button_up_visible(bool);
    void set_button_down_visible(bool);

private slots:
    void on_btn_reset_fail_up_clicked();
    void on_btn_reset_fail_down_clicked();


private:
    Ui::ResetFails *ui;
    bool is_reset_fail;
signals:
    void sig_reset_up(bool);
    void sig_reset_down(bool);
};

#endif // RESETFAILS_H
