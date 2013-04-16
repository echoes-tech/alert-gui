/* 
 * File:   UserEditionWidget.h
 * Author: tsa
 *
 * Created on 14 août 2012, 11:50
 */

#ifndef USEREDITIONWIDGET_H
#define	USEREDITIONWIDGET_H

#include "GlobalIncludeFile.h"

#include <Wt/WButtonGroup>
#include <Wt/WRadioButton>
#include <Wt/WGroupBox>
#include <Wt/WBreak>

#include <Wt/WAnchor>
#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WDialog>
#include <Wt/WImage>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>

#include <Wt/WComboBox>
#include <Wt/WSelectionBox>
#include <Wt/WStringListModel>

#include <memory>

class UserEditionWidget : public Wt::WTemplateFormView
{
public:
  /*! \brief Constructor
   *
   * Creates a new authentication.
   */
  UserEditionWidget();

  /*! \brief Sets the registration model.
   */
  void setModel(UserEditionModel *model);
  
  /*! \brief Sets the session.
   */
  void setSession(Session *session);

  /*! \brief Returns the registration model.
   *
   * This returns the model that is used by the widget to do the actual
   * registration.
   */
  UserEditionModel *model() const { return model_; }

//  void companyClick();
//  void individualClick();
//  void associationClick();
  
  void checkMediaEmail();
  void checkMediaSms();
  
  /*! \brief Updates the user-interface.
   *
   * This updates the user-interface to reflect the current state of the
   * model.
   */
  void update();
  
  Wt::WTable *emailsTable;
  Wt::WTable *smsTable;
  Wt::WLineEdit *emailEdit;
  
  std::map<long long, Wt::WString> getMediasForCurrentUser(int mediaType);
  
  
  void addMedia(Wt::WFormModel::Field field, int medId);
  void deleteMedia(int medEnumId, long long medId, int row);
  
  void addEmail();
  void deleteEmail();
  void addSms();
  void deleteSms();

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
  
  virtual void close();

  virtual void render(Wt::WFlags<Wt::RenderFlag> flags);

protected:
  virtual Wt::WFormWidget *createFormWidget(Wt::WFormModel::Field field);

private:
  AuthWidget *authWidget_;
  UserEditionModel *model_;
  Session * session;

  bool created_;
  Login *confirmPasswordLogin_;
  
  Wt::WGroupBox *organizationContainer;
  Wt::WButtonGroup *organizationGroup;

};



#endif	/* USEREDITIONWIDGET_H */

