/**
 * @file PluginsWidget.cpp
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

#include "PluginsWidget.h"

using namespace std;

PluginsWidget::PluginsWidget(Echoes::Dbo::Session* session, const std::string apiUrl)
{        
    Wt::WApplication *app = Wt::WApplication::instance();
    app->messageResourceBundle().use(AbstractPage::xmlDirectory + "plugins", false);
    
    session_ = session;
    apiUrl_ = apiUrl;
    newClass_ = false;
    update();
}

void PluginsWidget::update()
{
   if (!newClass_)
    {
       this->clear();
       Wt::WTemplateFormView *templateFormView =
               new Wt::WTemplateFormView(Wt::WString::tr("Alert.plugins.Management.template"));
       templateFormView->addStyleClass("template");
       this->addWidget(templateFormView);
       
       // PLUGIN
       PluginsTablePluginWidget *ptPluginW = new PluginsTablePluginWidget(session_, apiUrl_);
       ptPluginW->updatePage();
       templateFormView->bindWidget("resource-table-plugin", ptPluginW);
       
       // SOURCE       
       PluginsTableSourceWidget *ptSourceW = new PluginsTableSourceWidget(session_, apiUrl_, ptPluginW);
       templateFormView->bindWidget("resource-table-source", ptSourceW);
       ptPluginW->addPageToUpdate(ptSourceW);
       
       // SEARCH       
       PluginsTableSearchWidget *ptSearchW = new PluginsTableSearchWidget(session_, apiUrl_, ptSourceW);
       templateFormView->bindWidget("resource-table-search", ptSearchW);
       ptSourceW->addPageToUpdate(ptSearchW);
       
       // FILTER       
       PluginsTableFilterWidget *ptFilterW = new PluginsTableFilterWidget(session_, apiUrl_, ptSearchW);
       templateFormView->bindWidget("resource-table-filter", ptFilterW);
       ptSearchW->addPageToUpdate(ptFilterW);
       
       // ASSOCIATION       
       PluginsTableAssociationWidget *ptAssociationW = new PluginsTableAssociationWidget(session_, apiUrl_, ptFilterW);
       templateFormView->bindWidget("resource-table-association", ptAssociationW);
       ptFilterW->addPageToUpdate(ptAssociationW);
       
       newClass_ = true;
    }
}
