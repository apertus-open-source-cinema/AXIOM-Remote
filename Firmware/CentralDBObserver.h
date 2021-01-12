#ifndef CENTRALDBOBSERVER_H
#define CENTRALDBOBSERVER_H

#include <functional>

#include "Attribute.h"
#include "CentralDB.h"

class CentralDB;

class CentralDBObserver
{
    Attribute::ID _observedAttributeId;
    std::function<void(const CentralDB&)> _callback;

  public:
    CentralDBObserver()
    {
    }
    CentralDBObserver(const Attribute::ID attribureID, std::function<void(const CentralDB&)> func);
    Attribute::ID ObservedAttribute() const;
    void Update(const CentralDB& db) const;
};

#endif // CENTRALDBOBSERVER_H
