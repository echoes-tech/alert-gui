/* 
 * File:   RegistrationModelAlert.h
 * Author: tsa
 *
 * Created on 17 janvier 2013, 17:36
 */

#ifndef REGISTRATIONMODELALERT_H
#define	REGISTRATIONMODELALERT_H

#include "tools/MainIncludeFile.h"
#include <Wt/Auth/RegistrationModel>
#include <Wt/Auth/AuthService>
#include <Wt/Auth/AbstractPasswordService>
#include <boost/random.hpp>
#include <boost/random/random_device.hpp>

class RegistrationModelAlert : public Wt::Auth::RegistrationModel {
public:
    RegistrationModelAlert(const Wt::Auth::AuthService& baseAuth,
                                     Wt::Auth::AbstractUserDatabase& users,
                                     Wt::Auth::Login& login,
                                     Wt::WObject *parent);
    virtual ~RegistrationModelAlert();
    
    enum RegistrationType {
                Company, 
                Individual, 
                Association 
            };
    
    static const Field TestField;

    static const Field FirstNameField;
    static const Field LastNameField;
    static const Field OrganizationTypeCompanyField;
    static const Field OrganizationTypeIndividualField;
    static const Field OrganizationTypeAssociationField;
    static const Field OrganizationNameField;
    
    virtual void reset();
    
    std::string generateToken();
    
    void setRegistrationType(RegistrationType rType);
    
    virtual bool validateField(Field field);
    
    virtual bool isVisible(Field field) const;
    
private:
    
    Wt::Auth::Identity idpIdentity_;
    Wt::Auth::User existingUser_;
    EmailPolicy emailPolicy_;
    RegistrationType rType;
};

#endif	/* REGISTRATIONMODELALERT_H */

