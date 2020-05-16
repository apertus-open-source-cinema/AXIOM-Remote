#ifndef CENTRALDB_H
#define CENTRALDB_H

#include <stdint.h>
//#include <vector>

class CentralDB
{
    // std::vector<class CentralDBObserver*> _views;
    class CentralDBObserver* _views[16];
    uint8_t _viewscount = 0;
    uint8_t _lcdBrightnessPercentage;

  public:
    void attach(CentralDBObserver* obs)
    {
        //_views.push_back(obs);
        _views[_viewscount++] = obs;
    }

    void SetLCDBrightness(uint8_t value)
    {
        _lcdBrightnessPercentage = value;
        Notify();
    }

    uint8_t GetLCDBrightness()
    {
        return _lcdBrightnessPercentage;
    }

    void Notify();
};

class CentralDBObserver
{
    CentralDB* _db;

  public:
    CentralDBObserver(CentralDB* db)
    {
        _db = db;
        _db->attach(this);
    }

    virtual void update() = 0;

  protected:
    CentralDB* GetDB()
    {
        return _db;
    }
};

void CentralDB::Notify()
{
    for (uint8_t i = 0; i < _viewscount; i++)
    {
        _views[i]->update();
    }
}

/*class TestObs : public CentralDBObserver
{
    uint8_t _value;

  public:
    TestObs(CentralDB* db) : CentralDBObserver(db)
    {
    }
    void update()
    {
        _value = GetDB()->GetLCDBrightness();
    }
    uint8_t getvalue()
    {
        return _value;
    }
};*/

#endif /* CENTRALDB_H */