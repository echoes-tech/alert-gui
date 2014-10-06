/* 
 * GUI PluginsTableSearchWidget.cpp
 * 
 * @author ECHOES Technologies (MLA)
 * 
 * @date 11/07/2014
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS WITHOUT
 * COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012-2013 BY ECHOES TECHNOLGIES SAS
 * 
 */

#include <boost/lexical_cast.hpp>

#include "PluginsTableSearchWidget.h"

using namespace std;

PluginsTableSearchWidget::PluginsTableSearchWidget(Echoes::Dbo::Session *session, string apiUrl,
        PluginsTableSourceWidget* pluginsTableSourceWidget)
: AbstractPage(session, apiUrl, "plugins-search", true)
{
    setButtonModif(true);
    setButtonSup(true);
    
    std::vector<std::pair <int, std::string>> titles;
    titles.push_back(make_pair(0, "search-type"));
    titles.push_back(make_pair(0, "period"));
    titles.push_back(make_pair(0, "parameters"));
    setTitles(titles);
    
    m_searchTypeStandardItemModel = new Wt::WStandardItemModel(0,2,this);
    
    m_pluginsTableSourceWidget = pluginsTableSourceWidget;
    m_selectedSourceID = 0;
}

void PluginsTableSearchWidget::fillModel(Wt::Json::Value result, Wt::WComboBox* searchTypeComboBox, long long searchID,
    boost::function<void (Wt::Json::Value)> functorHandleRequestPopupAdd)
{
    m_searchTypeStandardItemModel->clear();
    
    if(result.isNull())
        return;
        
    Wt::Json::Array& jsonArray = result;   
    
    for (int cpt(0); cpt < (int) jsonArray.size(); cpt++)
    {
        Wt::Json::Object jsonObject = jsonArray.at(cpt);
        
        long long searchTypeID = jsonObject.get("id");
        addEnumToModel(m_searchTypeStandardItemModel, searchTypeID, getSearchTypeName(searchTypeID));
    }
    
    if(searchID > 0)
        for(int row(0); row < m_searchTypeStandardItemModel->rowCount(); row++)    
            if(boost::lexical_cast<long long>(m_searchTypeStandardItemModel->item(row, 1)->text()) == m_searchesData[searchID].searchTypeID)
                searchTypeComboBox->setCurrentIndex(row);
    
    searchTypeComboBox->changed().connect(boost::bind(&PluginsTableSearchWidget::sendRequestPopupAdd, this, functorHandleRequestPopupAdd, searchTypeComboBox, searchID));        
    sendRequestPopupAdd(functorHandleRequestPopupAdd, searchTypeComboBox, searchID);
}

void PluginsTableSearchWidget::updatePage()
{
    if(m_selectedSourceID != m_pluginsTableSourceWidget->getSelectedID())
    {
        m_selectedSourceID = m_pluginsTableSourceWidget->getSelectedID();
        setSelectedID(0);
    }    

    list<list<pair<string, vector<string>>>> listsUrl;
    if(m_pluginsTableSourceWidget->getSelectedID() != 0)
    {
        list<pair<string, vector<string>>> listUrl;
        vector<string> listParameter;
        
        listParameter.push_back("source_id=" + boost::lexical_cast<string>(m_pluginsTableSourceWidget->getSelectedID()));
        listUrl.push_back(pair<string, vector<string>>("searches", listParameter));        
        listParameter.clear();
        listUrl.push_back(pair<string, vector<string>>("searches/:id/parameters", listParameter));        
        listsUrl.push_back(listUrl);
        listUrl.clear();
    }
    setUrl(listsUrl);
    
    setAddButtonEnable(m_pluginsTableSourceWidget->getSelectedID() != 0);
    
    AbstractPage::updatePage();
}

vector<Wt::WInteractWidget *> PluginsTableSearchWidget::initRowWidgets(Wt::Json::Object jsonObject, vector<Wt::Json::Value> jsonResource, int cpt)
{    
    if(cpt == 0)
        m_searchesData.clear();
    
    struct SearchData searchData;
        
    vector<Wt::WInteractWidget *> rowWidgets;
    
    int searchID = jsonObject.get("id");
    
    int searchTypeID = ((Wt::Json::Object)jsonObject.get("search_type")).get("id");    
    rowWidgets.push_back(new Wt::WText(getSearchTypeName(searchTypeID)));
    
    searchData.searchTypeID = searchTypeID;  
    
    int period = jsonObject.get("period");
    rowWidgets.push_back(new Wt::WText(boost::lexical_cast<string>(period)));
        
    Wt::WString searchParametersString;        
    if(!((Wt::Json::Value)jsonResource.at(cpt+1)).isNull())
    {
        Wt::Json::Array& jsonArrayParameters = jsonResource.at(cpt+1);
        for (int cpt(0); cpt < (int) jsonArrayParameters.size(); cpt++)
        {
            Wt::Json::Object jsonObjectParameter = jsonArrayParameters.at(cpt);

            int searchParameterID = ((Wt::Json::Object)jsonObjectParameter.get("id")).get("search_parameter_id");
            Wt::WString searchParameterValue = jsonObjectParameter.get("value");

            searchParametersString += getSearchParameterName(searchParameterID);
            searchParametersString += ": ";
            searchParametersString += searchParameterValue;
            if(cpt != (int) jsonArrayParameters.size() - 1)
                searchParametersString += "<br />";
            
            searchData.parametersValue[searchParameterID] = searchParameterValue;
        }
    }        
    Wt::WContainerWidget* containerWidget = new Wt::WContainerWidget();
    containerWidget->addWidget(new Wt::WText(searchParametersString));
    containerWidget->setContentAlignment(Wt::AlignmentFlag::AlignLeft);
    rowWidgets.push_back(containerWidget);      
    
    searchData.period = period;
    
    m_searchesData[searchID] = searchData;
    
    return rowWidgets;
}

Wt::WString PluginsTableSearchWidget::getSearchParameterName(long long searchParameterID)
{    
    map<long long, Wt::WString> mapSearchParameterName;
    mapSearchParameterName[Echoes::Dbo::ESearchParameter::PATH] = tr("Alert.plugins-search.search-parameter.path");
    mapSearchParameterName[Echoes::Dbo::ESearchParameter::LINE] = tr("Alert.plugins-search.search-parameter.line");
    mapSearchParameterName[Echoes::Dbo::ESearchParameter::LAST_LINE] = tr("Alert.plugins-search.search-parameter.last-line");
    mapSearchParameterName[Echoes::Dbo::ESearchParameter::QUERY] = tr("Alert.plugins-search.search-parameter.query");
    mapSearchParameterName[Echoes::Dbo::ESearchParameter::OID] = tr("Alert.plugins-search.search-parameter.oid");
    mapSearchParameterName[Echoes::Dbo::ESearchParameter::ALGORITHM] = tr("Alert.plugins-search.search-parameter.algorithm");
    mapSearchParameterName[Echoes::Dbo::ESearchParameter::XPATH] = tr("Alert.plugins-search.search-parameter.xpath");
    mapSearchParameterName[Echoes::Dbo::ESearchParameter::NAME] = tr("Alert.plugins-search.search-parameter.name");
    
    return mapSearchParameterName[searchParameterID];
}

Wt::WString PluginsTableSearchWidget::getSearchTypeName(long long searchTypeID)
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

void PluginsTableSearchWidget::addPopupAddHandler(Wt::WInteractWidget* widget)
{
    widget->clicked().connect(boost::bind(&PluginsTableSearchWidget::addResourcePopup, this, 0));
}

void PluginsTableSearchWidget::addPopupModifHandler(Wt::WInteractWidget* widget, long long searchID)
{
    widget->clicked().connect(boost::bind(&PluginsTableSearchWidget::addResourcePopup, this, searchID));
}

void PluginsTableSearchWidget::addResourcePopup(long long searchID)
{
    vector<Wt::WInteractWidget*>* inputName = new vector<Wt::WInteractWidget*>();
    vector<Wt::WText*> errorMessage;

    Wt::WDialog *dialog = new Wt::WDialog(tr("Alert.plugins-search.add-plugins-search"));
    dialog->setMinimumSize(Wt::WLength(300), Wt::WLength::Auto);
             
    Wt::WContainerWidget* paramsContainer = new Wt::WContainerWidget();  
    boost::function<void (Wt::Json::Value)> functorHandleRequestPopupAdd = boost::bind(&PluginsTableSearchWidget::handleRequestPopupAdd, this, _1, paramsContainer, inputName, searchID);    
    
    Wt::WComboBox* searchTypeComboBox = new Wt::WComboBox();            
    if(searchID == 0)
    {
        dialog->contents()->addWidget(searchTypeComboBox);
        inputName->push_back(searchTypeComboBox);
        searchTypeComboBox->setModel(m_searchTypeStandardItemModel);
        m_searchTypeStandardItemModel->clear();
        addEnumToModel(m_searchTypeStandardItemModel, 0, "");  
        new Wt::WText("<br />", dialog->contents());
        
        boost::function<void (Wt::Json::Value)> functorFillModel = boost::bind(&PluginsTableSearchWidget::fillModel, this, _1, searchTypeComboBox, searchID, functorHandleRequestPopupAdd);    
        string resource = "addons/" + boost::lexical_cast<string>(m_pluginsTableSourceWidget->getSelectedSourceAddonID()) + "/search_types";
        sendHttpRequestGet(resource, vector<string>(), functorFillModel);
    }
    else
    {
        sendRequestPopupAdd(functorHandleRequestPopupAdd, searchTypeComboBox, searchID);
    }  
        
    new Wt::WText(tr("Alert.plugins-search.add-period") + " : <br />", dialog->contents());    
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

    dialog->finished().connect(boost::bind(&PluginsTableSearchWidget::popupCheck, this, inputName, errorMessage, dialog, searchID));
    
    dialog->show();
}

void PluginsTableSearchWidget::sendRequestPopupAdd(boost::function<void (Wt::Json::Value)> functor, Wt::WComboBox* searchTypeComboBox, long long searchID)
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

void PluginsTableSearchWidget::handleRequestPopupAdd(Wt::Json::Value result, Wt::WContainerWidget* paramsContainer,
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
            lineEdit->setText(m_searchesData[searchID].parametersValue[sourceParameterID]);
        
        Wt::WString wsName = jsonObject.get("name");
        lineEdit->setAttributeValue("name", wsName);
        
        inputName->push_back(lineEdit);        
        
        new Wt::WText("<br />", paramsContainer);
    }
}

void PluginsTableSearchWidget::setAddResourceMessage(Wt::Http::Message *message,vector<Wt::WInteractWidget*>* argument)
{    
    vector<Wt::WInteractWidget*>::iterator it = argument->begin();
        
    message->addBodyText("{");
    message->addBodyText("\n\"source_id\": " + boost::lexical_cast<string>(m_pluginsTableSourceWidget->getSelectedID()));
    message->addBodyText(",\n\"type_id\": " + m_searchTypeStandardItemModel->item(((Wt::WComboBox*)(*it++))->currentIndex(), 1)->text().toUTF8());
    
    while(it != argument->end())
    {
        string name = ((Wt::WLineEdit*)(*it))->attributeValue("name").toUTF8();
        string value = ((Wt::WLineEdit*)(*it++))->text().toUTF8();
        message->addBodyText(",\n\"" + name + "\": \"" + value + "\"");
    }
    
    message->addBodyText("\n}");
}

void PluginsTableSearchWidget::setModifResourceMessage(Wt::Http::Message *message,vector<Wt::WInteractWidget*>* argument)
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
}