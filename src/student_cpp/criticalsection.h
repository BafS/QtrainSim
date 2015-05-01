#ifndef CRITICALSECTION_H
#define CRITICALSECTION_H

#include <QSemaphore>
#include <QString>
#include <QDebug>
#include "locomotive.h"

class CriticalSection
{
    int noContact1;
    int noContact2;
    bool accessing;

    // File d'attente
    QList<Locomotive*> requestsList;

    QSemaphore* waiting;
    QSemaphore* mutex;

public:

    CriticalSection(int noContact1, int noContact2){
        this->noContact1 = noContact1;
        this->noContact2 = noContact2;
        accessing = false;

        waiting = new QSemaphore(0);
        mutex = new QSemaphore(1);
    }

    ~CriticalSection(){
        delete waiting;
        delete mutex;
    }

    /**
     * Ajout d'une loco dans la file d'attente
     * @brief accessQuery
     * @param loco
     */
    void accessQuery(Locomotive* loco){
        requestsList.append(loco);
    }

    /**
     * @brief tryLock
     * @param loco
     */
    void tryLock(Locomotive* loco){

        mutex->acquire();

        int maxNumber = loco->numero();

        for(Locomotive* l : requestsList){
            maxNumber = maxNumber < l->numero() ? l->numero(): maxNumber;
        }

        if(accessing && maxNumber > loco->numero()){
            mutex->release();
            loco->arreter();
            waiting->acquire();
            requestsList.clear();
            if(!loco->isRunning()){
                loco->demarrer();
            }

        } else {
            accessing = true;
            mutex->release();
        }
    }

    /**
     *
     * @brief unlock
     */
    void unlock(){
        mutex->acquire();
        waiting->release();
        accessing = false;
        mutex->release();
    }

    bool equals(int noContact1, int noContact2){
        return this->noContact1 == noContact1 && this->noContact2 == noContact2;
    }

    bool isAccessing(){
        return accessing;
    }

};

#endif // CRITICALSECTION_H
