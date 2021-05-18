#include "SmsDb.hpp"
#include "Sms.hpp"
#include <vector>

namespace ue
{
SmsDb::SmsDb()
{
    std::vector<Sms> vec;
    this->v_sms = vec;
}

SmsDb::SmsDb(std::vector<Sms> v_sms)
{
    this->v_sms = v_sms;
}

std::vector<Sms> SmsDb::getAll()
{
    return v_sms;
}

Sms* SmsDb::getOne(int id)
{
    return &v_sms.at(id);
}

void SmsDb::addOne(Sms sms)
{
    v_sms.push_back(sms);
}

void SmsDb::remove()
{
    v_sms.clear();
}

Sms* SmsDb::getLast()
{
    return &v_sms.back();
}

}
