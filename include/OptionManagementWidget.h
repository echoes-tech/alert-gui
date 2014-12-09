/* 
 * File:   OptionManagementWidget.h
 * Author: tsa
 *
 * Created on 11 novembre 2012, 12:09
 */

#ifndef OPTIONMANAGEMENTWIDGET_H
#define	OPTIONMANAGEMENTWIDGET_H

#include "GlobalIncludeFile.h"
#include <Wt/WApplication>
#include <Wt/WContainerWidget>

class OptionManagementWidget : public Wt::WContainerWidget {
public:
    OptionManagementWidget(OptionManagementModel *model, Echoes::Dbo::Session *session, std::string apiUrl);
    virtual ~OptionManagementWidget();
protected:

  virtual bool validate();


  virtual void render(Wt::WFlags<Wt::RenderFlag> flags);
  void createUI();
  void replaceChangePasswordWidget();

  virtual Wt::WFormWidget *createFormWidget(Wt::WFormModel::Field field);
  
  void setApiUrl(std::string apiUrl);
  std::string getApiUrl() const;

private:
  OptionManagementModel *model_;
  Echoes::Dbo::Session * session;
  std::string apiUrl_;
  Wt::WComboBox roleComboBox;

  Wt::WTemplateFormView *mainForm;
  
  bool created_;
  
  void askSms();
  void changeRole();
  void fillRoleSelector();
  void getQuota(boost::system::error_code err, const Wt::Http::Message& response);
  void getRoles(boost::system::error_code err, const Wt::Http::Message& response);
  
  
};

#endif	/* OPTIONMANAGEMENTWIDGET_H */

