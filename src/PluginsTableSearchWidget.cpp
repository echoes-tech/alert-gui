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
        AbstractPage* abstractPage)
: AbstractPage(session, apiUrl, "plugins-search", true)
{
    setButtonModif(true);
    setButtonSup(true);
    
    multimap<int, string> titles;
    titles.insert(make_pair(0, "search-type"));
    titles.insert(make_pair(0, "parameters"));
    titles.insert(make_pair(0, "period"));
    setTitles(titles);
    
    m_addonStandardItemModel = new Wt::WStandardItemModel(0,2,this);
    fillModel();
    
    m_pluginsTableSourceWidget = abstractPage;
    m_selectedSourceID = 0;
}

void PluginsTableSearchWidget::fillModel()
{
    m_addonStandardItemModel->clear();
     addEnumToModel(m_addonStandardItemModel, Echoes::Dbo::ESearchType::PATH, getSearchTypeName(Echoes::Dbo::ESearchType::PATH));
     addEnumToModel(m_addonStandardItemModel, Echoes::Dbo::ESearchType::PATH_LINE, getSearchTypeName(Echoes::Dbo::ESearchType::PATH_LINE));
    addEnumToModel(m_addonStandardItemModel, Echoes::Dbo::ESearchType::PATH_LAST_LINE, getSearchTypeName(Echoes::Dbo::ESearchType::PATH_LAST_LINE));
    addEnumToModel(m_addonStandardItemModel, Echoes::Dbo::ESearchType::QUERY, getSearchTypeName(Echoes::Dbo::ESearchType::QUERY));
    addEnumToModel(m_addonStandardItemModel, Echoes::Dbo::ESearchType::OID, getSearchTypeName(Echoes::Dbo::ESearchType::OID));
    addEnumToModel(m_addonStandardItemModel, Echoes::Dbo::ESearchType::PATH_ALGORITHM, getSearchTypeName(Echoes::Dbo::ESearchType::PATH_ALGORITHM));
    addEnumToModel(m_addonStandardItemModel, Echoes::Dbo::ESearchType::PATH_XPATH, getSearchTypeName(Echoes::Dbo::ESearchType::PATH_XPATH));
    addEnumToModel(m_addonStandardItemModel, Echoes::Dbo::ESearchType::NAME, getSearchTypeName(Echoes::Dbo::ESearchType::NAME));
}

void PluginsTableSearchWidget::updatePage()
{
    if(m_selectedSourceID != m_pluginsTableSourceWidget->getSelectedID())
    {
        m_selectedSourceID = m_pluginsTableSourceWidget->getSelectedID();
        setSelectedID(0);
    }    

    std::list<std::list<std::string>> listsUrl;
    if(m_pluginsTableSourceWidget->getSelectedID() != 0)
    {
        std::list<std::string> listUrl;
        listUrl.push_back("searches");
        listUrl.push_back("searches/:id/parameters");
        listsUrl.push_back(listUrl);
        listUrl.clear();
    }
    setUrl(listsUrl);
    
    AbstractPage::updatePage();
}

string PluginsTableSearchWidget::addParameter()
{
    return "&source_id=" + boost::lexical_cast<string>(m_pluginsTableSourceWidget->getSelectedID());
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
        
    Wt::WString sourceParametersString;
        
    if(!((Wt::Json::Value)jsonResource.at(cpt+1)).isNull())
    {
        Wt::Json::Array& jsonArrayParameters = jsonResource.at(cpt+1);
        for (int cpt(0); cpt < (int) jsonArrayParameters.size(); cpt++)
        {
            Wt::Json::Object jsonObjectParameter = jsonArrayParameters.at(cpt);

            int searchParameterID = ((Wt::Json::Object)jsonObjectParameter.get("id")).get("search_parameter_id");
            Wt::WString sourceParameterValue = jsonObjectParameter.get("value");

            sourceParametersString += getSearchParameterName(searchParameterID);
            sourceParametersString += ": ";
            sourceParametersString += sourceParameterValue;
            if(cpt != (int) jsonArrayParameters.size() - 1)
                sourceParametersString += "<br />";
            
            searchData.parametersValue[searchParameterID] = sourceParameterValue;
        }
    }
        
    Wt::WContainerWidget* containerWidget = new Wt::WContainerWidget();
    containerWidget->addWidget(new Wt::WText(sourceParametersString));
    containerWidget->setContentAlignment(Wt::AlignmentFlag::AlignLeft);
    rowWidgets.push_back(containerWidget);        
    
    int period = jsonObject.get("period");
    rowWidgets.push_back(new Wt::WText(boost::lexical_cast<string>(period)));
    
    searchData.period = period;
    
    m_searchesData[searchID] = searchData;
    
    return rowWidgets;
}

Wt::WString PluginsTableSearchWidget::getSearchParameterName(long long searchParameterID)
{    
    map<long long, Wt::WString> mapSourceParameterName;
    mapSourceParameterName[Echoes::Dbo::ESearchParameter::PATH] = tr("Alert.plugins-search.search-parameter.path");
    mapSourceParameterName[Echoes::Dbo::ESearchParameter::LINE] = tr("Alert.plugins-search.search-parameter.line");
    mapSourceParameterName[Echoes::Dbo::ESearchParameter::LAST_LINE] = tr("Alert.plugins-search.search-parameter.last-line");
    mapSourceParameterName[Echoes::Dbo::ESearchParameter::QUERY] = tr("Alert.plugins-search.search-parameter.query");
    mapSourceParameterName[Echoes::Dbo::ESearchParameter::OID] = tr("Alert.plugins-search.search-parameter.oid");
    mapSourceParameterName[Echoes::Dbo::ESearchParameter::ALGORITHM] = tr("Alert.plugins-search.search-parameter.algorithm");
    mapSourceParameterName[Echoes::Dbo::ESearchParameter::XPATH] = tr("Alert.plugins-search.search-parameter.xpath");
    mapSourceParameterName[Echoes::Dbo::ESearchParameter::NAME] = tr("Alert.plugins-search.search-parameter.name");
    
    return mapSourceParameterName[searchParameterID];
}

Wt::WString PluginsTableSearchWidget::getSearchTypeName(long long searchTypeID)
{    
    map<long long, Wt::WString> mapAddonName;
    mapAddonName[Echoes::Dbo::ESearchType::PATH] = tr("Alert.plugins-search.search-type.path");
    mapAddonName[Echoes::Dbo::ESearchType::PATH_LINE] = tr("Alert.plugins-search.search-type.path-line");
    mapAddonName[Echoes::Dbo::ESearchType::PATH_LAST_LINE] = tr("Alert.plugins-search.search-type.path-last-line");
    mapAddonName[Echoes::Dbo::ESearchType::QUERY] = tr("Alert.plugins-search.search-type.query");
    mapAddonName[Echoes::Dbo::ESearchType::OID] = tr("Alert.plugins-search.search-type.oid");
    mapAddonName[Echoes::Dbo::ESearchType::PATH_ALGORITHM] = tr("Alert.plugins-search.search-type.path-algorithm");
    mapAddonName[Echoes::Dbo::ESearchType::PATH_XPATH] = tr("Alert.plugins-search.search-type.path-xpath");
    mapAddonName[Echoes::Dbo::ESearchType::NAME] = tr("Alert.plugins-search.search-type.name");
    
    return mapAddonName[searchTypeID];
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
        
    Wt::WComboBox* searchTypeComboBox = new Wt::WComboBox(dialog->contents());
    searchTypeComboBox->setModel(m_addonStandardItemModel);    
    if(searchID > 0)
        for(int row(0); row < m_addonStandardItemModel->rowCount(); row++)    
            if(boost::lexical_cast<long long>(m_addonStandardItemModel->item(row, 1)->text()) == m_searchesData[searchID].searchTypeID)
                searchTypeComboBox->setCurrentIndex(row);
    inputName->push_back(searchTypeComboBox);
    new Wt::WText("<br />", dialog->contents());
        
    new Wt::WText(tr("Alert.plugins-search.add-period") + " : <br />", dialog->contents());    
    Wt::WLineEdit* periodLineEdit = new Wt::WLineEdit(dialog->contents());  
    if(searchID > 0)  
        periodLineEdit->setText(boost::lexical_cast<string>(m_searchesData[searchID].period));
    inputName->push_back(periodLineEdit); 
    new Wt::WText("<br />", dialog->contents());
    
    Wt::WContainerWidget* paramsContainer = new Wt::WContainerWidget(dialog->contents());     
    boost::function<void (Wt::Json::Value)> functor = boost::bind(&PluginsTableSearchWidget::handleRequestPopupAdd, this, _1, paramsContainer, inputName, searchID);    
    searchTypeComboBox->changed().connect(boost::bind(&PluginsTableSearchWidget::sendRequestPopupAdd, this, functor, searchTypeComboBox));        
    sendRequestPopupAdd(functor, searchTypeComboBox);
    
    popupFinalization(dialog, 0);    

    dialog->finished().connect(boost::bind(&PluginsTableSearchWidget::popupCheck, this, inputName, errorMessage, dialog, searchID));
    
    dialog->show();
}

void PluginsTableSearchWidget::sendRequestPopupAdd(boost::function<void (Wt::Json::Value)> functor, Wt::WComboBox* searchTypeComboBox)
{
    string parameters = "&type_id=" + m_addonStandardItemModel->item((searchTypeComboBox->currentIndex() == -1 ? 0 : searchTypeComboBox->currentIndex()), 1)->text().toUTF8();
          
    sendHttpRequestGet("searches/parameters", parameters, functor);
}

void PluginsTableSearchWidget::handleRequestPopupAdd(Wt::Json::Value result, Wt::WContainerWidget* paramsContainer,
                                     vector<Wt::WInteractWidget*>* inputName, long long searchID)
{
    paramsContainer->clear();
    
    vector<Wt::WInteractWidget*>::iterator it = inputName->begin();
    it+=2;
    while(it != inputName->end())
        it = inputName->erase(it);
    
    if(result.isNull())
        return;
        
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
    message->addBodyText(",\n\"type_id\": " + m_addonStandardItemModel->item(((Wt::WComboBox*)(*it++))->currentIndex(), 1)->text().toUTF8());
    message->addBodyText(",\n\"period\": " + ((Wt::WLineEdit*)(*it++))->text().toUTF8());
    
    while(it != argument->end())
    {
        string name = ((Wt::WLineEdit*)(*it))->attributeValue("name").toUTF8();
        string value = ((Wt::WLineEdit*)(*it++))->text().toUTF8();
        message->addBodyText(",\n\"" + name + "\": \"" + value + "\"");
    }
    
    message->addBodyText("\n}");
    Wt::log("test") << message->body();
}

void PluginsTableSearchWidget::setModifResourceMessage(Wt::Http::Message *message,vector<Wt::WInteractWidget*>* argument)
{
    setAddResourceMessage(message, argument);
}