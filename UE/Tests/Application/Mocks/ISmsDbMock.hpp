#pragma once

#include <gmock/gmock.h>
#include "Ports/ISmsDb.hpp"
#include "Ports/Sms.hpp"

namespace ue
{

class Sms;

class ISmsDbMock : public ISmsDb
{
public:
    ISmsDbMock();
    ~ISmsDbMock() override;

    MOCK_METHOD(std::vector<Sms>, getAll, (), (final));
    MOCK_METHOD(Sms*, getOne, (int), (final));
    MOCK_METHOD(void, addOne, (Sms), (final));
    MOCK_METHOD(void, remove, (), (final));
};

}
