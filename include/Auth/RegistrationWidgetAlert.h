/* 
 * File:   RegistrationWidgetAlert.h
 * Author: tsa
 *
 * Created on 17 janvier 2013, 19:41
 */

#ifndef REGISTRATIONWIDGETALERT_H
#define	REGISTRATIONWIDGETALERT_H

#include <string.h>
#include <boost/lexical_cast.hpp>
#include <boost/any.hpp>

#include <Wt/WTemplateFormView>
#include <Wt/Auth/AuthWidget>
#include <Wt/WAnchor>
#include <Wt/WImage>
#include <Wt/WDialog>
#include <Wt/WPushButton>
#include <Wt/WRadioButton>
#include <Wt/WButtonGroup>
#include <Wt/WLineEdit>
#include <Wt/WApplication>
#include <Wt/Auth/Login>

#include "Auth/RegistrationModelAlert.h"

#include "tools/MainIncludeFile.h"
#include "tools/Session.h"
#include <tools/Enums.h>

#include <Enums.h>



class Login;

class RegistrationWidgetAlert : public Wt::WTemplateFormView {
public:
    /*! \brief Constructor
    *
    * Creates a new authentication.
    */
    RegistrationWidgetAlert(Wt::Auth::AuthWidget *authWidget = 0);
    virtual ~RegistrationWidgetAlert();

   /*! \brief Sets the registration model.
    */
   virtual void setModel(RegistrationModelAlert *model);

   /*! \brief Returns the registration model.
    *
    * This returns the model that is used by the widget to do the actual
    * registration.
    */
   RegistrationModelAlert *model() const { return model_; }

   /*! \brief Updates the user-interface.
    *
    * This updates the user-interface to reflect the current state of the
    * model.
    */
   void update();
   
protected:
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
    Wt::WFormWidget *createFormWidget(RegistrationModelAlert::Field field);
    
    Wt::WButtonGroup *organizationGroup;

    /*! \brief Validates the current information.
     *
     * The default implementation simply calls
     * RegistrationModel::validate() on the model.
     *
     * You may want to reimplement this method if you've added other
     * information to the registration form that need validation.
     */
    virtual bool validate();

    /*! \brief Performs the registration.
     *
     * The default implementation checks if the information is valid
     * with validate(), and then calls
     * RegistrationModel::doRegister(). If registration was successful,
     * it calls registerUserDetails() and subsequently logs the user in.
     */
    virtual void doRegister();

    /*! \brief Closes the registration widget.
     *
     * The default implementation simply deletes the widget.
     */
    virtual void close();

    virtual void render(Wt::WFlags<Wt::RenderFlag> flags);

    
private:
    Wt::Auth::AuthWidget *authWidget_;
    RegistrationModelAlert *model_;

    bool created_;
    Wt::Auth::Login *confirmPasswordLogin_;

    void checkFirstName();
    void checkLastName();
    void checkOrganization();
    void checkLoginName();
    void checkPassword();
    void checkPassword2();
    void confirmIsYou();
    void confirmedIsYou();
    void oAuthDone(Wt::Auth::OAuthProcess *oauth, const Wt::Auth::Identity& identity);
    

};

#endif	/* REGISTRATIONWIDGETALERT_H */

