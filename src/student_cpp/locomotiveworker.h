#ifndef LOCOMOTIVEWORKER_H
#define LOCOMOTIVEWORKER_H

#include <QThread>
#include <QObject>
#include <QDebug>
#include "locomotive.h"
#include "ctrain_handler.h"
#include "criticalsection.h"
#include "criticalsectionmanager.h"

#define NB_CONTACTS 2

class LocomotiveWorker : public QObject
{
    Q_OBJECT
    QList<CriticalSection* > lockedSc;
    Locomotive* locomotive;
    QList<int> parcours;
    QMap<QString,QPair<int,int> > aiguillages;

public:
    explicit LocomotiveWorker(Locomotive * parent, QList<int> &p, QMap<QString,QPair<int,int> >& aiguillages);
    ~LocomotiveWorker();

    void unlockCriticalSections(int currentPosition);
    void receiveResponse(int currentPosition);
    void checkAiguillage(int noLoco,int noContact1, int noContact2);

signals:
    void finished();

public slots:
    void process();
};

#endif // LOCOMOTIVEWORKER_H
