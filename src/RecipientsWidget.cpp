/* 
 * File:   RecipientsWidget.cpp
 * Author: gkr
 * 
 * Created on 13 novembre 2013, 17:03
 */

#include "RecipientsWidget.h"

RecipientsWidget::RecipientsWidget(Session *session, std::string apiUrl)
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
       Wt::WTemplateFormView *templat =
               new Wt::WTemplateFormView(Wt::WString::tr("Alert.recipients.Management.template"));
       templat->addStyleClass("template");
       this->addWidget(templat);
       Wt::WTabWidget *tab = new Wt::WTabWidget();
       templat->bindWidget("resource-table", tab);

       UserEditionWidget *uew = new UserEditionWidget(session_, apiUrl_, 1);
       uew->recoverListAsset();
       uew->setNameSpecial("mail");
       Wt::WMenuItem *tabMail = tab->addTab(uew, "Mail");
       tabMail->setStyleClass("recipients recipients-radius-left");
       tabMail->select();

       uew = new UserEditionWidget(session_, apiUrl_, 2);
       uew->recoverListAsset();
       uew->setNameSpecial("sms");
       tab->addTab(uew, "SMS")->setStyleClass("recipients recipients-radius-mid");

       uew = new UserEditionWidget(session_, apiUrl_, 3);
       uew->recoverListAsset();
       uew->setNameSpecial("app");
       tab->addTab(uew, "App")->setStyleClass("recipients recipients-radius-right");
       newClass_ = true;
    }
}
