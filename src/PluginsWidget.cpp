/* 
 * File:   PluginsWidget.cpp
 * Author: gdr
 *
 * Created on 18 mars 2013, 11:59
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
       PluginsTablePluginWidget *uewMail = new PluginsTablePluginWidget(session_, apiUrl_);
       uewMail->getResourceList();
       templateFormView->bindWidget("resource-table-plugin", uewMail);
       
       // SOURCE       
       PluginsTablePluginWidget *uewSMS = new PluginsTablePluginWidget(session_, apiUrl_);
       uewSMS->getResourceList();
       templateFormView->bindWidget("resource-table-source", uewSMS);
       
       // SEARCH       
       PluginsTablePluginWidget *uewSMSa = new PluginsTablePluginWidget(session_, apiUrl_);
       uewSMSa->getResourceList();
       templateFormView->bindWidget("resource-table-search", uewSMSa);
       
       newClass_ = true;
    }
}