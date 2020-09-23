#include "moto_clock.h"
#include <QTime>
#include <QTimerEvent>
#include <QDateTime>
#include <qdebug.h>
MotoClock::MotoClock(QWidget *parent):
    timerId(0)
{
    layout = new QHBoxLayout();
    lblDateNTime = new QLabel (this);
    name = new QLabel(this);
    name->setText("МЧ");
    lblDateNTime->setStyleSheet("color: rgb(105, 165, 255);"
                                "background-color: rgb(0,0,0)");
    layout->addWidget(name);
    layout->addWidget(lblDateNTime);
    QTime time = QTime::currentTime();
    while(time == QTime::currentTime())
        ;
    timerId = this->startTimer(1000);

    lblDateNTime->setFixedSize(70,12);
}
MotoClock::~MotoClock()
{
    if(timerId)
        killTimer(timerId);
}
void MotoClock::updateDateNTime()
{
    QTime dateTime = QTime::currentTime();

    lblDateNTime->setText(dateTime.toString("hhhhh:mm:ss"));
}

void MotoClock::timerEvent(QTimerEvent *eventInfo)
{
    if(eventInfo->timerId() == timerId)
        updateDateNTime();
    else
        MotoClock::timerEvent(eventInfo);
}
