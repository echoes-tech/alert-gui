/* 
 * File:   ProbeDownloadWidget.h
 * Author: tsa
 *
 * Created on 14 août 2012, 11:50
 */

#ifndef ASSETMANAGEMENTWIDGET_H
#define	ASSETMANAGEMENTWIDGET_H

#include <Wt/WButtonGroup>
#include <Wt/WRadioButton>
#include <Wt/WGroupBox>
#include <Wt/WBreak>
#include "WTemplateFormView"
#include "AssetManagementModel.h"
#include <Wt/Mail/Client>

#include "tools/Session.h"





#include <Wt/WApplication>
#include <Wt/WBreak>
#include <Wt/WDatePicker>
#include <Wt/WSelectionBox>
#include <Wt/WContainerWidget>
#include <Wt/WImage>
#include <Wt/WIntValidator>
#include <Wt/WLabel>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WTableCell>
#include <Wt/WTextArea>
#include <Wt/WText>
#include<Wt/WFileResource>



class AssetManagementWidget : public Wt::WContainerWidget
{
public:
  /*! \brief Constructor
   *
   * Creates a new authentication.
   */
  AssetManagementWidget(AssetManagementModel *model, Session *session);

  /*! \brief Returns the registration model.
   *
   * This returns the model that is used by the widget to do the actual
   * registration.
   */
  AssetManagementModel *model() const { return model_; }
  


  
  long long userId;
  
 
  
  
  
  
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
  void createUI();

  virtual Wt::WFormWidget *createFormWidget(AssetManagementModel::Field field);
  
  void deleteAsset(long long id);
  Wt::WFileResource *generateScript(long long i);
  std::string getStringFromFile(std::string resourcePath);

private:
  AssetManagementModel *model_;
  Session * session;

  bool created_;
  
  
  
  
  
  
  Wt::WContainerWidget *feedbackMessages_;

  Wt::WLineEdit *nameEdit_;
  Wt::WLineEdit *firstNameEdit_;

  Wt::WComboBox *countryEdit_;
  Wt::WComboBox *cityEdit_;

  Wt::WDatePicker *birthDateEdit_;
  Wt::WPushButton *childCountEdit_;
  Wt::WLineEdit *weightEdit_;

  Wt::WTextArea *remarksEdit_;
  
};



#endif	/* ASSETMANAGEMENTWIDGET_H */

