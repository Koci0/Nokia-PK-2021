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

}
