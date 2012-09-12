/* 
 * File:   ProbeDownloadWidget.h
 * Author: tsa
 *
 * Created on 14 août 2012, 11:50
 */

#ifndef PROBEDOWNLOADWIDGET_H
#define	PROBEDOWNLOADWIDGET_H

#include <Wt/WButtonGroup>
#include <Wt/WRadioButton>
#include <Wt/WGroupBox>
#include <Wt/WBreak>
#include "WTemplateFormView"
#include "ProbeDownloadModel.h"
#include <Wt/Mail/Client>

#include "tools/Session.h"


class ProbeDownloadWidget : public Wt::WTemplateFormView
{
public:
  /*! \brief Constructor
   *
   * Creates a new authentication.
   */
  ProbeDownloadWidget();

  /*! \brief Sets the registration model.
   */
  void setModel(ProbeDownloadModel *model);
  
  /*! \brief Sets the session.
   */
  void setSession(Session *session);

  /*! \brief Returns the registration model.
   *
   * This returns the model that is used by the widget to do the actual
   * registration.
   */
  ProbeDownloadModel *model() const { return model_; }
  
  /*! \brief Updates the user-interface.
   *
   * This updates the user-interface to reflect the current state of the
   * model.
   */
  void update();
  
  long long userId;
  Wt::WComboBox *comboOs;
  Wt::WComboBox *comboArch;
  
  
  Wt::WStringListModel *getMediasForCurrentUser(int mediaType);
  
  
  void sendMail();
  
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


  /*! \brief Closes the registration widget.
   *
   * The default implementation simply deletes the widget.
   */
  virtual void close();


  virtual void render(Wt::WFlags<Wt::RenderFlag> flags);

protected:
  virtual Wt::WFormWidget *createFormWidget(ProbeDownloadModel::Field field);

private:
  ProbeDownloadModel *model_;
  Session * session;

  bool created_;
  
  
};



#endif	/* PROBEDOWNLOADWIDGET_H */

