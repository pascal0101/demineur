#include "case.h"

Case::Case(int x, int y) : bombe(false), flag(false), m_x(x), m_y(y)
{
    QString h;
        h.setNum(x);
    QString l;
        l.setNum(y);
    setToolTip("[" + h + "]:[" + l + "]");
    setMinimumSize(20,20);
    setMaximumSize(100, 100);
    setBaseSize(50,50);

    connect(this, SIGNAL(clicked()), this, SLOT(emClic()));
}

bool Case::isBombe() const{
    return bombe;
}
void Case::setBombe(bool a){
    bombe = a;
}

void Case::emClic(){
    emit Clic(m_x, m_y);
}

void Case::turnFlag(){
    if(!signalsBlocked()){
        if(flag){
            setIcon(QIcon(""));
            flag = false;
        }
        else{
            setIcon(QIcon(":images/flag.png"));
            //setStyleSheet("color: green; background-color: green");
            flag = true;
        }
        emit flagChanged();
    }
}
bool Case::hasFlag() const{
    return flag;
}

void Case::mousePressEvent(QMouseEvent *event){

    if(event->button() == Qt::RightButton){
        turnFlag();
    }
    QPushButton::mousePressEvent(event);
}

void Case::devoiler(){
    if(bombe && !flag){
        setIcon(QIcon(":/status/mine"));
         //setStyleSheet("color: red; background-color: red");
    }
    else if(flag && bombe){
        setIcon(QIcon(":/status/checkMine"));
         //setStyleSheet("color: yellow; background-color: yellow");
    }
    blockSignals(true);
}
