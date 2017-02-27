/**
 * @file MessagesTableAlertWidget.cpp
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
    titles.push_back(make_pair(setValidatorType(ETypeJson::text, 0, EMandatory::isMandatory), "led"));
    titles.push_back(make_pair(setValidatorType(ETypeJson::widget, 0, EMandatory::isMandatory), "forward"));
    titles.push_back(make_pair(setValidatorType(ETypeJson::widget, 0, EMandatory::isMandatory), "take"));
    titles.push_back(make_pair(setValidatorType(ETypeJson::widget, 0, EMandatory::isMandatory), "resolved"));

    setTitles(titles);
    
    list<list<pair<string, vector<string>>>> listsUrl;
    list<pair<string, vector<string>>> listUrl;
    
    listUrl.push_back(pair<string, vector<string>>("alerts", vector<string>()));
    listsUrl.push_back(listUrl);
    listUrl.clear();
    
    setUrl(listsUrl);
    
    usersModel_ = new Wt::WStandardItemModel(0, 2, this);
    
    timer_refresh = new Wt::WTimer;
    timer_refresh->setInterval(60000);
    timer_refresh->timeout().connect(boost::bind(&MessagesTableAlertWidget::updatePage, this));
    timer_refresh->start();
    
    m_assetComboBox = new Wt::WComboBox();
    m_assetsStandardItemModel = new Wt::WStandardItemModel(0,2,this);
}

void MessagesTableAlertWidget::updatePage()
{  
    if (!Wt::WApplication::instance()->internalPathMatches("/messages"))
    {
        m_autoUpdate = true;
        timer_refresh->stop();
        return;
    }
    if(!timer_refresh->isActive())
        timer_refresh->start();
    list<list<pair<string, vector<string>>>> listsUrl;

    list<pair<string, vector<string>>> listUrl;
    vector<string> listParameter;
        
        
    listUrl.push_back(pair<string, vector<string>>("alerts", listParameter));
    listParameter.clear();
    listUrl.push_back(pair<string, vector<string>>("alerts/:id/status", listParameter));        
    listsUrl.push_back(listUrl);
    listUrl.clear();
    
    listUrl.push_back(pair<string, vector<string>>("users", vector<string>()));                     
    listsUrl.push_back(listUrl);
    listUrl.clear();

    setUrl(listsUrl);
            
    AbstractPage::updatePage();
}

int MessagesTableAlertWidget::addCustomButtonsToResourceTable(long long id, int rowTable, int columnTable)
{
    Wt::WText *ledButton = new Wt::WText();
    ledButton->setTextFormat(Wt::XHTMLUnsafeText);
    ledButton->setText("<div class='cercle" + getStateColor(m_alertsData[id].stateID) + "'></div>");
    getResourceTable()->elementAt(rowTable, columnTable)->addWidget(ledButton); 
    getResourceTable()->elementAt(rowTable, columnTable)->setWidth(Wt::WLength(Wt::WLength(5, Wt::WLength::Percentage)));
    getResourceTable()->elementAt(rowTable, columnTable)->setContentAlignment(Wt::AlignCenter);
    ++columnTable;
    
    Wt::WAnchor *forwardButton = new Wt::WAnchor();
    forwardButton->setTextFormat(Wt::XHTMLUnsafeText);
    forwardButton->setText("<span class='input-group-btn'><i class='icon-share-alt icon-white'></i></span>");
    if(m_alertsData[id].stateID == Echoes::Dbo::EAlertStatus::PENDING
            || m_alertsData[id].stateID == Echoes::Dbo::EAlertStatus::SUPPORTED)
    {
        forwardButton->clicked().connect(boost::bind(&MessagesTableAlertWidget::forward, this, id));
        forwardButton->setAttributeValue("class", "btn btn-info");
    }
    else
    {
        forwardButton->clicked().connect(boost::bind(&MessagesTableAlertWidget::messageBoxError, this));
        forwardButton->setAttributeValue("class", "btn btn-disabled");
    }
    getResourceTable()->elementAt(rowTable, columnTable)->addWidget(forwardButton); 
    getResourceTable()->elementAt(rowTable, columnTable)->setWidth(Wt::WLength(Wt::WLength(5, Wt::WLength::Percentage)));
    getResourceTable()->elementAt(rowTable, columnTable)->setContentAlignment(Wt::AlignCenter);
    ++columnTable;
    
    Wt::WAnchor *assignementButton = new Wt::WAnchor();
    assignementButton->setTextFormat(Wt::XHTMLUnsafeText);
    assignementButton->setText("<span class='input-group-btn'><i class='icon-inbox icon-white'></i></span>");
    if(m_alertsData[id].stateID == Echoes::Dbo::EAlertStatus::PENDING
            || m_alertsData[id].stateID == Echoes::Dbo::EAlertStatus::FORWARDING)
    {
        assignementButton->clicked().connect(boost::bind(&MessagesTableAlertWidget::takeAssignement, this, id));
        assignementButton->setAttributeValue("class", "btn btn-info");
    }
    else
    {
        assignementButton->clicked().connect(boost::bind(&MessagesTableAlertWidget::messageBoxError, this));
        assignementButton->setAttributeValue("class", "btn btn-disabled");
    }
    getResourceTable()->elementAt(rowTable, columnTable)->addWidget(assignementButton); 
    getResourceTable()->elementAt(rowTable, columnTable)->setWidth(Wt::WLength(Wt::WLength(5, Wt::WLength::Percentage)));
    getResourceTable()->elementAt(rowTable, columnTable)->setContentAlignment(Wt::AlignCenter);
    ++columnTable;
    
    Wt::WAnchor *resolvedButton = new Wt::WAnchor();
    resolvedButton->setTextFormat(Wt::XHTMLUnsafeText);
    resolvedButton->setText("<span class='input-group-btn'><i class='icon-chevron-down icon-white'></i></span>");
    if(m_alertsData[id].stateID == Echoes::Dbo::EAlertStatus::SUPPORTED)
    {
        resolvedButton->clicked().connect(boost::bind(&MessagesTableAlertWidget::setResolved, this, id));
        resolvedButton->setAttributeValue("class", "btn btn-info");
    }
    else
    {
        resolvedButton->clicked().connect(boost::bind(&MessagesTableAlertWidget::messageBoxError, this));
        resolvedButton->setAttributeValue("class", "btn btn-disabled");
    }
    getResourceTable()->elementAt(rowTable, columnTable)->addWidget(resolvedButton); 
    getResourceTable()->elementAt(rowTable, columnTable)->setWidth(Wt::WLength(Wt::WLength(5, Wt::WLength::Percentage)));
    getResourceTable()->elementAt(rowTable, columnTable)->setContentAlignment(Wt::AlignCenter);
    return ++columnTable;
}

void MessagesTableAlertWidget::customButtonFooter(Wt::WDialog *dialog)
{
    Wt::WPushButton *forwardButton = new Wt::WPushButton(tr("Alert." + m_xmlPageName + ".button-forward"),
                                                          dialog->footer());
    if (getFooterOkButtonStatus())
    {
        forwardButton->clicked().connect(dialog, &Wt::WDialog::accept);
        forwardButton->setAttributeValue("style", "margin-left:12px");
        forwardButton->setObjectName("forward-button");
    }
    else
    {
        forwardButton->disable();
    }  
}

void MessagesTableAlertWidget::popupForward(std::vector<Wt::WInteractWidget*>* inputName, std::vector<Wt::WText*> errorMessage, Wt::WDialog *dialog, long long id)
{
    if (dialog->result() == Wt::WDialog::Rejected)
    {
        return;
    }
    Wt::Http::Message content;
    content.addBodyText("{\n\t\"user\": " + boost::lexical_cast<string>(m_session->user().id()) + ", \n"
                         + "\"userForward\": " + usersModel_->item(((Wt::WComboBox*)(*inputName->begin()))->currentIndex(), 1)->text().toUTF8()
                         + "\n}");

    const string apiAddress = getApiUrl() + "/alerts/"
                        + boost::lexical_cast<string>(id)
                        + "/forward"
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
    
    return;    
}


void MessagesTableAlertWidget::forward(long long id)
{
    vector<Wt::WText*> errorMessage;
    vector<Wt::WInteractWidget*>* inputName = new vector<Wt::WInteractWidget*>();
    Wt::WDialog *dialogForward = new Wt::WDialog(tr("Alert." + m_xmlPageName + ".popup-selectUser"));
    dialogForward->setMinimumSize(Wt::WLength(300), Wt::WLength::Auto);
    new Wt::WText(tr("Alert." + m_xmlPageName + ".name-name") + " : <br />", dialogForward->contents());
    Wt::WComboBox *comboBox = new Wt::WComboBox(dialogForward->contents());
    comboBox->setModel(usersModel_);
    comboBox->setCurrentIndex(0);
    inputName->push_back(comboBox);
            
    Wt::WText *error = new Wt::WText(tr("Alert." + m_xmlPageName + ".invalid-name-name"),
                                             dialogForward->contents());
    error->hide();
    errorMessage.push_back(error);

    new Wt::WText("<br />", dialogForward->contents());
            
    dialogForward->setResizable(true);
    dialogForward->setClosable(true);

    new Wt::WText("\n", dialogForward->contents());

    customButtonFooter(dialogForward);
    cancelButtonFooter(dialogForward);

    dialogForward->finished().connect(bind([ = ] ()
    {
        popupForward(inputName, errorMessage, dialogForward, id);
        return;
    }));
    dialogForward->show();
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

void MessagesTableAlertWidget::messageBoxError()
{
    Wt::WMessageBox::show(tr("Alert." + m_xmlPageName + ".ope-failed-title"),
                 tr("Alert." + m_xmlPageName + ".ope-failed"), Wt::Ok);
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

void MessagesTableAlertWidget::refresh()
{
    if (!Wt::WApplication::instance()->internalPathMatches("/messages"))
    {
        m_autoUpdate = true;
        timer_refresh->stop();
        return;
    }
    Wt::WApplication::instance()->triggerUpdate();
}

void MessagesTableAlertWidget::handleJsonGet(vectors_Json jsonResources)
{
    m_rowsTable.clear();
    
    vector<Wt::Json::Value> jsonResource;
    try
    {
        if(jsonResources.size() > 0)
        {
            jsonResource = jsonResources.at(0);
            if (jsonResource.size() > 0 && !jsonResource.at(0).isNull())
            {
                bool selectedIDExist = false;
                Wt::Json::Array& jsonArray = jsonResource.at(0);
                for (int cpt(0); cpt < (int) jsonArray.size(); cpt++)
                {
                    Wt::Json::Object jsonObject = jsonArray.at(cpt);
                    long long id = jsonObject.get("id");
                    selectedIDExist |= id == m_selectedID;                    
                    vector<Wt::WInteractWidget *> widgetsToAdd = initRowWidgets(jsonObject, jsonResource, cpt);
                    if(!widgetsToAdd.empty())
                    {
                        m_rowsTable.insert(make_pair(id, widgetsToAdd));
                    }
                }
                if(!selectedIDExist)
                    m_selectedID = 0;
            }
        }
        if(jsonResources.size() > 1)
        {
            jsonResource = jsonResources.at(1);
            if (jsonResource.size() > 0)
            {
                Wt::Json::Array& jsonArray = (*jsonResource.begin());
                if (!jsonArray.empty())
                {
                    usersModel_->clear();
                    int numRow = 0;
                    for (int cpt(0); cpt < (int) jsonArray.size(); cpt++)
                    {
                        Wt::Json::Object jsonUser = jsonArray.at(cpt);
                        long long userId = jsonUser.get("id");
                        string userFirstName = jsonUser.get("first_name");
                        string userLastName = jsonUser.get("last_name");
                        Wt::WStandardItem *itemId = new Wt::WStandardItem();
                        Wt::WStandardItem *itemName = new Wt::WStandardItem();
                        vector<Wt::WStandardItem*> rowVector;

                        itemId->setText(boost::lexical_cast<string>(userId));
                        itemName->setText(Wt::WString::fromUTF8((userFirstName + " " + userLastName)));
                        rowVector.push_back(itemName);
                        rowVector.push_back(itemId);
                        if(userId != m_session->user().id())
                        {
                            usersModel_->insertRow(numRow++, rowVector);
                            m_listUsr.insert(std::make_pair(userId, (userFirstName + " " + userLastName))); 
                        }
                    }
                }
            }
        }
    }
    catch (Wt::Json::ParseError const& e)
    {
        Wt::log("warning") << "[AbstractPage] Problems parsing JSON";
        Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
    }
    catch (Wt::Json::TypeException const& e)
    {
        
        Wt::log("warning") << "[AbstractPage] JSON Type Exception" << boost::lexical_cast<string>(e.what());
        //            Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
    }
    fillTable();
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

Wt::WString MessagesTableAlertWidget::getStateColor(long long stateID)
{
    map<long long, Wt::WString> mapStateColor;
    
    mapStateColor[Echoes::Dbo::EAlertStatus::BACKTONORMAL] = "Vert";
    mapStateColor[Echoes::Dbo::EAlertStatus::FORWARDING] = "Orange";
    mapStateColor[Echoes::Dbo::EAlertStatus::PENDING] = "Rouge";
    mapStateColor[Echoes::Dbo::EAlertStatus::SUPPORTED] = "Bleu";
    
    return mapStateColor[stateID];
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
