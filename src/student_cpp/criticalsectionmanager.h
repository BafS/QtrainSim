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

    static CriticalSection*  getCS(int noContact1, int noContact2){

        for(CriticalSection* cs: *csList){
            if(cs->equals(noContact1,noContact2)){
                return cs;
            }
        }

        CriticalSection* newCs = new CriticalSection(noContact1, noContact2);
        csList->append(newCs);

        return newCs;
    }

    ~CriticalSectionManager(){
        delete csList;
    }
};


#endif // CRITICALSECTIONMANAGER_H
