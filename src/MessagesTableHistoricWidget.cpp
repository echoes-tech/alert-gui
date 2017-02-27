/**
 * @file MessagesTableHistoricWidget.cpp
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

#include <boost/lexical_cast.hpp>

#include "MessagesTableHistoricWidget.h"

using namespace std;

MessagesTableHistoricWidget::MessagesTableHistoricWidget(Echoes::Dbo::Session *session, string apiUrl,
        MessagesTableMessageWidget* messageWidget)
: AbstractPage(session, apiUrl, "messages-historic")
{
    setButtonModif(false);
    setButtonSup(false);
    m_hasAddButton = false;
    
    std::vector<std::pair <int, std::string>> titles;
    titles.push_back(make_pair(0, "state"));
    titles.push_back(make_pair(0, "date"));
    setTitles(titles);
    
    m_searchTypeStandardItemModel = new Wt::WStandardItemModel(0,2,this);
    
    m_messagesTableMessageWidget = messageWidget;
    m_selectedSourceID = 0;
}

Wt::WContainerWidget *MessagesTableHistoricWidget::createTableFirstHeader()
{
    Wt::WContainerWidget *headerTableContainer = new Wt::WContainerWidget();
    headerTableContainer->addStyleClass("widget-title header-pers");
    
    Wt::WText *nameMainTable = new Wt::WText("<span class='icon'><i class='icon-tasks'></i></span><h5>"
                  + tr("Alert." + m_xmlPageName + ".main-table") + m_messagesTableMessageWidget->getSelectedMessageRecipient()
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

void MessagesTableHistoricWidget::updatePage()
{
    if(m_selectedSourceID != m_messagesTableMessageWidget->getSelectedID())
    {
        m_selectedSourceID = m_messagesTableMessageWidget->getSelectedID();
        setSelectedID(0);
    }    

    list<list<pair<string, vector<string>>>> listsUrl;
    if(m_messagesTableMessageWidget->getSelectedID() != 0)
    {
        list<pair<string, vector<string>>> listUrl;
        vector<string> listParameter;
        
        listUrl.push_back(pair<string, vector<string>>("messages/" + boost::lexical_cast<string>(m_messagesTableMessageWidget->getSelectedID()) + "/events", listParameter));
        //listUrl.push_back(pair<string, vector<string>>("messages/" + boost::lexical_cast<string>(m_messagesTableMessageWidget->getSelectedID()) + "/events/:id", listParameter));        
        listsUrl.push_back(listUrl);
        listUrl.clear();
    }
    setUrl(listsUrl);
    
    AbstractPage::updatePage();
}

vector<Wt::WInteractWidget *> MessagesTableHistoricWidget::initRowWidgets(Wt::Json::Object jsonObject, vector<Wt::Json::Value> jsonResource, int cpt)
{    
    if(cpt == 0)
    {
        m_messageHistoricData.clear();
    }
    
    struct HistoricData historicData;
        
    vector<Wt::WInteractWidget *> rowWidgets;
    
    
    try
    {
        Wt::Json::Array listHistoric = jsonResource.at(0);
        Wt::Json::Object historicInfo = (Wt::Json::Object)listHistoric[cpt];
        
        int messageHistoricID = historicInfo.get("id");
        
        int stateID = ((Wt::Json::Object)historicInfo.get("message_status")).get("id");    
        rowWidgets.push_back(new Wt::WText(getStateName(stateID)));
        historicData.stateID = stateID;

        Wt::WString date = historicInfo.get("date");
        Wt::WDateTime form = Wt::WDateTime::fromString(date);
        date = form.toString("dd/MM/yyyy HH:mm:ss");
        rowWidgets.push_back(new Wt::WText(date));
        historicData.date = date;
        

        m_messageHistoricData[messageHistoricID] = historicData;
    }
    catch (Wt::Json::ParseError const& e)
    {
        Wt::log("warning") << "[MessagesTableHistoricWidget] Problems parsing JSON";
        Wt::WMessageBox::show(tr("Alert.messages-historic.database-error-title"), tr("Alert.messages-historic.database-error"), Wt::Ok);
    }
    catch (Wt::Json::TypeException const& e)
    {
        Wt::log("warning") << "[MessagesTableHistoricWidget] JSON Type Exception";
        //            Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
    }
    
    return rowWidgets;
}

Wt::WString MessagesTableHistoricWidget::getStateName(long long stateID)
{    
    map<long long, Wt::WString> mapStateName;
    mapStateName[Echoes::Dbo::EMessageStatus::UNCREATED] = tr("Alert.messages-historic.uncreated");
    mapStateName[Echoes::Dbo::EMessageStatus::CREATED] = tr("Alert.messages-historic.created");
    mapStateName[Echoes::Dbo::EMessageStatus::SENDED] = tr("Alert.messages-historic.sended");
    mapStateName[Echoes::Dbo::EMessageStatus::SENDFAILED] = tr("Alert.messages-historic.sendfailed");
    mapStateName[Echoes::Dbo::EMessageStatus::SENDREFUSED] = tr("Alert.messages-historic.sendrefused");
    mapStateName[Echoes::Dbo::EMessageStatus::ACKFAILED] = tr("Alert.messages-historic.ackfailed");
    mapStateName[Echoes::Dbo::EMessageStatus::ANSWERED] = tr("Alert.messages-historic.answered");
    mapStateName[Echoes::Dbo::EMessageStatus::RECEIVED] = tr("Alert.messages-historic.received");
    mapStateName[Echoes::Dbo::EMessageStatus::LINKING] = tr("Alert.messages-historic.linking");
    mapStateName[Echoes::Dbo::EMessageStatus::LINKLOST] = tr("Alert.messages-historic.linklost");
    return mapStateName[stateID];
}
/*
Wt::WString MessagesTableHistoricWidget::getSearchTypeName(long long searchTypeID)
{    
    map<long long, Wt::WString> mapSearchTypeName;
    mapSearchTypeName[Echoes::Dbo::ESearchType::PATH] = tr("Alert.plugins-search.search-type.path");
    mapSearchTypeName[Echoes::Dbo::ESearchType::PATH_LINE] = tr("Alert.plugins-search.search-type.path-line");
    mapSearchTypeName[Echoes::Dbo::ESearchType::PATH_LAST_LINE] = tr("Alert.plugins-search.search-type.path-last-line");
    mapSearchTypeName[Echoes::Dbo::ESearchType::QUERY] = tr("Alert.plugins-search.search-type.query");
    mapSearchTypeName[Echoes::Dbo::ESearchType::OID] = tr("Alert.plugins-search.search-type.oid");
    mapSearchTypeName[Echoes::Dbo::ESearchType::PATH_ALGORITHM] = tr("Alert.plugins-search.search-type.path-algorithm");
    mapSearchTypeName[Echoes::Dbo::ESearchType::PATH_XPATH] = tr("Alert.plugins-search.search-type.path-xpath");
    mapSearchTypeName[Echoes::Dbo::ESearchType::NAME] = tr("Alert.plugins-search.search-type.name");
    
    return mapSearchTypeName[searchTypeID];
}

void MessagesTableHistoricWidget::addPopupAddHandler(Wt::WInteractWidget* widget)
{
    widget->clicked().connect(boost::bind(&MessagesTableHistoricWidget::addResourcePopup, this, 0));
}

void MessagesTableHistoricWidget::addPopupModifHandler(Wt::WInteractWidget* widget, long long searchID)
{
    widget->clicked().connect(boost::bind(&MessagesTableHistoricWidget::addResourcePopup, this, searchID));
}

void MessagesTableHistoricWidget::addResourcePopup(long long searchID)
{
    vector<Wt::WInteractWidget*>* inputName = new vector<Wt::WInteractWidget*>();
    vector<Wt::WText*> errorMessage;
    
    std::string xmlPopup;
    if (searchID == 0)
    {
        xmlPopup = "Alert.messages-historic.popup-add";
    }
    else
    {
        xmlPopup = "Alert.messages-historic.popup-modif";
    }
    Wt::WDialog *dialog = new Wt::WDialog(tr(xmlPopup));
    
    dialog->setMinimumSize(Wt::WLength(300), Wt::WLength::Auto);
             
    Wt::WContainerWidget* paramsContainer = new Wt::WContainerWidget();  
    boost::function<void (Wt::Json::Value)> functorHandleRequestPopupAdd = boost::bind(&MessagesTableHistoricWidget::handleRequestPopupAdd, this, _1, paramsContainer, inputName, searchID);    
    
    Wt::WComboBox* searchTypeComboBox = new Wt::WComboBox();            
    if(searchID == 0)
    {
        dialog->contents()->addWidget(searchTypeComboBox);
        inputName->push_back(searchTypeComboBox);
        searchTypeComboBox->setModel(m_searchTypeStandardItemModel);
        m_searchTypeStandardItemModel->clear();
        addEnumToModel(m_searchTypeStandardItemModel, 0, "");  
        new Wt::WText("<br />", dialog->contents());
        
        boost::function<void (Wt::Json::Value)> functorFillModel = boost::bind(&MessagesTableHistoricWidget::fillModel, this, _1, searchTypeComboBox, searchID, functorHandleRequestPopupAdd);    
        string resource = "addons/" + boost::lexical_cast<string>(m_messagesTableMessageWidget->getSelectedID()) + "/search_types";
        sendHttpRequestGet(resource, vector<string>(), functorFillModel);
    }
    else
    {
        sendRequestPopupAdd(functorHandleRequestPopupAdd, searchTypeComboBox, searchID);
    }  
        
    new Wt::WText(tr("Alert.messages-historic.add-period") + " : <br />", dialog->contents());    
    Wt::WLineEdit* periodLineEdit = new Wt::WLineEdit(dialog->contents()); 
    periodLineEdit->setAttributeValue("name", "period"); 
    if(searchID > 0)  
    {
        periodLineEdit->setText(boost::lexical_cast<string>(m_searchesData[searchID].period));
    }
    inputName->push_back(periodLineEdit); 
    new Wt::WText("<br />", dialog->contents()); 
    
    dialog->contents()->addWidget(paramsContainer);  
    
    popupFinalization(dialog, 0);    

    dialog->finished().connect(boost::bind(&MessagesTableHistoricWidget::popupCheck, this, inputName, errorMessage, dialog, searchID));
    
    dialog->show();
}

void MessagesTableHistoricWidget::sendRequestPopupAdd(boost::function<void (Wt::Json::Value)> functor, Wt::WComboBox* searchTypeComboBox, long long searchID)
{    
    vector<string> parameterList;
    if(searchID == 0)
    {
        parameterList.push_back("&type_id=" + m_searchTypeStandardItemModel->item((searchTypeComboBox->currentIndex() == -1 ? 0 : searchTypeComboBox->currentIndex()), 1)->text().toUTF8());
    }
    else 
    {        
        parameterList.push_back("&type_id=" + boost::lexical_cast<string>(m_searchesData[searchID].searchTypeID));
    }
          
    sendHttpRequestGet("searches/parameters", parameterList, functor);
}

void MessagesTableHistoricWidget::handleRequestPopupAdd(Wt::Json::Value result, Wt::WContainerWidget* paramsContainer,
                                     vector<Wt::WInteractWidget*>* inputName, long long searchID)
{
    paramsContainer->clear();
    
    vector<Wt::WInteractWidget*>::iterator it = inputName->begin();
    if(searchID == 0)
    {
        it+=2;
    }
    else
    {
        it++;
    }
    while(it != inputName->end())
    {
        it = inputName->erase(it);
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
        new Wt::WText(getSearchParameterName(sourceParameterID) + " : <br />", paramsContainer);
        
        Wt::WLineEdit* lineEdit = new Wt::WLineEdit(paramsContainer);
        if(searchID > 0)
        {
            lineEdit->setText(m_searchesData[searchID].parametersValue[sourceParameterID]);
        }
        
        Wt::WString wsName = jsonObject.get("name");
        lineEdit->setAttributeValue("name", wsName);
        
        inputName->push_back(lineEdit);        
        
        new Wt::WText("<br />", paramsContainer);
    }
}*/
