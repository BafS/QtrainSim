#include "criticalsectionmanager.h"


QList<CriticalSection*> * CriticalSectionManager::csList = new QList<CriticalSection*>;


CriticalSectionManager::CriticalSectionManager()
{

}

/**
 * @brief CriticalSectionManager::request
 * @param loco : Locomotive effetuant la requête
 * @param noContact1: Contact1 du trançon
 * @param noContact2: Contact2 du trançon
 * @return Un pointeur sur la section critique
 */
CriticalSection *CriticalSectionManager::request(Locomotive *loco, int noContact1, int noContact2)
{
    // Recherche de la section critique correspondant au numéros de contacts
    for(CriticalSection* cs: *csList){
        if(cs->equals(noContact1,noContact2)){

            // accessQuery Ajoute la loco dans la file d'attente de la section critique
            // cela permettra au moment voulut de les départager selon leur priorités
            cs->accessQuery(loco);
            return cs;
        }
    }

    // Si la section critique n'est pas trouvée, elle est crée.
    // Evite de devoir déclarer toutes les section critiques explicitement
    CriticalSection* newCs = new CriticalSection(noContact1, noContact2);
    csList->append(newCs);
    newCs->accessQuery(loco);
    return newCs;
}

CriticalSectionManager::~CriticalSectionManager()
{
    delete csList;
}
