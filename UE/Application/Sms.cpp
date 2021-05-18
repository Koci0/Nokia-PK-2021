#include "Sms.hpp"


namespace ue
{

Sms::Sms(common::PhoneNumber from,common::PhoneNumber to, std::string text)
    : from(from),
      to(to),
      text(text)
{
    read = false;
}

}
