/* 
 * File:   ClassTest.cpp
 * Author: gkr
 * 
 * Created on 23 octobre 2013, 14:24
 */

#include "ClassTest.h"

ClassTest::ClassTest(Session *session, std::string apiUrl)

{
        Wt::WApplication *app = Wt::WApplication::instance();
    app->messageResourceBundle().use("test",false);
    session_ = session;
    apiUrl_ = apiUrl;
    newClass_ = false;
    update();
}

void    ClassTest::update()
{
   if (!newClass_)
    {
       this->clear();
       Wt::WTemplateFormView *templat =
               new Wt::WTemplateFormView(Wt::WString::tr("Alert.test.Management.template"));
       templat->addStyleClass("template");
       this->addWidget(templat);
       Wt::WTabWidget *tab = new Wt::WTabWidget();
       templat->bindWidget("resource-table", tab);
       UserEditionWidget *uew = new UserEditionWidget(session_, apiUrl_, 2);
       uew->recoverListAsset();
       uew->setNameSpecial("mail");
       tab->addTab(uew, "Mail")->setStyleClass("recipients recipients-radius-left");
       uew = new UserEditionWidget(session_, apiUrl_, 1);
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

void    ClassTest::initPage()
{
}