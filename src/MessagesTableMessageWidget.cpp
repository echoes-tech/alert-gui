/**
 * @file MessagesTableMessageWidget.cpp
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

#include "MessagesTableMessageWidget.h"

using namespace std;

MessagesTableMessageWidget::MessagesTableMessageWidget(Echoes::Dbo::Session *session, string apiUrl,
        MessagesTableAlertWidget* alertWidget)
: AbstractPage(session, apiUrl, "messages-message", true)
{
    setButtonModif(false);
    setButtonSup(false);
    m_hasAddButton = false;
    
    std::vector<std::pair <int, string>> titles;
    titles.push_back(make_pair(setValidatorType(ETypeJson::text, 0, EMandatory::isMandatory), "dest"));
    titles.push_back(make_pair(setValidatorType(ETypeJson::text, 0, EMandatory::isMandatory), "state"));
    titles.push_back(make_pair(setValidatorType(ETypeJson::text, 0, EMandatory::isMandatory), "date"));
    setTitles(titles);
    
    
    m_messagesTableAlertWidget = alertWidget;
    m_selectedAlertID = 0;
}

Wt::WContainerWidget *MessagesTableMessageWidget::createTableFirstHeader()
{
    Wt::WContainerWidget *headerTableContainer = new Wt::WContainerWidget();
    headerTableContainer->addStyleClass("widget-title header-pers");
    
    Wt::WText *nameMainTable = new Wt::WText("<span class='icon'><i class='icon-tasks'></i></span><h5>"
                  + tr("Alert." + m_xmlPageName + ".main-table") + m_messagesTableAlertWidget->getSelectedAlertName()
                  + "</h5>", headerTableContainer);
    nameMainTable->setObjectName(m_xmlPageName + "-main-table");

    if (m_hasAddButton)
    {
        Wt::WAnchor *headerButton = new Wt::WAnchor(headerTableContainer);

        addPopupAddHandler(headerButton);
        headerButton->setStyleClass("button-add btn");
        headerButton->setText("<span class='btn-pink'><i class='icon-plus'></i></span>");
        headerButton->setObjectName("add-button");
    }
    return headerTableContainer;
}

void MessagesTableMessageWidget::updatePage()
{
    if(m_selectedAlertID != m_messagesTableAlertWidget->getSelectedID())
    {
        m_selectedAlertID = m_messagesTableAlertWidget->getSelectedID();
        setSelectedID(0);
    }     
    
    list<list<pair<string, vector<string>>>> listsUrl;
    if(m_messagesTableAlertWidget->getSelectedID() != 0)
    {
        list<pair<string, vector<string>>> listUrl;
        vector<string> listParameter;
        
        listParameter.push_back("alert_id=" + boost::lexical_cast<string>(m_messagesTableAlertWidget->getSelectedID()));
        listUrl.push_back(pair<string, vector<string>>("messages", listParameter));
        listParameter.clear();
        listUrl.push_back(pair<string, vector<string>>("messages/:id/status", listParameter));
        
        listsUrl.push_back(listUrl);
        listUrl.clear();
    }
    setUrl(listsUrl);
    
    AbstractPage::updatePage();
}

long long MessagesTableMessageWidget::getSelectedMessageAddonID()
{
    return m_messagesData[getSelectedID()].messageID;
}

Wt::WString MessagesTableMessageWidget::getSelectedMessageRecipient()
{
    return m_messagesData[getSelectedID()].dest;
}

vector<Wt::WInteractWidget *> MessagesTableMessageWidget::initRowWidgets(Wt::Json::Object jsonObject, vector<Wt::Json::Value> jsonResource, int cpt)
{    
    if(cpt == 0)
        m_messagesData.clear();
    
    struct MessageData messageData;
        
    vector<Wt::WInteractWidget *> rowWidgets;
    try
    {
        Wt::Json::Object messageStatusInfo = jsonResource.at(cpt+1);
        
        int messageID = messageStatusInfo.get("message_id");
        messageData.messageID = messageID;
        
        Wt::WString dest = messageStatusInfo.get("dest");
        rowWidgets.push_back(new Wt::WText(dest));
        messageData.dest = dest;
        
        int stateID = messageStatusInfo.get("state");    
        rowWidgets.push_back(new Wt::WText(getStateName(stateID)));
        messageData.stateID = stateID;

        Wt::WString date = messageStatusInfo.get("date");
        rowWidgets.push_back(new Wt::WText(date));
        messageData.date = date;
        

        m_messagesData[messageID] = messageData;
    }
    catch (Wt::Json::ParseError const& e)
    {
        Wt::log("warning") << "[MessagesTableMessageWidget] Problems parsing JSON";
        Wt::WMessageBox::show(tr("Alert.messages-message.database-error-title"), tr("Alert.messages-message.database-error"), Wt::Ok);
    }
    catch (Wt::Json::TypeException const& e)
    {
        Wt::log("warning") << "[MessagesTableMessageWidget] JSON Type Exception";
        //Wt::WMessageBox::show(tr("Alert.messages-message.database-error-title"), tr("Alert.messages-message.database-error"), Wt::Ok);
    }
    
    return rowWidgets;
}
/*
Wt::WString MessagesTableMessageWidget::getSourceParameterName(long long sourceParameterID)
{    
    map<long long, Wt::WString> mapSourceParameterName;
    mapSourceParameterName[Echoes::Dbo::ESourceParameter::CONNECTION_STRING] = tr("Alert.plugins-source.connection-string");
    mapSourceParameterName[Echoes::Dbo::ESourceParameter::COMMUNITY] = tr("Alert.plugins-source.community");
    mapSourceParameterName[Echoes::Dbo::ESourceParameter::VERSION] = tr("Alert.plugins-source.version");
    mapSourceParameterName[Echoes::Dbo::ESourceParameter::AUTH_PROTO] = tr("Alert.plugins-source.auth-proto");
    mapSourceParameterName[Echoes::Dbo::ESourceParameter::AUTH_PATH] = tr("Alert.plugins-source.auth-path");
    mapSourceParameterName[Echoes::Dbo::ESourceParameter::PRIV_PROTO] = tr("Alert.plugins-source.priv-proto");
    mapSourceParameterName[Echoes::Dbo::ESourceParameter::PRIV_PATH] = tr("Alert.plugins-source.priv-path");
    mapSourceParameterName[Echoes::Dbo::ESourceParameter::HOST] = tr("Alert.plugins-source.host");
    mapSourceParameterName[Echoes::Dbo::ESourceParameter::USER] = tr("Alert.plugins-source.user");
    
    return mapSourceParameterName[sourceParameterID];
}
*/
Wt::WString MessagesTableMessageWidget::getStateName(long long stateID)
{    
    map<long long, Wt::WString> mapStateName;
    mapStateName[Echoes::Dbo::EMessageStatus::UNCREATED] = tr("Alert.messages-message.uncreated");
    mapStateName[Echoes::Dbo::EMessageStatus::CREATED] = tr("Alert.messages-message.created");
    mapStateName[Echoes::Dbo::EMessageStatus::SENDED] = tr("Alert.messages-message.sended");
    mapStateName[Echoes::Dbo::EMessageStatus::SENDFAILED] = tr("Alert.messages-message.sendfailed");
    mapStateName[Echoes::Dbo::EMessageStatus::SENDREFUSED] = tr("Alert.messages-message.sendrefused");
    mapStateName[Echoes::Dbo::EMessageStatus::ACKFAILED] = tr("Alert.messages-message.ackfailed");
    mapStateName[Echoes::Dbo::EMessageStatus::ANSWERED] = tr("Alert.messages-message.answered");
    mapStateName[Echoes::Dbo::EMessageStatus::RECEIVED] = tr("Alert.messages-message.received");
    mapStateName[Echoes::Dbo::EMessageStatus::LINKING] = tr("Alert.messages-message.linking");
    mapStateName[Echoes::Dbo::EMessageStatus::LINKLOST] = tr("Alert.messages-message.linklost");
    
    return mapStateName[stateID];
}
/*
void MessagesTableMessageWidget::addPopupAddHandler(Wt::WInteractWidget* widget)
{
    widget->clicked().connect(boost::bind(&MessagesTableMessageWidget::addResourcePopup, this, 0));
}

void MessagesTableMessageWidget::addPopupModifHandler(Wt::WInteractWidget* widget, long long sourceID)
{
    widget->clicked().connect(boost::bind(&MessagesTableMessageWidget::addResourcePopup, this, sourceID));
}

void MessagesTableMessageWidget::addResourcePopup(long long sourceID)
{
    vector<Wt::WInteractWidget*>* inputName = new vector<Wt::WInteractWidget*>();
    vector<Wt::WText*> errorMessage;
    
    std::string xmlPopup;
    if (sourceID == 0)
    {
        xmlPopup = "Alert.messages-source.popup-add";
    }
    else
    {
        xmlPopup = "Alert.messages-source.popup-modif";
    }
    Wt::WDialog *dialog = new Wt::WDialog(tr(xmlPopup));
    
    dialog->setMinimumSize(Wt::WLength(300), Wt::WLength::Auto);
    
    Wt::WContainerWidget* paramsContainer = new Wt::WContainerWidget();    
    boost::function<void (Wt::Json::Value)> functor = boost::bind(&MessagesTableMessageWidget::handleRequestPopupAdd, this, _1, paramsContainer, inputName, sourceID);
    
    Wt::WComboBox* addonComboBox = new Wt::WComboBox();
    addonComboBox->setObjectName("addonComboBox");
    if(sourceID == 0)
    {
        dialog->contents()->addWidget(addonComboBox);
        inputName->push_back(addonComboBox);
        addonComboBox->setModel(m_addonStandardItemModel);
        addonComboBox->setCurrentIndex(0);
        addonComboBox->changed().connect(boost::bind(&MessagesTableMessageWidget::sendRequestPopupAdd, this, functor, addonComboBox, sourceID));            
        new Wt::WText("<br />", dialog->contents());
    }
    
    dialog->contents()->addWidget(paramsContainer);
    
    sendRequestPopupAdd(functor, addonComboBox, sourceID);
    
    popupFinalization(dialog, 0);    

    dialog->finished().connect(boost::bind(&MessagesTableMessageWidget::popupCheck, this, inputName, errorMessage, dialog, sourceID));
    
    dialog->show();
}

void MessagesTableMessageWidget::sendRequestPopupAdd(boost::function<void (Wt::Json::Value)> functor, Wt::WComboBox* addonComboBox, long long sourceID)
{    
    vector<string> parameterList;
    if(sourceID == 0)
    {
        parameterList.push_back("&addon_id=" + m_addonStandardItemModel->item((addonComboBox->currentIndex() == -1 ? 0 : addonComboBox->currentIndex()), 1)->text().toUTF8());
    }
    else 
    {        
        parameterList.push_back("&addon_id=" + boost::lexical_cast<string>(m_sourcesData[sourceID].addonID));
    }
    sendHttpRequestGet("sources/parameters", parameterList, functor);
}

void MessagesTableMessageWidget::handleRequestPopupAdd(Wt::Json::Value result, Wt::WContainerWidget* paramsContainer,
                                     vector<Wt::WInteractWidget*>* inputName, long long sourceID)
{
    paramsContainer->clear();    
    
    if(sourceID == 0)
    {
        vector<Wt::WInteractWidget*>::iterator it = inputName->begin();
        it++;
        while(it != inputName->end())
        {
            it = inputName->erase(it);
        }
    }
    
    if(result.isNull())
    {
        return;
    }
        
    Wt::Json::Array& jsonArray = result;   
    
    for (int cpt(0); cpt < (int) jsonArray.size(); cpt++)
    {
        Wt::Json::Object jsonObject = jsonArray.at(cpt);
        
        long long sourceParameterID = jsonObject.get("id");
        new Wt::WText(getSourceParameterName(sourceParameterID) + " : <br />", paramsContainer);
        
        Wt::WLineEdit* lineEdit = new Wt::WLineEdit(paramsContainer);
        if(sourceID > 0)
            lineEdit->setText(m_sourcesData[sourceID].parametersValue[sourceParameterID]);
        
        Wt::WString wsName = jsonObject.get("name");
        lineEdit->setAttributeValue("name", wsName);
        
        inputName->push_back(lineEdit);        
        
        new Wt::WText("<br />", paramsContainer);
    }
}*/

/*void MessagesTableMessageWidget::setAddResourceMessage(Wt::Http::Message *message,vector<Wt::WInteractWidget*>* argument)
{
    vector<Wt::WInteractWidget*>::iterator it = argument->begin();
        
    message->addBodyText("{");
    message->addBodyText("\n\"plugin_id\": " + boost::lexical_cast<string>(m_pluginsTablePluginWidget->getSelectedID()));
    message->addBodyText(",\n\"addon_id\": " + m_addonStandardItemModel->item(((Wt::WComboBox*)(*it++))->currentIndex(), 1)->text().toUTF8());
    
    while(it != argument->end())
    {
        string name = ((Wt::WLineEdit*)(*it))->attributeValue("name").toUTF8();
        string value = ((Wt::WLineEdit*)(*it++))->text().toUTF8();
        message->addBodyText(",\n\"" + name + "\": \"" + value + "\"");
    }
    
    message->addBodyText("\n}");
}

void MessagesTableMessageWidget::setModifResourceMessage(Wt::Http::Message *message,vector<Wt::WInteractWidget*>* argument)
{
    vector<Wt::WInteractWidget*>::iterator it = argument->begin();
        
    message->addBodyText("{");
    
    while(it != argument->end())
    {
        if(it != argument->begin())
        {
            message->addBodyText(",");
        }
        
        string name = ((Wt::WLineEdit*)(*it))->attributeValue("name").toUTF8();
        string value = ((Wt::WLineEdit*)(*it++))->text().toUTF8();
        message->addBodyText("\n\"" + name + "\": \"" + value + "\"");
    }
    
    message->addBodyText("\n}");
}*/
