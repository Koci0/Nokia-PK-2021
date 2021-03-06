#pragma once

#include "Sms.hpp"
#include "ISmsDb.hpp"
#include <vector>

namespace ue
{

class SmsDb : public ISmsDb
{
private:
    std::vector<Sms> v_sms;

public:
    SmsDb() = default;
    explicit SmsDb(const std::vector<Sms> &);

    std::vector<Sms> getAll() override;
    Sms* getOne(int id) override;
    void addOne(Sms sms) override;
    Sms* getLast() override;
};

}
