/* 
 * File:   RegistrationWidget.h
 * Author: tsa
 *
 * Created on 14 août 2012, 11:50
 */

#ifndef REGISTRATIONWIDGET_H
#define	REGISTRATIONWIDGET_H

#include <Wt/WButtonGroup>
#include <Wt/WRadioButton>
#include <Wt/WGroupBox>
#include <Wt/WBreak>
#include "WTemplateFormView"
#include "RegistrationModel.h"

namespace Wt {
  namespace Auth {

class AuthWidget;
class Login;
class OAuthProcess;

/*! \class RegistrationWidget Wt/Auth/RegistrationWidget
 *  \brief A registration widget.
 *
 * This implements a widget which allows a new user to register.  The
 * widget renders the <tt>"Wt.Auth.template.registration"</tt>
 * template. and uses a RegistrationModel for the actual registration
 * logic.
 *
 * Typically, you may want to specialize this widget to ask for other
 * information.
 *
 * \ingroup auth
 */
class WT_API RegistrationWidget : public WTemplateFormView
{
public:
  /*! \brief Constructor
   *
   * Creates a new authentication.
   */
  RegistrationWidget(AuthWidget *authWidget = 0);

  /*! \brief Sets the registration model.
   */
  void setModel(RegistrationModel *model);

  /*! \brief Returns the registration model.
   *
   * This returns the model that is used by the widget to do the actual
   * registration.
   */
  RegistrationModel *model() const { return model_; }

//  void companyClick();
//  void individualClick();
//  void associationClick();
  
  /*! \brief Updates the user-interface.
   *
   * This updates the user-interface to reflect the current state of the
   * model.
   */
  void update();

protected:
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

  /*! \brief Registers more user information.
   *
   * This method is called when a new user has been successfully
   * registered.
   *
   * You may want to reimplement this method if you've added other
   * information to the registration form which needs to be annotated
   * to the user.
   */
  virtual void registerUserDetails(User& user);

  virtual void render(WFlags<RenderFlag> flags);

protected:
  virtual WFormWidget *createFormWidget(RegistrationModel::Field field);

private:
  AuthWidget *authWidget_;
  RegistrationModel *model_;

  bool created_;
  Login *confirmPasswordLogin_;
  
  Wt::WGroupBox *organizationContainer;
  Wt::WButtonGroup *organizationGroup;

  void checkLoginName();
  void checkOrganization();
  void checkPassword();
  void checkPassword2();
  void confirmIsYou();
  void confirmedIsYou();
  void oAuthDone(OAuthProcess *oauth, const Identity& identity);
};

  }
}

#endif	/* REGISTRATIONWIDGET_H */

