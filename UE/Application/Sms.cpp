#include "Sms.hpp"


namespace ue
{

Sms::Sms(common::PhoneNumber from, std::string text)
    : from(from),
      text(text)
{
    read = false;
}

}
