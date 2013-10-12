/*
 * Copyright (C) 2011 Emweb bvba, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include "Wt/Auth/AbstractPasswordService"
#include "Wt/Auth/AuthModel"
#include "Wt/Auth/Login"
#include "UpdatePasswordWidget.h"
#include "Wt/Auth/User"

#include "Wt/WLineEdit"
#include "Wt/WPushButton"
#include "Wt/WText"
#include "Auth/RegistrationModelAlert.h"

namespace Wt {
  namespace Auth {

UpdatePasswordWidget::UpdatePasswordWidget(const User& user,
					   RegistrationModelAlert *registrationModel,
					   AuthModel *authModel,
					   WContainerWidget *parent)
  : WTemplateFormView(tr("Wt.Auth.template.update-password"), parent),
    user_(user),
    registrationModel_(registrationModel),
    authModel_(authModel)
{
  registrationModel_->setValue(RegistrationModelAlert::LoginNameField,
			       user.identity(Identity::LoginName));
  registrationModel_->setReadOnly(RegistrationModelAlert::LoginNameField, true);

  if (authModel_ && authModel_->baseAuth()->emailVerificationEnabled()) {
    /*
     * This is set in the model so that the password checker can take
     * into account whether the password is derived from the email
     * address.
     */
    registrationModel_->setValue
      (RegistrationModelAlert::EmailField,
       WT_USTRING::fromUTF8(user.email() + " " + user.unverifiedEmail()));
  }

  // Make sure it does not block validation
  registrationModel_->setVisible(RegistrationModelAlert::EmailField, false);

  WPushButton *okButton = new WPushButton(tr("Wt.WMessageBox.Ok"));
  WPushButton *cancelButton = new WPushButton(tr("Wt.WMessageBox.Cancel"));

  if (authModel_) {
    authModel_->setValue(AuthModel::LoginNameField,
			 user.identity(Identity::LoginName));

    updateViewField(authModel_, AuthModel::PasswordField);

    authModel_->configureThrottling(okButton);

    WLineEdit *password = resolve<WLineEdit *>(AuthModel::PasswordField);
    password->setFocus();
  }

  updateView(registrationModel_);

  WLineEdit *password = resolve<WLineEdit *>
    (RegistrationModelAlert::ChoosePasswordField);
  WLineEdit *password2 = resolve<WLineEdit *>
    (RegistrationModelAlert::RepeatPasswordField);
  WText *password2Info = resolve<WText *>
    (RegistrationModelAlert::RepeatPasswordField + std::string("-info"));

  registrationModel_->validatePasswordsMatchJS(password,
					       password2, password2Info);

  if (!authModel_)
    password->setFocus();

  okButton->clicked().connect(this, &UpdatePasswordWidget::doUpdate);
  cancelButton->clicked().connect(this, &UpdatePasswordWidget::close);

  bindWidget("ok-button", okButton);
  bindWidget("cancel-button", cancelButton);

}

WFormWidget *UpdatePasswordWidget::createFormWidget(WFormModel::Field field)
{
  WFormWidget *result = 0;

  if (field == RegistrationModelAlert::LoginNameField) {
    result = new WLineEdit();
  } else if (field == AuthModel::PasswordField) {
    WLineEdit *p = new WLineEdit();
    p->setEchoMode(WLineEdit::Password);
    result = p;
  } else if (field == RegistrationModelAlert::ChoosePasswordField) {
    WLineEdit *p = new WLineEdit();
    p->setEchoMode(WLineEdit::Password);
    p->keyWentUp().connect
      (boost::bind(&UpdatePasswordWidget::checkPassword, this));
    p->changed().connect
      (boost::bind(&UpdatePasswordWidget::checkPassword, this));
    result = p;
  } else if (field == RegistrationModelAlert::RepeatPasswordField) {
    WLineEdit *p = new WLineEdit();
    p->setEchoMode(WLineEdit::Password);
    p->changed().connect
      (boost::bind(&UpdatePasswordWidget::checkPassword2, this));
    result = p;
  }

  return result;
}

void UpdatePasswordWidget::checkPassword()
{
  updateModelField(registrationModel_, RegistrationModelAlert::ChoosePasswordField);
  registrationModel_->validateField(RegistrationModelAlert::ChoosePasswordField);
  updateViewField(registrationModel_, RegistrationModelAlert::ChoosePasswordField);
}

void UpdatePasswordWidget::checkPassword2()
{
  updateModelField(registrationModel_, RegistrationModelAlert::RepeatPasswordField);
  registrationModel_->validateField(RegistrationModelAlert::RepeatPasswordField);
  updateViewField(registrationModel_, RegistrationModelAlert::RepeatPasswordField);
}

bool UpdatePasswordWidget::validate()
{
  bool valid = true;

  if (authModel_) {
    updateModelField(authModel_, AuthModel::PasswordField);

    if (!authModel_->validate()) {
      updateViewField(authModel_, AuthModel::PasswordField);
      valid = false;
    }
  }

  registrationModel_->validateField(RegistrationModelAlert::LoginNameField);
  checkPassword();
  checkPassword2();

  registrationModel_->validateField(RegistrationModelAlert::EmailField);

  //FIXME: should be commented, try to understand that
  //  if (!registrationModel_->valid()) 
  //    valid = false;

  return valid;
}

void UpdatePasswordWidget::doUpdate()
{
    std::cout << "before if" << std::endl;
  if (validate()) {
      std::cout << "in if" << std::endl;
    WT_USTRING password
      = registrationModel_->valueText(RegistrationModelAlert::ChoosePasswordField);
    registrationModel_->passwordAuth()->updatePassword(user_, password);
    registrationModel_->login().login(user_);

    close();
  }
}

void UpdatePasswordWidget::close()
{
  delete this;
}

  }
}
