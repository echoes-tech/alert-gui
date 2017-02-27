/**
 * @file ClassTest.cpp
 * @author Thomas Saquet, Florent Poinsaut
 * @date 
 * @brief File containing example of doxygen usage for quick reference.
 *
 * Alert - GUI is a part of the Alert software
 * Copyright (C) 2013-2017
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 * 
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
       Wt::WMenuItem *tabMail = tab->addTab(uew, "Mail");
       tabMail->setStyleClass("recipients recipients-radius-left");
       tabMail->select();
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
