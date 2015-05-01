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

    void accessQuery(Locomotive* loco){
        requestsList.append(loco);
    }

    void tryLock(Locomotive* loco){

        qDebug() << QString("tryLock [%1 - %2] by %3").arg(noContact1).arg(noContact2).arg(loco->numero());


        mutex->acquire();

        int maxNumber = loco->numero();

        for(Locomotive* l : requestsList){
            maxNumber = maxNumber < l->numero() ? l->numero(): maxNumber;
        }

        if(requestsList.size() == 2){
            qDebug() << "WE ARE TWO " << maxNumber << " is prioritaire ";
        }

        if(accessing && maxNumber > loco->numero()){
            // try to access
            mutex->release();
            loco->arreter();
//            qDebug() << QString(" %3 ========= JE PEUX PAS PASSER  [%1 - %2] ===========").arg(noContact1).arg(noContact2).arg(loco->numero());
            waiting->acquire();

            if(!loco->isRunning()){
//                qDebug() << QString(" %1 ========= REDEMMARAGE ===========").arg(loco->numero());
                loco->demarrer();
            }

        } else {
            // is accessible
            accessing = true;
            mutex->release();
        }

    }

    /*bool tryLock(Locomotive* loco){

        mutex->acquire();

        if(accessing){
            // try to access
            mutex->release();
            loco->arreter();
            qDebug() << QString(" %3 ========= JE PEUX PAS PASSER  [%1 - %2] ===========").arg(noContact1).arg(noContact2).arg(loco->numero());
            wait->acquire();
            return false;
        }
        // is accessible
        accessing = true;
        mutex->release();
        qDebug() << QString("[%1 - %2] locked").arg(noContact1).arg(noContact2);
        if(!loco->isRunning()){
            loco->demarrer();
        }

        return true;
    }*/

    void unlock(){
        mutex->acquire();
        waiting->release();
        accessing = false;
        mutex->release();
        qDebug() << QString("[%1 - %2] unlocked").arg(noContact1).arg(noContact2);
    }

    bool equals(int noContact1, int noContact2){
        return this->noContact1 == noContact1 && this->noContact2 == noContact2;
    }

    bool isAccessing(){
        return accessing;
    }

};

#endif // CRITICALSECTION_H
