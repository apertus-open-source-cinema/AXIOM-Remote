#ifndef CENTRALDBOBSERVER_H
#define CENTRALDBOBSERVER_H

#include "Attribute.h"
#include "CentralDB.h"

class CentralDB;

class CentralDBObserver
{
  private:
    const Attribute::ID _observedAttributeId;
    void (*_callback)(const CentralDB*);

  public:
    explicit CentralDBObserver(const Attribute::ID attribureId, void (*callback)(const CentralDB*));
    Attribute::ID ObservedAttribute() const;
    void Update(const CentralDB* db) const;
};

#endif // CENTRALDBOBSERVER_H
