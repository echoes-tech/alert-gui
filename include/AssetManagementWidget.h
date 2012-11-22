/* 
 * File:   AssetManagementWidget.h
 * Author: tsa
 *
 * Created on 14 août 2012, 11:50
 */

#ifndef ASSETMANAGEMENTWIDGET_H
#define	ASSETMANAGEMENTWIDGET_H

#include "GlobalIncludeFile.h"

#include <Wt/WAnchor>
#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WDialog>
#include <Wt/WImage>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>

#include <Wt/WBreak>
#include <Wt/WDatePicker>
#include <Wt/WSelectionBox>
#include <Wt/WIntValidator>
#include <Wt/WTextArea>
#include <Wt/WFileResource>

#include <Wt/WComboBox>
#include <Wt/WSelectionBox>
#include <Wt/WStringListModel>
#include <Wt/Dbo/Query>
#include <Wt/WStandardItem>
#include <Wt/WStandardItemModel>
#include <Wt/Mail/Client>
#include <Wt/Mail/Message>

#include <Wt/WMessageBox>

#include <Wt/WVBoxLayout>
#include <Wt/WHBoxLayout>
#include <Wt/WTable>
#include <Wt/WTableCell>
#include <Wt/WTableView>

#include <Wt/Utils>
#include <Wt/WLabel>

#include <Wt/WGroupBox>
#include <Wt/WButtonGroup>
#include <Wt/WRadioButton>

#include <memory>
#include <iostream>
#include <fstream>
#include <boost/random.hpp>
#include <boost/random/random_device.hpp>
#include <boost/random/uniform_int_distribution.hpp>

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

  virtual Wt::WFormWidget *createFormWidget(Wt::WFormModel::Field field);
  
  
  void addAsset();
  void deleteAsset(long long id);
  Wt::WFileResource *generateScript(long long i, Wt::WString assetName);
  std::string getStringFromFile(std::string resourcePath);

private:
  AssetManagementModel *model_;
  Session * session;

  Wt::WTemplateFormView *mainForm;
  
  bool created_;
  
  void downloadScript(std::string fileName);
  
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

