#include "locomotiveworker.h"

LocomotiveWorker::LocomotiveWorker(Locomotive &parent, QList<int>& p)
{
    locomotive = parent;
    parcours = p;
}

LocomotiveWorker::~LocomotiveWorker()
{

}

bool LocomotiveWorker::request(CriticalSection c)
{

}

void LocomotiveWorker::unlockCriticalSections(int currentPosition)
{
   int noContact1 = qMin(parcours.at(currentPosition), currentPosition -1 > 0 ? parcours.at(currentPosition -1): -1);
   int noContact2 = qMax(parcours.at(currentPosition), currentPosition -1 > 0 ? parcours.at(currentPosition -1): -1);

   if(noContact1 == -1 || noContact2 == -1)
       return;

   for(CriticalSection* cs: lockedSc){
       if(cs->equals(noContact1, noContact2)){
           cs->unlock();
           lockedSc.removeOne(cs);
           qDebug() << "CS: [" << noContact1 << " - " << noContact2 << "] unlocked";
       }
   }
}

void LocomotiveWorker::process() {
    qDebug() << parcours ;

    int nbTours = 2;
    forever {
        locomotive.allumerPhares();
        locomotive.demarrer();
        locomotive.afficherMessage("Ready!");

        qDebug() << "balbal" << endl;
        for(int j = 0; j < nbTours; ++j) {
            //Attente du passage sur les contacts
            for (int i = 0; i < parcours.size(); i++) {

                attendre_contact(parcours.at(i));
                afficher_message(qPrintable(QString("The engine no. %1 has reached contact no. %2.").arg(locomotive.numero()).arg(parcours.at(i))));
                locomotive.afficherMessage(QString("I've reached contact no. %1.").arg(parcours.at(i)));

//                unlockCriticalSections(parcours.at(i)); // Bounce error

//                 Identification des numéros de contacts de la section critique
//                 Une section critique est définie par 2 contacts.
                int noContact1 = i+ NB_CONTACTS+1 < parcours.size() ? qMin(parcours.at(i+NB_CONTACTS), parcours.at(i+NB_CONTACTS+1)): -1;
                int noContact2 = i+ NB_CONTACTS+1 < parcours.size() ? qMax(parcours.at(i+NB_CONTACTS), parcours.at(i+NB_CONTACTS+1)): -1;

                locomotive.afficherMessage(QString("nextCritical Section: [%1 - %2]").arg(noContact1).arg(noContact2));

                if(noContact1 != -1 && noContact2 != -1){

//                    // Le Manager des sections critiques nous fournis un pointeur sur la section critique
                    CriticalSection* nextCs = CriticalSectionManager::getCS(noContact1,noContact2);

//                    // vérification si la section est disponible
//                    nextCs->tryLock();
//                    lockedSc.push_front(nextCs);
//                    locomotive.afficherMessage(QString("CS: [%1 - %2] locked" ).arg(noContact1).arg(noContact2));
                }

            }
        }        

        // Reverse parcours
        QList<int> parcoursRevers;
        for(int a = 0; a < parcours.length(); ++a) {
            parcoursRevers.push_front(parcours.at(a));
        }
        parcours = parcoursRevers;


        //Arreter la locomotive
        locomotive.arreter();
        locomotive.afficherMessage("Yeah, piece of cake!");
        locomotive.inverser();

//        QList<T> result;
//        result.reserve( parcours.size() );
//        std::reverse_copy( parcours.begin(), parcours.end(), std::back_inserter( parcours ) );

    }
    emit finished();
}
