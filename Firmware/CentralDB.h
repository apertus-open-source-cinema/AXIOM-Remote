#ifndef CENTRALDB_H
#define CENTRALDB_H

#include <stdint.h>
//#include <vector>

class CentralDBObserver;

class CentralDB
{
    // std::vector<class CentralDBObserver*> _views;
    CentralDBObserver* _views[16];
    uint8_t _viewscount = 0;
    uint8_t _lcdBrightnessPercentage;

  public:
    CentralDB() : _views()
    {
        for (uint8_t i = 0; i < 16; i++)
        {
            _views[i] = nullptr;
        }
    }

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
    explicit CentralDBObserver(CentralDB* db) : _db(db)
    {
    }

    virtual void update() = 0;

  protected:
    CentralDB* GetDB()
    {
        return _db;
    }
};

class TestObs : public CentralDBObserver
{
    uint8_t _value = 0;

  public:
    explicit TestObs(CentralDB* db) : CentralDBObserver(db)
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
};

#endif /* CENTRALDB_H */
