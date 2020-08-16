#ifndef PARAMETRE_H
#define PARAMETRE_H

#include <QtGui>
#include <QDialog>
#include <QSpinBox>

class Parametre : public QDialog
{
       Q_OBJECT;
       public:
        Parametre(QWidget *parent, int *bhauteur, int *blargeur, int *bbombes, bool *changements);

        public slots:
            void valider();
            void actualiserChampBombes();

        private:
            int *hauteur;
            int *largeur;
            int *bombes;
            bool *changement;

            QSpinBox *champLargeur;
            QSpinBox *champHauteur;
            QSpinBox *champBombes;
};

#endif // PARAMETRE_H
