/* 
 * GUI MessagesWidget.cpp
 * 
 * @author ECHOES Technologies (CDE)
 * 
 * @date 03/08/2015
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS WITHOUT
 * COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012-2013 BY ECHOES TECHNOLGIES SAS
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