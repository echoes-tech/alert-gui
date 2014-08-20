///* 
// * File:   RegistrationModelAlert.cpp
// * Author: tsa
// * 
// * Created on 17 janvier 2013, 17:36
// */
//
//#include "Auth/SpecializedRegistrationWidget.h"
//#include <Wt/WFormModel>
//
//SpecializedRegistrationModel::SpecializedRegistrationModel(const Wt::Auth::AuthService& baseAuth,
//                                     Wt::Auth::AbstractUserDatabase& users,
//                                     Wt::Auth::Login& login,
//                                     Wt::WObject *parent)
//: Wt::Auth::RegistrationModel(baseAuth, users, login, parent)
//{
//    reset();
//}
//
//SpecializedRegistrationModel::~SpecializedRegistrationModel() {
//}
//
//std::string SpecializedRegistrationModel::generateToken()
//{
//    std::string res = "";
//    std::string chars(
//        "abcdefghijklmnopqrstuvwxyz"
//        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
//        "1234567890"
////        "!@#$%^&*()"
////        "`~-_=+[{]{\\|;:'\",<.>/? "
//    );
//    boost::random::random_device rng;
//    boost::random::uniform_int_distribution<> index_dist(0, chars.size() - 1);
//    for(int i = 0; i < 25; ++i) 
//    {
//        res += chars[index_dist(rng)];
//    }
//    return res;
//}  
//
//void SpecializedRegistrationModel::setRegistrationType(SpecializedRegistrationModel::RegistrationType rType)
//{
//    this->rType = rType;
//}
