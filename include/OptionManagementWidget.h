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
    OptionManagementWidget(OptionManagementModel *model, Session *session);
    virtual ~OptionManagementWidget();
protected:

  virtual bool validate();


  virtual void render(Wt::WFlags<Wt::RenderFlag> flags);
  void createUI();

  virtual Wt::WFormWidget *createFormWidget(Wt::WFormModel::Field field);
  
  
//  void saveOptions();
  

private:
  OptionManagementModel *model_;
  Session * session;

  Wt::WTemplateFormView *mainForm;
  
  bool created_;
  
  void askSms();
  
  
};

#endif	/* OPTIONMANAGEMENTWIDGET_H */

