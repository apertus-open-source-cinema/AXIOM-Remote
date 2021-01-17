#ifndef CENTRALDBOBSERVER_H
#define CENTRALDBOBSERVER_H

#include "Attribute.h"

class CentralDB;

class CentralDBObserver
{
    const Attribute::ID _observedAttributeID;    
    void (*_callback)(const CentralDB&);

  public:
    CentralDBObserver(Attribute::ID attributeID, void (*callback)(const CentralDB&));
    Attribute::ID ObservedAttribute() const;
    void Update(const CentralDB& db) const;
};

#endif // CENTRALDBOBSERVER_H
