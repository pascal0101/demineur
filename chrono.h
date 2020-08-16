#ifndef CHRONO_H
#define CHRONO_H

#include <QObject>
#include <QLCDNumber>
#include <QTimer>
#include <QTime>
#include <QLabel>
#include <QPushButton>

class Chrono : public QObject
{
    Q_OBJECT
public:
    explicit Chrono(QObject *parent = nullptr);

    public:
        QTime startTime;
        QTimer t;
        QLabel * l;
        QPushButton *start;
        QPushButton *stop;
        QPushButton *reset;
        QLCDNumber *m_lcd;
        QTimer *timer_chrono;
        int countTimer;

public slots:
    void onBtnClicked();
    void onTimer_Tick();
    void onButton_Start();
    void onButton_Reset();
    void resetCount();
    void updateCount();
signals:

};

#endif // CHRONO_H
