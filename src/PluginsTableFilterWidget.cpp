/* 
 * GUI PluginsTableFilterWidget.cpp
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
#include <boost/property_tree/json_parser.hpp>

#include "PluginsTableFilterWidget.h"

using namespace std;

PluginsTableFilterWidget::PluginsTableFilterWidget(Echoes::Dbo::Session *session, string apiUrl,
        PluginsTableSearchWidget* pluginsTableSearchWidget)
: AbstractPage(session, apiUrl, "plugins-filter", true)
{
    setButtonModif(true);
    setButtonSup(true);
    
    std::vector<std::pair <int, std::string>> titles;
    titles.push_back(make_pair(0, "filter-type"));
    titles.push_back(make_pair(0, "parameters"));
    titles.push_back(make_pair(0, "nb-value"));
    titles.push_back(make_pair(0, "pos-key-value"));
    setTitles(titles);
    
    m_filterTypeStandardItemModel = new Wt::WStandardItemModel(0,2,this);
    fillModel();
    
    m_pluginsTableSearchWidget = pluginsTableSearchWidget;
    m_selectedSearchID = 0;
}

void PluginsTableFilterWidget::fillModel()
{
    m_filterTypeStandardItemModel->clear();
    addEnumToModel(m_filterTypeStandardItemModel, Echoes::Dbo::EFilterType::ALL, getFilterTypeName(Echoes::Dbo::EFilterType::ALL));
    addEnumToModel(m_filterTypeStandardItemModel, Echoes::Dbo::EFilterType::REGEX, getFilterTypeName(Echoes::Dbo::EFilterType::REGEX));
    addEnumToModel(m_filterTypeStandardItemModel, Echoes::Dbo::EFilterType::LOCATION, getFilterTypeName(Echoes::Dbo::EFilterType::LOCATION));
}

void PluginsTableFilterWidget::updatePage()
{
    if(m_selectedSearchID != m_pluginsTableSearchWidget->getSelectedID())
    {
        m_selectedSearchID = m_pluginsTableSearchWidget->getSelectedID();
        setSelectedID(0);
    }             
    
    list<list<pair<string, vector<string>>>> listsUrl;
    if(m_pluginsTableSearchWidget->getSelectedID() != 0)
    {
        list<pair<string, vector<string>>> listUrl;
        vector<string> listParameter;
        
        listParameter.push_back("search_id=" + boost::lexical_cast<string>(m_pluginsTableSearchWidget->getSelectedID()));
        listUrl.push_back(pair<string, vector<string>>("filters", listParameter));        
        listParameter.clear();
        listUrl.push_back(pair<string, vector<string>>("filters/:id/parameters", listParameter));
        
        listsUrl.push_back(listUrl);
        listUrl.clear();
    }
    setUrl(listsUrl);
    
    setAddButtonEnable(m_pluginsTableSearchWidget->getSelectedID() != 0);
    
    AbstractPage::updatePage();
}

vector<Wt::WInteractWidget *> PluginsTableFilterWidget::initRowWidgets(Wt::Json::Object jsonObject, vector<Wt::Json::Value> jsonResource, int cpt)
{    
    if(cpt == 0)
    {
        m_filtersData.clear();
    }
    
    struct FilterData filterData;
        
    vector<Wt::WInteractWidget *> rowWidgets;
    
    try
    {
        int filterID = jsonObject.get("id");

        int filterTypeID = ((Wt::Json::Object)jsonObject.get("filter_type")).get("id");    
        rowWidgets.push_back(new Wt::WText(getFilterTypeName(filterTypeID)));

        filterData.filterTypeID = filterTypeID;    

        int nbValue = jsonObject.get("nb_value");
        rowWidgets.push_back(new Wt::WText(boost::lexical_cast<string>(nbValue)));    
        filterData.nbValue = nbValue;       

        int posKeyValue = jsonObject.get("pos_key_value");
        rowWidgets.push_back(new Wt::WText(boost::lexical_cast<string>(posKeyValue)));    
        filterData.posKeyValue = posKeyValue;

        Wt::WString filterParametersString;        
        if(!((Wt::Json::Value)jsonResource.at(cpt+1)).isNull())
        {
            Wt::Json::Array& jsonArrayParameters = jsonResource.at(cpt+1);
            for (int cpt(0); cpt < (int) jsonArrayParameters.size(); cpt++)
            {
                Wt::Json::Object jsonObjectParameter = jsonArrayParameters.at(cpt);

                int filterParameterID = ((Wt::Json::Object)jsonObjectParameter.get("id")).get("filter_parameter_id");
                Wt::WString filterParameterValue = jsonObjectParameter.get("value");

                filterParametersString += getFilterParameterName(filterParameterID);
                filterParametersString += ": ";
                filterParametersString += filterParameterValue;
                if(cpt != (int) jsonArrayParameters.size() - 1)
                    filterParametersString += "<br />";

                filterData.parametersValue[filterParameterID] = filterParameterValue;
            }
        }        
        Wt::WContainerWidget* containerWidget = new Wt::WContainerWidget();
        containerWidget->addWidget(new Wt::WText(filterParametersString));
        containerWidget->setContentAlignment(Wt::AlignmentFlag::AlignLeft);
        rowWidgets.push_back(containerWidget);    

        m_filtersData[filterID] = filterData;
    }
    catch (Wt::Json::ParseError const& e)
    {
        Wt::log("warning") << "[PluginsTableFilterWidget] Problems parsing JSON";
        Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
    }
    catch (Wt::Json::TypeException const& e)
    {
        Wt::log("warning") << "[PluginsTableFilterWidget] JSON Type Exception";
        //            Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
    }
    
    return rowWidgets;
}

Wt::WString PluginsTableFilterWidget::getFilterParameterName(long long filterParameterID)
{    
    map<long long, Wt::WString> mapFilterParameterName;
    mapFilterParameterName[Echoes::Dbo::EFilterParameter::REGEX] = tr("Alert.plugins-filter.filter-parameter.regex");
    mapFilterParameterName[Echoes::Dbo::EFilterParameter::FIRST_CHAR] = tr("Alert.plugins-filter.filter-parameter.first-char");
    mapFilterParameterName[Echoes::Dbo::EFilterParameter::LENGTH] = tr("Alert.plugins-filter.filter-parameter.length");
    
    return mapFilterParameterName[filterParameterID];
}

Wt::WString PluginsTableFilterWidget::getFilterTypeName(long long filterTypeID)
{    
    map<long long, Wt::WString> mapFilterTypeName;
    mapFilterTypeName[Echoes::Dbo::EFilterType::ALL] = tr("Alert.plugins-filter.filter-type.all");
    mapFilterTypeName[Echoes::Dbo::EFilterType::REGEX] = tr("Alert.plugins-filter.filter-type.regex");
    mapFilterTypeName[Echoes::Dbo::EFilterType::LOCATION] = tr("Alert.plugins-filter.filter-type.location");
    
    return mapFilterTypeName[filterTypeID];
}

void PluginsTableFilterWidget::addPopupAddHandler(Wt::WInteractWidget* widget)
{
    widget->clicked().connect(boost::bind(&PluginsTableFilterWidget::addResourcePopup, this, 0));
}

void PluginsTableFilterWidget::addPopupModifHandler(Wt::WInteractWidget* widget, long long filterID)
{
    widget->clicked().connect(boost::bind(&PluginsTableFilterWidget::addResourcePopup, this, filterID));
}

void PluginsTableFilterWidget::addResourcePopup(long long filterID)
{
    vector<Wt::WInteractWidget*>* inputName = new vector<Wt::WInteractWidget*>();
    vector<Wt::WText*> errorMessage;
    
    std::string xmlPopup;
    if (filterID == 0)
    {
        xmlPopup = "Alert.plugins-filter.popup-add";
    }
    else
    {
        xmlPopup = "Alert.plugins-filter.popup-modif";
    }
    Wt::WDialog *dialog = new Wt::WDialog(tr(xmlPopup));
    
    dialog->setMinimumSize(Wt::WLength(300), Wt::WLength::Auto);
        
    Wt::WContainerWidget* paramsContainer = new Wt::WContainerWidget();      
    boost::function<void (Wt::Json::Value)> functor = boost::bind(&PluginsTableFilterWidget::handleRequestPopupAdd, this, _1, paramsContainer, inputName, filterID);
    
    Wt::WComboBox* filterTypeComboBox = new Wt::WComboBox();
    if(filterID == 0)
    {
        Wt::WComboBox* filterTypeComboBox = new Wt::WComboBox(dialog->contents());
        inputName->push_back(filterTypeComboBox);
        filterTypeComboBox->setModel(m_filterTypeStandardItemModel);
        filterTypeComboBox->setCurrentIndex(0);
        for(int row(0); row < m_filterTypeStandardItemModel->rowCount(); row++)  
        {
            if(boost::lexical_cast<long long>(m_filterTypeStandardItemModel->item(row, 1)->text()) == m_filtersData[filterID].filterTypeID)
            {
                filterTypeComboBox->setCurrentIndex(row);            
            }
        }    
        filterTypeComboBox->changed().connect(boost::bind(&PluginsTableFilterWidget::sendRequestPopupAdd, this, functor, filterTypeComboBox, filterID));
        new Wt::WText("<br />", dialog->contents());   
    }    
    
    new Wt::WText(tr("Alert.plugins-filter.add-nb-value") + " : <br />", dialog->contents());    
    Wt::WLineEdit* nbValueLineEdit = new Wt::WLineEdit(dialog->contents());  
    nbValueLineEdit->setAttributeValue("name", "nb_value"); 
    if(filterID > 0)  
    {
        nbValueLineEdit->setText(boost::lexical_cast<string>(m_filtersData[filterID].nbValue));
    }
    inputName->push_back(nbValueLineEdit); 
    new Wt::WText("<br />", dialog->contents());
    
    new Wt::WText(tr("Alert.plugins-filter.add-pos-key-value") + " : <br />", dialog->contents());    
    Wt::WLineEdit* posKeyValueLineEdit = new Wt::WLineEdit(dialog->contents());  
    posKeyValueLineEdit->setAttributeValue("name", "position_key_value"); 
    if(filterID > 0)  
    {
        posKeyValueLineEdit->setText(boost::lexical_cast<string>(m_filtersData[filterID].posKeyValue));
    }
    inputName->push_back(posKeyValueLineEdit); 
    new Wt::WText("<br />", dialog->contents());  
    
    dialog->contents()->addWidget(paramsContainer);   
        
    sendRequestPopupAdd(functor, filterTypeComboBox, filterID);
    
    popupFinalization(dialog, 0);    

    dialog->finished().connect(boost::bind(&PluginsTableFilterWidget::popupCheck, this, inputName, errorMessage, dialog, filterID));
    
    dialog->show();
}

void PluginsTableFilterWidget::sendRequestPopupAdd(boost::function<void (Wt::Json::Value)> functor, Wt::WComboBox* filterTypeComboBox, long long filterID)
{
    vector<string> parameterList;    
    if(filterID == 0)
    {
        parameterList.push_back("&type_id=" + m_filterTypeStandardItemModel->item((filterTypeComboBox->currentIndex() == -1 ? 0 : filterTypeComboBox->currentIndex()), 1)->text().toUTF8());
    }
    else 
    {        
        parameterList.push_back("&type_id=" + boost::lexical_cast<string>(m_filtersData[filterID].filterTypeID));
    }
    
    sendHttpRequestGet("filters/parameters", parameterList, functor);
}

void PluginsTableFilterWidget::handleRequestPopupAdd(Wt::Json::Value result, Wt::WContainerWidget* paramsContainer,
                                     vector<Wt::WInteractWidget*>* inputName, long long filterID)
{
    paramsContainer->clear();    
   
    vector<Wt::WInteractWidget*>::iterator it = inputName->begin();
    if(filterID == 0)
    {
        it+=3;
    }
    else
    {
        it+=2;
    }
    while(it != inputName->end())
    {
        it = inputName->erase(it);
    }
    
    if(result.isNull())
    {
        return;
    }
    
    try    
    {
        Wt::Json::Array& jsonArray = result;   

        for (int cpt(0); cpt < (int) jsonArray.size(); cpt++)
        {
            Wt::Json::Object jsonObject = jsonArray.at(cpt);

            long long filterParameterID = jsonObject.get("id");
            new Wt::WText(getFilterParameterName(filterParameterID) + " : <br />", paramsContainer);

            Wt::WLineEdit* lineEdit = new Wt::WLineEdit(paramsContainer);
            if(filterID > 0)
                lineEdit->setText(m_filtersData[filterID].parametersValue[filterParameterID]);

            Wt::WString wsName = jsonObject.get("name");
            lineEdit->setAttributeValue("name", wsName);

            inputName->push_back(lineEdit);        

            new Wt::WText("<br />", paramsContainer);
        }
    }
    catch (Wt::Json::ParseError const& e)
    {
        Wt::log("warning") << "[PluginsTableFilterWidget] Problems parsing JSON";
        Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
    }
    catch (Wt::Json::TypeException const& e)
    {
        Wt::log("warning") << "[PluginsTableFilterWidget] JSON Type Exception";
        //            Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
    }
    
}

void PluginsTableFilterWidget::setAddResourceMessage(Wt::Http::Message *message,vector<Wt::WInteractWidget*>* argument)
{
    vector<Wt::WInteractWidget*>::iterator it = argument->begin();
    
    message->addBodyText("{");
    message->addBodyText("\n\"search_id\": " + boost::lexical_cast<string>(m_pluginsTableSearchWidget->getSelectedID()));
    message->addBodyText(",\n\"type_id\": " + m_filterTypeStandardItemModel->item(((Wt::WComboBox*)(*it++))->currentIndex(), 1)->text().toUTF8());
    
    while(it != argument->end())
    {
        string name = ((Wt::WLineEdit*)(*it))->attributeValue("name").toUTF8();
        string value = ((Wt::WLineEdit*)(*it++))->text().toUTF8();
        message->addBodyText(",\n\"" + name + "\": \"" + boost::property_tree::json_parser::create_escapes(value) + "\"");
    }
    
    message->addBodyText("\n}");
}

void PluginsTableFilterWidget::setModifResourceMessage(Wt::Http::Message *message,vector<Wt::WInteractWidget*>* argument)
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
        message->addBodyText("\n\"" + name + "\": \"" + boost::property_tree::json_parser::create_escapes(value) + "\"");
    }
    
    message->addBodyText("\n}");
}