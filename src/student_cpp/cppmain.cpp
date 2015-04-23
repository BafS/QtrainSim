#include "ctrain_handler.h"
#include "locomotive.h"
#include "locomotiveworker.h"

#include <QList>
#include <QThread>
#include <QObject>

//Creation d'une locomotive
static Locomotive locomotive;

//Arret d'urgence
void emergency_stop()
{
    locomotive.arreter();
    afficher_message("\nSTOP!");
}


// Lance un nouveau thread pour une locomotive et son parcours
void addLocomotiveThread(Locomotive& locomotive, QList<int>& parcours)
{
    // https://mayaposch.wordpress.com/2011/11/01/how-to-really-truly-use-qthreads-the-full-explanation/
    QThread* thread = new QThread;
    LocomotiveWorker* lw1 = new LocomotiveWorker(locomotive, parcours);
    lw1->moveToThread(thread);

    QObject::connect(thread, SIGNAL(started()), lw1, SLOT(process()));
    QObject::connect(lw1, SIGNAL(finished()), thread, SLOT(quit()));
    QObject::connect(lw1, SIGNAL(finished()), lw1, SLOT(deleteLater()));
    QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}


void runLocomotive1()
{
    //Initialisation d'un parcours
    QList<int> parcours;
    parcours << 7 << 15 << 14 << 7 << 6 << 5 << 34 << 33 << 32 << 25 << 24;
//    parcours << 7 << 15 << 14 << 7 << 6 << 5 << 34 << 33 << 32 << 25 << 24 << 23 << 16;
//    parcours << 26 << 25 << 1 << 19 << 20 << 8 << 7 << 13;
//    parcours << 2 << 7 << 15 << 18 << 23 << 22 << 1 << 2;

    //Initialisation des aiguillages
    diriger_aiguillage(8,  DEVIE,       0);
    diriger_aiguillage(2,  DEVIE,       0);
    diriger_aiguillage(20, DEVIE,       0);
    diriger_aiguillage(14, DEVIE,       0);
    diriger_aiguillage(11, TOUT_DROIT,  0);
    diriger_aiguillage(17, TOUT_DROIT,  0);
    diriger_aiguillage(23, TOUT_DROIT,  0);


    //Initialisation de la locomotive
    locomotive.fixerNumero(1);
    locomotive.fixerVitesse(25);
    locomotive.fixerPosition(16, 23);
//    locomotive.allumerPhares();
//    locomotive.demarrer();
//    locomotive.afficherMessage("Ready!");


    diriger_aiguillage(5, TOUT_DROIT,       0);

    addLocomotiveThread(locomotive, parcours);

    // TODO : Inverser le parcours
}


void runLocomotive2()
{

}


//Fonction principale
int cmain()
{

    afficher_message("Hit play to start the simulation...");

    //Choix de la maquette
    selection_maquette(MAQUETTE_A);

    // Thread Loco 1
    runLocomotive1();

    //Fin de la simulation
    mettre_maquette_hors_service();

    //Exemple de commande
    afficher_message("Enter a command in the input field at the top of the window.");
    QString commande = getCommand();
    afficher_message(qPrintable(QString("Your command is: ") + commande));

    return EXIT_SUCCESS;
}
