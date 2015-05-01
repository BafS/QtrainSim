#include "locomotiveworker.h"

LocomotiveWorker::LocomotiveWorker(Locomotive* parent, QList<int>& p, QMap<QString,QPair<int,int> >& aiguillages)
{
    locomotive = parent;
    parcours = p;
    this->aiguillages = aiguillages;
}

LocomotiveWorker::~LocomotiveWorker()
{

}

void LocomotiveWorker::unlockCriticalSections(int currentPosition)
{

   int noContact1 = qMin(parcours.at(currentPosition), currentPosition -1 >= 0 ? parcours.at(currentPosition -1): -1);
   int noContact2 = qMax(parcours.at(currentPosition), currentPosition -1 >= 0 ? parcours.at(currentPosition -1): -1);

   if(noContact1 == -1 || noContact2 == -1)
       return;

   for(CriticalSection* cs: lockedSc){
       if(cs->equals(noContact1, noContact2)){
           cs->unlock();
           lockedSc.removeOne(cs);
       }
   }
}

void LocomotiveWorker::receiveResponse(int currentPosition)
{
    int noContact1 = (currentPosition + 2) < parcours.size() ? qMin(parcours.at(currentPosition+1), parcours.at(currentPosition+2)): -1;
    int noContact2 = (currentPosition + 2) < parcours.size() ? qMax(parcours.at(currentPosition+1), parcours.at(currentPosition+2)): -1;

    for(CriticalSection* cs : lockedSc){
        if(cs->equals(noContact1,noContact2)){
            cs->tryLock(locomotive);
        }
    }

    checkAiguillage(locomotive->numero(), noContact1, noContact2);
}

/**
 * @brief LocomotiveWorker::checkAiguillage
 * @param noLoco : no de la locomotive
 * @param noContact1: un des deux contact (valeur min)
 * @param noContact2: l'autre contact (valeur max)
 */
void LocomotiveWorker::checkAiguillage(int noLoco, int noContact1,int  noContact2)
{

    QString key = QString("%1-%2").arg(noContact1).arg(noContact2);

    if (aiguillages.contains(key)){
        QPair<int,int> infos = aiguillages[key];
        diriger_aiguillage (infos.first, infos.second, 0);
    }

}

void LocomotiveWorker::process() {
    qDebug() << parcours ;

    int nbTours = 2;
    forever {

        locomotive->allumerPhares();
        locomotive->demarrer();
        locomotive->afficherMessage("Ready!");

        for(int j = 0; j < nbTours; ++j) {

            //Attente du passage sur les contacts
            for (int i = 0; i < parcours.size(); i++) {
                attendre_contact(parcours.at(i));
                afficher_message(qPrintable(QString("The engine no. %1 has reached contact no. %2.").arg(locomotive->numero()).arg(parcours.at(i))));
                locomotive->afficherMessage(QString("I've reached contact no. %1.").arg(parcours.at(i)));

                unlockCriticalSections(i);

                /*
                 * Identification des numéros de contacts de la section critique
                 * Une section critique est définie par 2 contacts.
                 */
                int noContact1 = i+ NB_CONTACTS+1 < parcours.size() ? qMin(parcours.at(i+NB_CONTACTS), parcours.at(i+NB_CONTACTS+1)): -1;
                int noContact2 = i+ NB_CONTACTS+1 < parcours.size() ? qMax(parcours.at(i+NB_CONTACTS), parcours.at(i+NB_CONTACTS+1)): -1;

                locomotive->afficherMessage(QString("nextCritical Section: [%1 - %2]").arg(noContact1).arg(noContact2));

                // Si les numéro sont valide (à la fin du parcours, ils valent -1)
                if(noContact1 != -1 && noContact2 != -1){

                    /*
                     * Formulation d'une requête pour un tronçon
                     */
                    CriticalSection* nextCs = CriticalSectionManager::request(locomotive,noContact1,noContact2);
                    lockedSc.push_front(nextCs);

                    /*
                     * La réponse à la précédente requête est prise en compte lorsque la locomotive
                     * se situe un contact avant le trançon.
                     */
                    receiveResponse(i);
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
        locomotive->arreter();
        locomotive->afficherMessage("Yeah, piece of cake!");
        locomotive->inverser();
    }

    emit finished();
}
