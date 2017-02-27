/**
 * @file MessagesWidget.cpp
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

#include <Wt/Utils>

#include "MessagesWidget.h"

using namespace std;

MessagesWidget::MessagesWidget(Echoes::Dbo::Session* session, const std::string apiUrl)
{        
    Wt::WApplication *app = Wt::WApplication::instance();
    app->messageResourceBundle().use(AbstractPage::xmlDirectory + "messages", false);
    
    session_ = session;
    apiUrl_ = apiUrl;
    newClass_ = false;
    update();
}

void MessagesWidget::update()
{
    this->clear();
    Wt::WTemplateFormView *templateFormView =
        new Wt::WTemplateFormView(Wt::WString::tr("Alert.messages.Management.template"));
    templateFormView->addStyleClass("template");
    this->addWidget(templateFormView);

    
    // ALERTS
    MessagesTableAlertWidget *mtAlertW = new MessagesTableAlertWidget(session_, apiUrl_);
    mtAlertW->updatePage();
    templateFormView->bindWidget("resource-table-alert", mtAlertW);
    
    // MESSAGE       
    MessagesTableMessageWidget *mtMessageW = new MessagesTableMessageWidget(session_, apiUrl_, mtAlertW);
    templateFormView->bindWidget("resource-table-message", mtMessageW);
    mtAlertW->addPageToUpdate(mtMessageW);

    // HISTORIC       
    MessagesTableHistoricWidget *mtHistoricW = new MessagesTableHistoricWidget(session_, apiUrl_, mtMessageW);
    templateFormView->bindWidget("resource-table-historic", mtHistoricW);
    mtMessageW->addPageToUpdate(mtHistoricW);

}
