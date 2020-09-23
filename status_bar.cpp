#include <QTime>
#include <QTimerEvent>
#include <QDateTime>
#include <qdebug.h>
#include "status_bar.h"

StatusBar::StatusBar(QWidget *parent):
    timerId(0)
{

    lblDateNTime = new QLabel (this);
    lblDateNTime->setStyleSheet("color: rgb(255, 255, 255);");
    QTime time = QTime::currentTime();
    while(time == QTime::currentTime())
        ;
    timerId = this->startTimer(1000);


    lblDateNTime->setFixedSize(350,40);
}
StatusBar::~StatusBar()
{
    if(timerId)
        killTimer(timerId);
}
void StatusBar::updateDateNTime()
{
    QDateTime dateTime = QDateTime::currentDateTime();
    //lblDateNTime->setText( QDateTime::currentDateTime().toString( "dd.MM.yy hh:mm:ss" ) );

    lblDateNTime->setText(dateTime.toString("hh:mm:ss   dd.MM.yyyy"));
}

void StatusBar::timerEvent(QTimerEvent *eventInfo)
{
    if(eventInfo->timerId() == timerId)
        updateDateNTime();
    else
        StatusBar::timerEvent(eventInfo);
}
void StatusBar::setQFont(QFont font)
{
    lblDateNTime->setFont(font);
}
QFont StatusBar::retQFont()
{
    return lblDateNTime->font();
}
