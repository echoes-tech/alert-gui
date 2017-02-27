/**
 * @file RecipientsWidget.cpp
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

#include "RecipientsWidget.h"

RecipientsWidget::RecipientsWidget(Echoes::Dbo::Session *session, std::string apiUrl)
{
    Wt::WApplication *app = Wt::WApplication::instance();
    app->messageResourceBundle().use(AbstractPage::xmlDirectory + "recipients",false);
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
       uewMail->updatePage();
       UserEditionWidget *uewSMS = new UserEditionWidget(session_, apiUrl_, 2);
       uewSMS->updatePage();
       UserEditionWidget *uewPush = new UserEditionWidget(session_, apiUrl_, 3);
       uewPush->updatePage();
       
       Wt::WMenuItem *tabMail = tab->addTab(uewMail, "Mail");
       tabMail->setStyleClass("recipients recipients-radius-left");
       
       tab->addTab(uewSMS, "SMS")->setStyleClass("recipients recipients-radius-mid");

       tab->addTab(uewPush, "Push")->setStyleClass("recipients recipients-radius-right");
       
       tabMail->select();
       
       newClass_ = true;
    }
}
