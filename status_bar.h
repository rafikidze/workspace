#ifndef STATUS_BAR_H
#define STATUS_BAR_H
#include <QLabel>
#include <QWidget>
#include <QHBoxLayout>

class StatusBar: public QWidget
{
public:
    StatusBar(QWidget *parent);
    ~StatusBar();
    QLabel *lblDateNTime;
  //  QVBoxLayout *layout;

    void setQFont(QFont);
    QFont retQFont();

private:
    int timerId;
    void updateDateNTime();
protected:
    void timerEvent(QTimerEvent *eventInfo);

};

#endif // STATUS_BAR_H
