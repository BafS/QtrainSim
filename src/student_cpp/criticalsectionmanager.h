#ifndef CRITICALSECTIONMANAGER_H
#define CRITICALSECTIONMANAGER_H

#include "criticalsection.h"
#include <QList>


class CriticalSectionManager
{
    // List des sections Critiques
    static QList<CriticalSection* >* csList;

public:
    CriticalSectionManager();

    // Requête d'une locomotive pour une section critique
    static CriticalSection*  request(Locomotive* loco,int noContact1, int noContact2);
    ~CriticalSectionManager();
};


#endif // CRITICALSECTIONMANAGER_H
