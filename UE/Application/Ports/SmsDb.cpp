#include "SmsDb.hpp"
#include "Sms.hpp"

#include <vector>

namespace ue
{
SmsDb::SmsDb(){
    std::vector<Sms> vec;
    std::string s1 = "Alamakota";
    this->v_sms = vec;
    struct common::PhoneNumber number;
    v_sms.push_back(Sms(number,s1));
    //v_sms.push_back(new Sms('10','Ala'));
}
SmsDb::SmsDb(std::vector<Sms> v_sms){
    this->v_sms = v_sms;
}


    std::vector<Sms> SmsDb::getAll(){
        return v_sms;
    }
    Sms* SmsDb::getOne(int id){

        return &v_sms.at(id);
    }
    void SmsDb::addOne(Sms sms){

        v_sms.push_back(sms);
    }

    void SmsDb::remove(){
        v_sms.clear();
    }


}
