#include <gtest/gtest.h>
#include "Messages/PhoneNumber.hpp"
#include "Sms.hpp"
#include "SmsDb.hpp"

namespace ue
{
using namespace ::testing;

class SmsDbTestSuite: public Test
{
public:
    static constexpr common::PhoneNumber number{220};
    static constexpr common::PhoneNumber from{123};
    SmsDb objTested;
};

TEST_F(SmsDbTestSuite, shallAddUnreadSms)
{
    const Sms sms{number, from, "test suite"};
    objTested.addOne(sms);
    const std::vector<Sms>& fetched = objTested.getAll();

    ASSERT_EQ(fetched[0].read, false);
    ASSERT_EQ(fetched[0].from, number);
}

TEST_F(SmsDbTestSuite, shallGetSms)
{
    const Sms sms{number, from, "test suite"};
    objTested.addOne(sms);
    const std::vector<Sms>& fetched = objTested.getAll();

    ASSERT_EQ(sms, fetched[0]);
}
}
