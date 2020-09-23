#ifndef MOTO_CLOCK_H
#define MOTO_CLOCK_H
#include <QLabel>
#include <QWidget>
#include "status_bar.h"
class MotoClock: public QWidget
{
public:
    MotoClock(QWidget *parent);
    ~MotoClock();
    QLabel *lblDateNTime;
    QLabel *name;
   // QFrame *frame;
    QHBoxLayout *layout;
private:
    int timerId;
    void updateDateNTime();
protected:
    void timerEvent(QTimerEvent *eventInfo);
};

#endif // MOTO_CLOCK_H
