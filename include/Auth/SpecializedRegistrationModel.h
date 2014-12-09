///* 
// * File:   RegistrationModelAlert.h
// * Author: tsa
// *
// * Created on 17 janvier 2013, 17:36
// */
//
//#ifndef REGISTRATIONMODELALERT_H
//#define	REGISTRATIONMODELALERT_H
//
//#include "tools/MainIncludeFile.h"
//#include <Wt/Auth/RegistrationModel>
//#include <Wt/Auth/AuthService>
//#include <Wt/Auth/AbstractPasswordService>
//#include <boost/random.hpp>
//#include <boost/random/random_device.hpp>
//
//class SpecializedRegistrationModel : public Wt::Auth::RegistrationModel {
//public:
//    SpecializedRegistrationModel(const Wt::Auth::AuthService& baseAuth,
//                                     Wt::Auth::AbstractUserDatabase& users,
//                                     Wt::Auth::Login& login,
//                                     Wt::WObject *parent);
//    virtual ~SpecializedRegistrationModel();
//    
//    enum RegistrationType {
//                Company, 
//                Individual, 
//                Association 
//            };
//    
//    
//    std::string generateToken();
//    
//    void setRegistrationType(RegistrationType rType);
//
//    
//private:
//    
//    RegistrationType rType;
//};
//
//#endif	/* REGISTRATIONMODELALERT_H */
//
