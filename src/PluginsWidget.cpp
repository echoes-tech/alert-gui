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
    app->messageResourceBundle().use("plugins", false);
    
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
       ptPluginW->getResourceList();
       templateFormView->bindWidget("resource-table-plugin", ptPluginW);
       
       // SOURCE       
       PluginsTableSourceWidget *ptSourceW = new PluginsTableSourceWidget(session_, apiUrl_, ptPluginW);
       ptSourceW->getResourceList();
       templateFormView->bindWidget("resource-table-source", ptSourceW);
       ptPluginW->addPageToUpdate(ptSourceW);
       
       // SEARCH       
//       PluginsTablePluginWidget *ptSearchW = new PluginsTablePluginWidget(session_, apiUrl_);
//       ptSearchW->getResourceList();
//       templateFormView->bindWidget("resource-table-search", ptSearchW);
       
       newClass_ = true;
    }
}