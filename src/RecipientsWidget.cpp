/* 
 * File:   RecipientsWidget.cpp
 * Author: gkr
 * 
 * Created on 13 novembre 2013, 17:03
 */

#include "RecipientsWidget.h"

RecipientsWidget::RecipientsWidget(Echoes::Dbo::Session *session, std::string apiUrl)
{
    Wt::WApplication *app = Wt::WApplication::instance();
    app->messageResourceBundle().use("recipients",false);
    session_ = session;
    apiUrl_ = apiUrl;
    newClass_ = false;
    update();
}

void    RecipientsWidget::update()
{
   if (!newClass_)
    {
       this->clear();
       Wt::WTemplateFormView *templateFormView =
               new Wt::WTemplateFormView(Wt::WString::tr("Alert.recipients.Management.template"));
       templateFormView->addStyleClass("template");
       this->addWidget(templateFormView);
       Wt::WTabWidget *tab = new Wt::WTabWidget();
       templateFormView->bindWidget("resource-table", tab);

       UserEditionWidget *uewMail = new UserEditionWidget(session_, apiUrl_, 1);
       uewMail->getResourceList();
       UserEditionWidget *uewSMS = new UserEditionWidget(session_, apiUrl_, 2);
       uewSMS->getResourceList();
       UserEditionWidget *uewPush = new UserEditionWidget(session_, apiUrl_, 3);
       uewPush->getResourceList();
       
       Wt::WMenuItem *tabMail = tab->addTab(uewMail, "Mail");
       tabMail->setStyleClass("recipients recipients-radius-left");
       
       tab->addTab(uewSMS, "SMS")->setStyleClass("recipients recipients-radius-mid");

       tab->addTab(uewPush, "Push")->setStyleClass("recipients recipients-radius-right");
       
       tabMail->select();
       
       newClass_ = true;
    }
}
