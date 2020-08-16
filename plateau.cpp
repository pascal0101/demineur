#include "plateau.h"
#include "chrono.h"
#include <QWidget>
#include <QMenu>
#include <QAction>
#include <QVBoxLayout>
#include <QLabel>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QMessageBox>

Plateau::Plateau(QWidget *parent) : hauteur(20), largeur(20), bombes(50), grille(hauteur, QVector<Case*>(largeur, new Case(0, 0)) ){

    //Creation de la barre de menu

    QMenu *menuJeu = menuBar()->addMenu("Parametres");

        QAction *actionRelancer = menuJeu->addAction("Relancer");

        connect(actionRelancer, SIGNAL(triggered()), this, SLOT(nouvellePartie()));

        //QAction *actionSet = menuJeu->addAction("Parametres de partie");

        //connect(actionSet, SIGNAL(triggered()), this, SLOT(changerParametres()));

        //QAction *actionInit = menuJeu->addAction("Initialiser la map");

        //connect(actionInit, SIGNAL(triggered()), this, SLOT(initialiser()));

        QAction *actionAbandon = menuJeu->addAction("Abandonner");

        connect(actionAbandon, SIGNAL(triggered()), this, SLOT(reveler()));

        QAction *actionQuitter = menuJeu->addAction("Quitter");

        connect(actionQuitter, SIGNAL(triggered()), qApp, SLOT(quit()));


        menuBar()->addAction("Infos", this, SLOT(informations()));
         menuBar()->addAction("Choisir un niveau", this, SLOT(changerParametres()));



        nouvellePartie();
        resize(hauteur*30, largeur*30);

        connect(this, SIGNAL(winner()), this, SLOT(gagne()));
        connect(this, SIGNAL(winner()), this, SLOT(reveler()));

        connect(this, SIGNAL(loser(int,int)), this, SLOT(perd(int,int)));
}
//destructeur
Plateau::~Plateau(){

}



int Plateau::nbBombesTotal() const{
    int compteur(0);

    for(int i = 0 ; i < hauteur ; i++){
        for(int j = 0 ; j < largeur ; j++){
            compteur += grille[i][j]->isBombe();
        }
    }

    return compteur;
}

int Plateau::nbBombes(int x, int y) const{

    int compteur(0);
    for(int i = x-1 ; i <= x+1 ; i++){
        for(int j = y-1 ; j <= y+1 ; j++){
            if(i >= 0 && j >= 0 && i <= hauteur-1 && j <= largeur-1){
                compteur += grille[i][j]->isBombe();
            }
        }
    }

    return compteur;
}

int Plateau::nbFlags() const{

    int compteur(0);

    for(int i = 0 ; i < hauteur ; i++){
        for(int j = 0 ; j < largeur ; j++){
            compteur += grille[i][j]->hasFlag();
        }
    }

    return compteur;
}

int Plateau::nbRestant() const{
    return nbBombesTotal() - nbFlags();
}

bool Plateau::win() const{

    for(int i = 0 ; i < hauteur ; i++){
        for(int j = 0 ; j < largeur ; j++){
            if(grille[i][j]->isBombe() && !grille[i][j]->hasFlag()){
                return false;
            }
            if(grille[i][j]->hasFlag() && !grille[i][j]->isBombe()){
                return false;
            }
            if(debut){
                return false;
            }
        }
    }
    return true;
}

QString Plateau::couleur(int nombre) const{

    QString texte;
    switch (nombre){
        case 1:
            texte  = "<strong><font color=\"#0000ff\">";//Bleu
            break;
        case 2:
            texte  = "<strong><font color=\"#008000\">";//Dark green
            break;
        case 3:
            texte  = "<strong><font color=\"#ff0000\">";//Red
            break;
        case 4:
            texte  = "<strong><font color=\"#000080\">";//Dark blue
            break;
        case 5:
            texte  = "<strong><font color=\"#800000\">";//Dark red
            break;
        default:
            texte  = "<strong><font color=\"#800080\">";//Dark magenta
            break;
    }
    texte += nombre + 48;
    texte += "</font></strong>";
    return texte;
}


void Plateau::devoiler(int x, int y){

    //En quelque sorte la fonction principale de la classe elle reagis lors du clic sur une case
    //Elle prend en parametre les coordonnes

    if(debut){
        //Si la partie n'a pas encore ete initialisee, il genere la map
        //La map sera genere en fonction de la case cliquee afin de creer un zone vide a l'edroit du clic
        debut = false;
        do{
            for(int i = 0 ; i < hauteur ; i++){
                for(int j = 0 ; j < largeur ; j++){
                    grille[i][j]->setBombe(false);
                }
            }
            if(grille[x][y]->isVisible()){
                initialiser(bombes, x, y);
            }
        }while(nbBombes(x, y) > 0);
        devoiler(x, y);
    }

    if(!grille[x][y]->hasFlag()){//Empeche de cliquer sur une case marquee
        if(grille[x][y]->isBombe()){
            //Emmet un signal si la case cliquee etait une bombe (perdu)
            //Ce signal emmet egalement les coordonnes du clic
            emit loser(x, y);
        }

        else if(nbBombes(x, y) == 0){
            //Si la case cliquee est vide, devoile toutes les cases autours (c'est une fonction recursive :p)
            grille[x][y]->hide();
            QLabel *texte = new QLabel;
            layoutGrille->addWidget(texte, x, y);

            for(int i = x-1 ; i <= x+1 ; i++){
                for(int j = y-1 ; j <= y+1 ; j++){
                    if(i >= 0 && j >= 0 && i < hauteur && j < largeur && grille[i][j]->isVisible()){
                        devoiler(i, j);
                    }
                }
            }
        }

        else{
            //Sinon (pas un bombes et des bombes proches)
            //Affiche le nombre de bombes broches

            grille[x][y]->hide();
            QLabel *texte = new QLabel(couleur(nbBombes(x, y)));
            texte->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            layoutGrille->addWidget(texte, x, y);

        }
    }
}

void Plateau::reveler(int x, int y){

    //Cette fonction met la map en mode "fin de partie":
    //    -Empeche de cliquer sur les bouttons
    //    -Affiche ou etait les bombes non-trouves, les drapeaux bien places, mal places ...
    //Elle prend en parametre (optionnel) les coordonnes de la case qui a explose

    QPixmap icone(":/smiley/sad");
    recommencer->setIcon(QIcon(icone));

    for(int i = 0 ; i < hauteur ; i++){
        for(int j = 0 ; j < largeur ; j++){
            if(grille[i][j]->isVisible()){
                grille[i][j]->devoiler();
            }
        }
    }
    if(x >= 0 && y >=0){
        grille[x][y]->setIcon(QIcon(":/status/hitMine"));
    }
}

void Plateau::initialiser(int nombre, int x, int y){

    // Initialise les bombes sur la grille
    // Prend en parametre :
    //  -Le nombre de bombes a placer
    //  -(optionnel) Les coordonés d'une case ou ne pas mettre de bombe

    if(nombre == 0){
        nouvellePartie();
        nombre = bombes;
        debut = false;
    }
    int xx, yy;
    for(int i = 0 ; i < nombre ; i++){
        do{
            xx = rand() % hauteur;
            yy = rand() % largeur;
        }while(grille[xx][yy]->isBombe() && xx != x && yy != y);

        grille[xx][yy]->setBombe(true);
    }
}

void Plateau::nouvellePartie(){
    //onButton_Start();
    //Reinitialise la partie et le QWidget central
    debut = true;
    layoutGrille = new QGridLayout;
    layoutGrille->setSpacing(0);

    grille.resize(hauteur);
    for(int i = 0 ; i < hauteur ; i++){
        grille[i].resize(largeur);
        for(int j = 0 ; j < largeur ; j++){
            grille[i][j] = new Case(i, j);
            grille[i][j]->setStyleSheet("color: black; background-color: blue");
            layoutGrille->addWidget(grille[i][j], i, j);
            connect(grille[i][j], SIGNAL(Clic(int, int)), this, SLOT(devoiler(int, int)));
            connect(grille[i][j], SIGNAL(flagChanged()), this, SLOT(actualliserCompteur()));
        }
    }

    zoneCentrale = new QWidget;
    QVBoxLayout *layoutPrincipal = new QVBoxLayout;
    QHBoxLayout *layoutHaut = new QHBoxLayout;

        compteurReste = new QLCDNumber;
        compteurReste->setToolTip("Nombre restant de bombes a trouver (si tous les drapeaux sont bien places)");
        compteurReste->setFixedHeight(50);
        compteurReste->setStyleSheet("color: red");
        layoutHaut->addWidget(compteurReste);
        compteurReste->display(bombes);     

        recommencer = new QPushButton;
        recommencer->setToolTip("Cliquez sur le smiley pour relancer une partie");
        QPixmap icone(":images/sun.png");
            recommencer->setIcon(QIcon(icone));
            recommencer->setFixedSize(50, 50);
            recommencer->setIconSize(QSize(40, 40));
        layoutHaut->addWidget(recommencer);
        connect(recommencer, SIGNAL(pressed()), this, SLOT(nouvellePartie()));

        chrono = new QLCDNumber;
        chrono->setToolTip("chronometre");
        chrono->setFixedHeight(50);
        chrono->setStyleSheet("color: green");
        layoutHaut->addWidget(chrono);


        timer_chrono = new QTimer();                // Création du Compteur
        resetCount();



        connect( timer_chrono, SIGNAL(timeout()), this, SLOT(onTimer_Tick()));

        //connect( start, SIGNAL(clicked()), this, SLOT(onButton_Start()));

        layoutHaut->addWidget(new QWidget);

    layoutPrincipal->addLayout(layoutHaut);
    layoutPrincipal->addLayout(layoutGrille);
    zoneCentrale->setLayout(layoutPrincipal);

    setCentralWidget(zoneCentrale);
    onButton_Start();
}

void Plateau::actualliserCompteur(){

    //Actualise le compteur de bombes restant a trouver
    //Ce slot est utilise lors de la pose d'un drapeau et contient quelques element supplementaires

    if (compteurReste->intValue() > nbRestant()){
        //Si le drapeau a ete pose (pas supprime)

        if(tropFlagsMsg->isChecked() && nbRestant() == 0 && !win()){
     /*S'il y a autant de drapeaux que de bombes sur la carte et que ce n'est pas gagne, affiche un message d'alerte Est retirable via le menu*/

            QMessageBox::warning(this, "Attention", "Vous avez depose autant de drapeaux qu'il a ete cree de bombes\n"
                                                    "Vous avez surement pose des drapeaux au mauvais endroit");
        }
    }

    if(win()){
        //Emet un signal si la partie est gagnee
        emit winner();
    }

    compteurReste->display(nbRestant());
}

void Plateau::changerParametres(){

    bool change(false);

    Parametre *fenetre = new Parametre(this, &hauteur, &largeur, &bombes, &change);
    fenetre->exec();
    if(change){
        nouvellePartie();
        resize(hauteur*30, largeur*30);
    }
}


void Plateau::informations(){

    QMessageBox *message = new QMessageBox(this);
    message->setWindowTitle("Informations sur le jeu");
    message->setText(QString("Cet jeu a ete realise par deux étudiants"));
    message->show();
}

void Plateau::gagne(){
    QMessageBox *fenetre = new QMessageBox(this);
    fenetre->setWindowTitle("Vous avez gagnez");
    fenetre->setIconPixmap(QPixmap(":/imageFin/gagne"));
    fenetre->show();
}

void Plateau::perd(int x, int y){
    reveler(x, y);
    QMessageBox *perdu = new QMessageBox(this);
    perdu->setIconPixmap(QPixmap(":/imageFin/perd"));
    perdu->setWindowTitle("Vous avez perdu");

    perdu->show();
}

//fonction chronometre

    void Plateau::resetCount(){
        countTimer=0;
    }
    void Plateau::updateCount(){
        countTimer++;
    }
    void Plateau::onTimer_Tick(){
        updateCount();
        chrono -> display(countTimer);
        qDebug()<<"click tick"<< endl;
    }
    void Plateau::onButton_Start(){
        timer_chrono -> start(1000);
        qDebug()<<"click start"<< endl;
    }
