/* 
 * GUI MessagesTableAlertWidget.cpp
 * 
 * @author ECHOES Technologies (CDE)
 * 
 * @date 04/08/2015
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS WITHOUT
 * COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012-2013 BY ECHOES TECHNOLGIES SAS
 * 
 */

#include "MessagesTableAlertWidget.h"

using namespace std;

MessagesTableAlertWidget::MessagesTableAlertWidget(Echoes::Dbo::Session *session, string apiUrl)
: AbstractPage(session, apiUrl, "messages-alert", true)
{
    setButtonModif(false);
    setButtonSup(false);
    m_hasAddButton = false;
    
    std::vector<std::pair <int, string>>titles;
    titles.push_back(make_pair(setValidatorType(ETypeJson::text, 0, EMandatory::isMandatory), "name"));
    titles.push_back(make_pair(setValidatorType(ETypeJson::text, 0, EMandatory::isNotMandatory), "statement"));
    titles.push_back(make_pair(setValidatorType(ETypeJson::text, 0, EMandatory::isNotMandatory), "supporter"));
    titles.push_back(make_pair(setValidatorType(ETypeJson::widget, 0, EMandatory::isMandatory), "take"));
    titles.push_back(make_pair(setValidatorType(ETypeJson::widget, 0, EMandatory::isMandatory), "resolved"));

    setTitles(titles);
    
    list<list<pair<string, vector<string>>>> listsUrl;
    list<pair<string, vector<string>>> listUrl;
    
    listUrl.push_back(pair<string, vector<string>>("alerts", vector<string>()));
    listsUrl.push_back(listUrl);
    listUrl.clear();
    
    setUrl(listsUrl);
    
    m_assetComboBox = new Wt::WComboBox();
    m_assetsStandardItemModel = new Wt::WStandardItemModel(0,2,this);

}

void MessagesTableAlertWidget::updatePage()
{  
    
    list<list<pair<string, vector<string>>>> listsUrl;

    list<pair<string, vector<string>>> listUrl;
    vector<string> listParameter;
        
        
    listUrl.push_back(pair<string, vector<string>>("alerts", listParameter));
    listParameter.clear();
    listUrl.push_back(pair<string, vector<string>>("alerts/:id/status", listParameter));
        
    listsUrl.push_back(listUrl);
    listUrl.clear();

    setUrl(listsUrl);
    
    AbstractPage::updatePage();
}

int MessagesTableAlertWidget::addCustomButtonsToResourceTable(long long id, int rowTable, int columnTable)
{
    Wt::WAnchor *assignementButton = new Wt::WAnchor();
    assignementButton->setAttributeValue("class", "btn btn-info");
    assignementButton->setTextFormat(Wt::XHTMLUnsafeText);
    assignementButton->setText("<span class='input-group-btn'><i class='icon-inbox icon-white'></i></span>");
    if(m_alertsData[id].stateID == Echoes::Dbo::EAlertStatus::PENDING
            || m_alertsData[id].stateID == Echoes::Dbo::EAlertStatus::FORWARDING)
    {
        assignementButton->clicked().connect(boost::bind(&MessagesTableAlertWidget::takeAssignement, this, id));
    }
    getResourceTable()->elementAt(rowTable, columnTable)->addWidget(assignementButton); 
    getResourceTable()->elementAt(rowTable, columnTable)->setWidth(Wt::WLength(Wt::WLength(5, Wt::WLength::Percentage)));
    getResourceTable()->elementAt(rowTable, columnTable)->setContentAlignment(Wt::AlignCenter);
    ++columnTable;
    
    Wt::WAnchor *resolvedButton = new Wt::WAnchor();
    resolvedButton->setAttributeValue("class", "btn btn-info");
    resolvedButton->setTextFormat(Wt::XHTMLUnsafeText);
    resolvedButton->setText("<span class='input-group-btn'><i class='icon-chevron-down icon-white'></i></span>");
    if(m_alertsData[id].stateID == Echoes::Dbo::EAlertStatus::SUPPORTED)
    {
        resolvedButton->clicked().connect(boost::bind(&MessagesTableAlertWidget::setResolved, this, id));
    }
    getResourceTable()->elementAt(rowTable, columnTable)->addWidget(resolvedButton); 
    getResourceTable()->elementAt(rowTable, columnTable)->setWidth(Wt::WLength(Wt::WLength(5, Wt::WLength::Percentage)));
    getResourceTable()->elementAt(rowTable, columnTable)->setContentAlignment(Wt::AlignCenter);
    return ++columnTable;
}

void MessagesTableAlertWidget::takeAssignement(long long id)
{
    Wt::Http::Message content;
    content.addBodyText("{\n\t\"user\": " + boost::lexical_cast<string>(m_session->user().id()) + "\n}");

    const string apiAddress = getApiUrl() + "/alerts/"
                        + boost::lexical_cast<string>(id)
                        + "/assign"
                        + "?login=" + Wt::Utils::urlEncode(m_session->user()->eMail.toUTF8())
                        + "&token=" + m_session->user()->token.toUTF8();
    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&MessagesTableAlertWidget::assignementCallBack, this, _1, _2, client));

    Wt::log("debug") << "MessagesAlertWidget : [PUT] address to call : " << apiAddress;

    if (client->put(apiAddress, content))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
    {
        Wt::log("error") << "Error Client Http";
    }
}

void MessagesTableAlertWidget::setResolved(long long id)
{
    Wt::Http::Message content;
    content.addBodyText("{\n\t\"user\": " + boost::lexical_cast<string>(m_session->user().id()) + "\n}");

    const string apiAddress = getApiUrl() + "/alerts/"
                        + boost::lexical_cast<string>(id)
                        + "/resolve"
                        + "?login=" + Wt::Utils::urlEncode(m_session->user()->eMail.toUTF8())
                        + "&token=" + m_session->user()->token.toUTF8();
    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&MessagesTableAlertWidget::resolveCallBack, this, _1, _2, client));

    Wt::log("debug") << "MessagesAlertWidget : [PUT] address to call : " << apiAddress;

    if (client->put(apiAddress, content))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
    {
        Wt::log("error") << "Error Client Http";
    }
}

void MessagesTableAlertWidget::assignementCallBack(boost::system::error_code err, const Wt::Http::Message& response, Wt::Http::Client *client)
{
    delete client;
    Wt::WApplication::instance()->resumeRendering();
    Wt::Json::Value error;

    if (!err)
    {
        if (response.status() >= 200 && response.status() < 300)
        {
            Wt::WMessageBox::show(tr("Alert." + m_xmlPageName + ".ope-success-title"),
                 tr("Alert." + m_xmlPageName + ".ope-success"), Wt::Ok);
        }
        else
        {
            Wt::WMessageBox::show(tr("Alert." + m_xmlPageName + ".ope-failed-title"),
                 tr("Alert." + m_xmlPageName + ".ope-failed"), Wt::Ok);
        }
    }
    else
    {
        Wt::log("warning") << "[" + tr("Alert." + m_xmlPageName + ".add-form."
                                       + m_xmlPageName) + " Widget] Http::Client error2: " << response.body();
        Wt::WMessageBox::show(tr("Alert." + m_xmlPageName + ".database-error-title"),
                              tr("Alert." + m_xmlPageName + ".database-error"), Wt::Ok);
    }
    updatePage();
}

void MessagesTableAlertWidget::resolveCallBack(boost::system::error_code err, const Wt::Http::Message& response, Wt::Http::Client *client)
{
    delete client;
    Wt::WApplication::instance()->resumeRendering();
    Wt::Json::Value error;

    if (!err)
    {
        if (response.status() >= 200 && response.status() < 300)
        {
            Wt::WMessageBox::show(tr("Alert." + m_xmlPageName + ".ope-success-title"),
                 tr("Alert." + m_xmlPageName + ".ope-success"), Wt::Ok);
        }
        else
        {
            Wt::WMessageBox::show(tr("Alert." + m_xmlPageName + ".ope-failed-title"),
                 tr("Alert." + m_xmlPageName + ".ope-failed"), Wt::Ok);
        }
    }
    else
    {
        Wt::log("warning") << "[" + tr("Alert." + m_xmlPageName + ".add-form."
                                       + m_xmlPageName) + " Widget] Http::Client error2: " << response.body();
        Wt::WMessageBox::show(tr("Alert." + m_xmlPageName + ".database-error-title"),
                              tr("Alert." + m_xmlPageName + ".database-error"), Wt::Ok);
    }
    updatePage();
}


std::vector<Wt::WInteractWidget*> MessagesTableAlertWidget::initRowWidgets(Wt::Json::Object jsonObject, std::vector<Wt::Json::Value> jsonResource, int cpt)
{
    if(cpt == 0)
        m_alertsData.clear();
    
    struct AlertData alertData;
        
    vector<Wt::WInteractWidget *> rowWidgets;
    try
    {
        Wt::Json::Object alertStatusInfo = jsonResource.at(cpt+1);
        
        int alertID = alertStatusInfo.get("id");
        alertData.alertID = alertID;
        
        Wt::WString name = alertStatusInfo.get("name");
        rowWidgets.push_back(new Wt::WText(name));
        alertData.name = name;
        
        int stateID = alertStatusInfo.get("state");    
        rowWidgets.push_back(new Wt::WText(getStateName(stateID)));
        alertData.stateID = stateID;
        
        if(alertStatusInfo.contains("user"))
        {
            Wt::WString user = alertStatusInfo.get("user");
            rowWidgets.push_back(new Wt::WText(user));
            alertData.user = user;
        }
        else
        {
            rowWidgets.push_back(new Wt::WText("-"));
            alertData.user = "";
        }

        m_alertsData[alertID] = alertData;
    }
    catch (Wt::Json::ParseError const& e)
    {
        Wt::log("warning") << "[MessagesTableAlertWidget] Problems parsing JSON";
        Wt::WMessageBox::show(tr("Alert.messages-Alert.database-error-title"), tr("Alert.messages-alert.database-error"), Wt::Ok);
    }
    catch (Wt::Json::TypeException const& e)
    {
        Wt::log("warning") << "[MessagesTableAlertWidget] JSON Type Exception";
        //Wt::WMessageBox::show(tr("Alert.messages-message.database-error-title"), tr("Alert.messages-message.database-error"), Wt::Ok);
    }
    
    return rowWidgets;
}

Wt::WString MessagesTableAlertWidget::getStateName(long long stateID)
{    
    map<long long, Wt::WString> mapStateName;
    
    mapStateName[Echoes::Dbo::EAlertStatus::BACKTONORMAL] = tr("Alert.messages-alert.backtonormal");
    mapStateName[Echoes::Dbo::EAlertStatus::FORWARDING] = tr("Alert.messages-alert.forwarding");
    mapStateName[Echoes::Dbo::EAlertStatus::PENDING] = tr("Alert.messages-alert.pending");
    mapStateName[Echoes::Dbo::EAlertStatus::SUPPORTED] = tr("Alert.messages-alert.supported");
    
    return mapStateName[stateID];
}

long long   MessagesTableAlertWidget::getSelectedAlertID()
{
    return m_alertsData[getSelectedID()].alertID;
}

Wt::WString   MessagesTableAlertWidget::getSelectedAlertName()
{
    return m_alertsData[getSelectedID()].name;
}

/*Wt::WComboBox *MessagesTableAlertWidget::popupAdd(Wt::WDialog *dialog)
{
    m_assetComboBox = new Wt::WComboBox(dialog->contents());
    m_assetComboBox->setModel(m_assetsStandardItemModel);
    m_assetComboBox->setCurrentIndex(0);
 
    return m_assetComboBox;
}*/

/*
void PluginsTablePluginWidget::setAddResourceMessage(Wt::Http::Message *message,vector<Wt::WInteractWidget*>* argument)
{
    vector<Wt::WInteractWidget*>::iterator it = argument->begin();
    
    message->addBodyText("{");
    message->addBodyText("\n\"name\": \"" + ((Wt::WLineEdit*)(*it++))->text().toUTF8() + "\"");
    message->addBodyText(",\n\"desc\": \"" + ((Wt::WLineEdit*)(*it++))->text().toUTF8() + "\"");
    message->addBodyText(",\n\"asset_id\": \"" + m_assetsStandardItemModel->item((((Wt::WComboBox*)(*it))->currentIndex() == -1 ? 0 : ((Wt::WComboBox*)(*it))->currentIndex()), 1)->text().toUTF8() + "\"");
    message->addBodyText("\n}");
}

void PluginsTablePluginWidget::setModifResourceMessage(Wt::Http::Message *message,vector<Wt::WInteractWidget*>* argument)
{
    vector<Wt::WInteractWidget*>::iterator it = argument->begin();
    
    message->addBodyText("{");
    message->addBodyText("\n\"name\": \"" + ((Wt::WLineEdit*)(*it++))->text().toUTF8() + "\"");
    message->addBodyText(",\n\"desc\": \"" + ((Wt::WLineEdit*)(*it++))->text().toUTF8() + "\"");
    message->addBodyText(",\n\"asset_id\": \"" + m_assetsStandardItemModel->item((((Wt::WComboBox*)(*it))->currentIndex() == -1 ? 0 : ((Wt::WComboBox*)(*it))->currentIndex()), 1)->text().toUTF8() + "\"");
    message->addBodyText("\n}");
}*/

