#ifndef CASE_H
#define CASE_H

#include <QObject>
#include <QtGui>
#include <QPushButton>

class Case : public QPushButton
{
    Q_OBJECT
public:
    Case();
    Case(int x = 0, int y = 0);

        bool isBombe() const;
        void setBombe(bool a);

        bool hasFlag() const;

        void devoiler();

    signals:
        void Clic(int x, int y);
        void flagChanged();

    public slots:
        void emClic();
        void turnFlag();

    private:
        bool bombe;
        bool flag;

        int m_x;
        int m_y;

    protected:
        void mousePressEvent(QMouseEvent *event);
};

#endif // CASE_H
