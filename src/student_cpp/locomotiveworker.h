#ifndef LOCOMOTIVEWORKER_H
#define LOCOMOTIVEWORKER_H

#include <QThread>
#include <QObject>
#include "locomotive.h"
#include "ctrain_handler.h"

class LocomotiveWorker : public QObject
{
    Q_OBJECT

    Locomotive locomotive;
    QList<int> parcours;

public:
    explicit LocomotiveWorker(Locomotive& parent, QList<int> &p);
    ~LocomotiveWorker();

signals:

public slots:
    void process();
};

#endif // LOCOMOTIVEWORKER_H
