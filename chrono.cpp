#include "chrono.h"
#include <QDebug>

Chrono::Chrono(QObject *parent) : QObject(parent)
{

}
void Chrono::resetCount(){
        countTimer=0;
    }
    void Chrono::updateCount(){
        countTimer++;
    }
    void Chrono::onTimer_Tick(){
        updateCount();
        m_lcd -> display(countTimer);
        qDebug()<<"click tick"<< endl;
    }
    void Chrono::onButton_Start(){
        timer_chrono -> start(1000);
        qDebug()<<"click start"<< endl;
    }
    void Chrono::onButton_Reset(){
        resetCount();
        m_lcd -> display(countTimer);
    }
    void Chrono::onBtnClicked(){
        //qDebug()<<"click"<< endl;
    }
