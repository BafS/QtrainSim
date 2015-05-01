#include "ctrain_handler.h"
#include "locomotive.h"
#include "locomotiveworker.h"

#include <QList>
#include <QThread>
#include <QObject>
#include <QMap>

//Arret d'urgence
void emergency_stop()
{
    afficher_message("\nSTOP!");
}

// Lance un nouveau thread pour une locomotive et son parcours
void addLocomotiveThread(Locomotive * locomotive, QList<int>& parcours, QMap<QString,QPair<int,int> >& aiguillages)
{
    // https://mayaposch.wordpress.com/2011/11/01/how-to-really-truly-use-qthreads-the-full-explanation/
    QThread* thread = new QThread;
    LocomotiveWorker* lw1 = new LocomotiveWorker(locomotive, parcours, aiguillages);
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
    parcours << 15 << 14 << 7 << 6 << 5 << 34 << 33 << 32 << 25 << 24 << 23 << 16;

   /*
    * Choix des aiguillages avec une QMap:
    * Key type:QString: noContact1 - noContact2 (trançon sur le quel l'aguillage doit être modifié)
    * Value type QPair<int,int>: <no de l'aiguillage, Valeur de l'aiguillage>
    */
    QMap<QString, QPair<int,int> > aiguillages;
    aiguillages["23-24"] = QPair<int,int>(14,DEVIE);
    aiguillages["33-34"] = QPair<int,int>(21,DEVIE);


    //Initialisation de la locomotive
    Locomotive * locomotive = new Locomotive;
    locomotive->fixerNumero(1);
    locomotive->fixerVitesse(8);
    locomotive->fixerPosition(16, 23);

    // Création d'un thread pour la locomotive
    addLocomotiveThread(locomotive, parcours,aiguillages);
}


void runLocomotive2()
{
    QList<int> parcours;
    parcours << 12 << 11 << 10 << 4 << 3 << 2 << 1 << 31 << 33 << 32 << 24 << 19 << 13 ;

    /*
     * Choix des aiguillages avec une QMap:
     * Key type:QString: noContact1 - noContact2 (trançon sur le quel l'aguillage doit être modifié)
     * Value type QPair<int,int>: <no de l'aiguillage, Valeur de l'aiguillage>
     */
    QMap<QString, QPair<int,int> > aiguillages;
    aiguillages["19-24"] = QPair<int,int>(14,TOUT_DROIT);
    aiguillages["31-33"] = QPair<int,int>(21,TOUT_DROIT);
    aiguillages["1-2"] = QPair<int,int>(1,TOUT_DROIT);

    //Initialisation de la locomotive
    Locomotive * locomotive = new Locomotive;
    locomotive->fixerNumero(2);
    locomotive->fixerVitesse(8);
    locomotive->fixerPosition(12, 13);

    // Création d'un thread pour la locomotive
    addLocomotiveThread(locomotive, parcours, aiguillages);
}


//Fonction principale
int cmain()
{
    afficher_message("Hit play to start the simulation...");

    //Choix de la maquette
    selection_maquette(MAQUETTE_A);

    //Initialisation des aiguillages
    diriger_aiguillage(8,  DEVIE,       0);
    diriger_aiguillage(2,  DEVIE,       0);
    diriger_aiguillage(20, DEVIE,       0);
    diriger_aiguillage(5,  DEVIE,       0);
    diriger_aiguillage(4,  TOUT_DROIT,  0);
    diriger_aiguillage(10, TOUT_DROIT,  0);
    diriger_aiguillage(14, DEVIE,       0);
    diriger_aiguillage(11, TOUT_DROIT,  0);
    diriger_aiguillage(7, TOUT_DROIT,  0);
    diriger_aiguillage(17, TOUT_DROIT,  0);
    diriger_aiguillage(23, TOUT_DROIT,  0);
    diriger_aiguillage(5,  TOUT_DROIT,  0);

    // Thread Loco 1
    runLocomotive1();
    runLocomotive2();

    //Fin de la simulation
    mettre_maquette_hors_service();

    //Exemple de commande
    afficher_message("Enter a command in the input field at the top of the window.");
    QString commande = getCommand();
    afficher_message(qPrintable(QString("Your command is: ") + commande));

    return EXIT_SUCCESS;
}
