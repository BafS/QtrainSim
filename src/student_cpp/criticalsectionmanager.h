#ifndef CRITICALSECTIONMANAGER_H
#define CRITICALSECTIONMANAGER_H

#include "criticalsection.h"
#include <QList>


class CriticalSectionManager
{
    static QList<CriticalSection* >* csList;

public:

    CriticalSectionManager(){

    }

    static CriticalSection*  request(Locomotive* loco,int noContact1, int noContact2){

        for(CriticalSection* cs: *csList){
            if(cs->equals(noContact1,noContact2)){
                cs->accessQuery(loco);
                return cs;
            }
        }

        CriticalSection* newCs = new CriticalSection(noContact1, noContact2);
        csList->append(newCs);
        newCs->accessQuery(loco);
        return newCs;
    }

    ~CriticalSectionManager(){
        delete csList;
    }
};


#endif // CRITICALSECTIONMANAGER_H
