#include "locomotiveworker.h"

LocomotiveWorker::LocomotiveWorker(Locomotive &parent, QList<int>& p)
{
    locomotive = parent;
    parcours = p;
}

LocomotiveWorker::~LocomotiveWorker()
{

}

void LocomotiveWorker::process() {
    int nbTours = 2;
    forever {
        locomotive.allumerPhares();
        locomotive.demarrer();
        locomotive.afficherMessage("Ready!");

        for(int j = 0; j < nbTours; ++j) {
            //Attente du passage sur les contacts
            for (int i = 0; i < parcours.size(); i++) {
                attendre_contact(parcours.at(i));
                afficher_message(qPrintable(QString("The engine no. %1 has reached contact no. %2.").arg(locomotive.numero()).arg(parcours.at(i))));
                locomotive.afficherMessage(QString("I've reached contact no. %1.").arg(parcours.at(i)));
            }
        }        

        // Reverse parcours
        QList<int> parcoursRevers;
        for(int a = 0; a < parcours.length(); ++a) {
            parcoursRevers.push_front(parcours.at(a));
        }
        parcours = parcoursRevers;


//        qDebug() << parcoursRevers << "   -   " << parcours;

        //Arreter la locomotive
        locomotive.arreter();
        locomotive.afficherMessage("Yeah, piece of cake!");
        locomotive.inverser();

//        QList<T> result;
//        result.reserve( parcours.size() );
//        std::reverse_copy( parcours.begin(), parcours.end(), std::back_inserter( parcours ) );

        emit finished();
    }
}
