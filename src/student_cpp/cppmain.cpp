#include "ctrain_handler.h"
#include "locomotive.h"

#include <QList>

//Creation d'une locomotive
static Locomotive locomotive;

//Arret d'urgence
void emergency_stop()
{
    locomotive.arreter();
    afficher_message("\nSTOP!");
}


void tLoco1() {

    //Initialisation d'un parcours
    QList<int> parcours;
    parcours << 7 << 15 << 14 << 7 << 6 << 5 << 34 << 33 << 32 << 25 << 24 << 23 << 16;
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


// TODO : Inverser le parcours

    int nbTours = 2;
    for(;;) {
        locomotive.allumerPhares();
        locomotive.demarrer();
        locomotive.afficherMessage("Ready!");

        for(int j = 0; j < nbTours; ++j) {
            //Attente du passage sur les contacts
            for (int i = 1; i < parcours.size(); i++) {
                attendre_contact(parcours.at(i));
                afficher_message(qPrintable(QString("The engine no. %1 has reached contact no. %2.").arg(locomotive.numero()).arg(parcours.at(i))));
                locomotive.afficherMessage(QString("I've reached contact no. %1.").arg(parcours.at(i)));
            }
        }

        //Arreter la locomotive
        locomotive.arreter();
        locomotive.afficherMessage("Yeah, piece of cake!");
        locomotive.inverser();

//        QList<T> result;
//        result.reserve( parcours.size() );
//        std::reverse_copy( parcours.begin(), parcours.end(), std::back_inserter( parcours ) );
    }


}

void tLoco2() {

}


//Fonction principale
int cmain()
{
    afficher_message("Hit play to start the simulation...");

    //Choix de la maquette
    selection_maquette(MAQUETTE_A);

    // Thread Loco 1
    tLoco1();

    //Fin de la simulation
    mettre_maquette_hors_service();

    //Exemple de commande
    afficher_message("Enter a command in the input field at the top of the window.");
    QString commande = getCommand();
    afficher_message(qPrintable(QString("Your command is: ") + commande));

    return EXIT_SUCCESS;
}
