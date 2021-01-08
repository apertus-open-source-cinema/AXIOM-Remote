#ifndef CENTRALDBOBSERVER_H
#define CENTRALDBOBSERVER_H

#include "Attribute.h"
#include "CentralDB.h"

class CentralDB;

class CentralDBObserver
{
    const Attribute::Id _observedAttributeId;
    void (*_callback)(const CentralDB&);

  public:
    explicit CentralDBObserver(const Attribute::Id attribureId, void (*callback)(const CentralDB&));
    Attribute::Id ObservedAttribute() const;
    void Update(const CentralDB& db) const;
};

#endif // CENTRALDBOBSERVER_H
