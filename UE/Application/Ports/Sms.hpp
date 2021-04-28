#pragma once

#include <string>
#include "Messages/PhoneNumber.hpp"

namespace ue
{

using common::PhoneNumber;

class Sms
{
public:
    Sms(PhoneNumber from, std::string text);
    int messageId;
    std::string text;
    PhoneNumber from, to;
    bool read;

};

inline bool operator==(const Sms& left, const Sms& right)
{
    return left.from==right.from
            && left.messageId==right.messageId
            && left.text==right.text
            && left.to==right.to;
}

}
