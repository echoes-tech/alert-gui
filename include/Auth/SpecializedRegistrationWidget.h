/* 
 * Gui SpecializedRegistrationWidget.h
 * 
 * @author Echoes Technologies
 * @author Thomas SAQUET (TSA)
 * 
 * @date 13/08/2014
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS WITHOUT
 * COMPANY AUTHORIZATION.
 * AbstractPage
 * COPYRIGHT 2012-2014 BY ECHOES TECHNOLGIES SAS
 * 
 */

#ifndef REGISTRATIONWIDGETALERT_H
#define	REGISTRATIONWIDGETALERT_H

#include <Wt/Auth/RegistrationWidget>

#include "SpecializedAuthWidget.h"

class SpecializedRegistrationWidget : public Wt::Auth::RegistrationWidget {
public:
    /*! \brief Constructor
    *
    * Creates a new authentication.
    */
    SpecializedRegistrationWidget(SpecializedAuthWidget *authWidget = 0) 
           : Wt::Auth::RegistrationWidget(authWidget) {}
    
protected:
    
    std::string generateToken();
    
    /*! \brief Registers more user information.
    *
    * This method is called when a new user has been successfully
    * registered.
    *
    * You may want to reimplement this method if you've added other
    * information to the registration form which needs to be annotated
    * to the user.
    */
    virtual void registerUserDetails(Wt::Auth::User& user);
    
};

#endif	/* REGISTRATIONWIDGETALERT_H */

