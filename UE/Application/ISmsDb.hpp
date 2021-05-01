#pragma once

#include "Sms.hpp"
#include <vector>

namespace ue
{

class ISmsDb
{
public:
    virtual ~ISmsDb() = default;

    //wszystkie sms dla ue
    //jeden specyficzny sms
    //dodanie sms

    virtual std::vector<Sms> getAll()=0;
    virtual Sms* getOne(int id)=0;
    virtual void addOne(Sms sms)=0;
    virtual void remove()=0;

};



}
