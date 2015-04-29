#ifndef CRITICALSECTION_H
#define CRITICALSECTION_H

#include <QSemaphore>

class CriticalSection
{
    int noContact1;
    int noContact2;
    bool accessing;

    QSemaphore* wait;
    QSemaphore* mutex;

public:

    CriticalSection(int noContact1, int noContact2){
        this->noContact1 = noContact1;
        this->noContact2 = noContact2;

        wait = new QSemaphore(0);
        mutex = new QSemaphore(1);
    }

    ~CriticalSection(){
        delete wait;
        delete mutex;
    }

    bool tryLock(){

        mutex->acquire();

        while(accessing){
            // try to access
            mutex->release();
            wait->acquire();
            mutex->acquire();

        }
        // is accessible
        accessing = true;
        mutex->release();
    }


    void unlock(){
        mutex->acquire();
        wait->release();
        accessing = false;
        mutex->release();
    }

    bool equals(int noContact1, int noContact2){
        return this->noContact1 == noContact1 && this->noContact2 == noContact2;
    }

};

#endif // CRITICALSECTION_H
