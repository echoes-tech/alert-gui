/* 
 * GUI PluginsTablePluginWidget.h
 * 
 * @author ECHOES Technologies (MLA)
 * 
 * @date 09/07/2014
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS WITHOUT
 * COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012-2013 BY ECHOES TECHNOLGIES SAS
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