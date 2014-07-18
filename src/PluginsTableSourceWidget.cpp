/* 
 * GUI PluginsTableSourceWidget.cpp
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

#include "PluginsTableSourceWidget.h"

using namespace std;

PluginsTableSourceWidget::PluginsTableSourceWidget(Echoes::Dbo::Session *session, string apiUrl,
        AbstractPage* abstractPage)
: AbstractPage(session, apiUrl, "plugins-source", true)
{
    setButtonModif(true);
    setButtonSup(true);
    
    multimap<int, string> titles;
    titles.insert(make_pair(0, "addon"));
    titles.insert(make_pair(0, "parameters"));
    setTitles(titles);
    
    m_addonStandardItemModel = new Wt::WStandardItemModel(0,2,this);
    fillModel();
    
    m_pluginsTablePluginWidget = abstractPage;
    m_selectedPluginID = 0;
}

void PluginsTableSourceWidget::fillModel()
{
    m_addonStandardItemModel->clear();
     addEnumToModel(m_addonStandardItemModel, Echoes::Dbo::EAddon::FILESYSTEM, getAddonName(Echoes::Dbo::EAddon::FILESYSTEM));
    addEnumToModel(m_addonStandardItemModel, Echoes::Dbo::EAddon::FILE, getAddonName(Echoes::Dbo::EAddon::FILE));
    addEnumToModel(m_addonStandardItemModel, Echoes::Dbo::EAddon::LOG, getAddonName(Echoes::Dbo::EAddon::LOG));
    addEnumToModel(m_addonStandardItemModel, Echoes::Dbo::EAddon::ODBC, getAddonName(Echoes::Dbo::EAddon::ODBC));
    addEnumToModel(m_addonStandardItemModel, Echoes::Dbo::EAddon::SNMP, getAddonName(Echoes::Dbo::EAddon::SNMP));
    addEnumToModel(m_addonStandardItemModel, Echoes::Dbo::EAddon::HASH, getAddonName(Echoes::Dbo::EAddon::HASH));
    addEnumToModel(m_addonStandardItemModel, Echoes::Dbo::EAddon::XML, getAddonName(Echoes::Dbo::EAddon::XML));
    addEnumToModel(m_addonStandardItemModel, Echoes::Dbo::EAddon::PROCESS, getAddonName(Echoes::Dbo::EAddon::PROCESS));
}

void PluginsTableSourceWidget::updatePage()
{
    if(m_selectedPluginID != m_pluginsTablePluginWidget->getSelectedID())
    {
        m_selectedPluginID = m_pluginsTablePluginWidget->getSelectedID();
        setSelectedID(0);
    }       

    std::list<std::list<std::string>> listsUrl;
    if(m_pluginsTablePluginWidget->getSelectedID() != 0)
    {
        std::list<std::string> listUrl;
        listUrl.push_back("sources");
        listUrl.push_back("sources/:id/parameters");
        listsUrl.push_back(listUrl);
        listUrl.clear();
    }
    setUrl(listsUrl);
    AbstractPage::updatePage();
}

long long PluginsTableSourceWidget::getSelectedSourceAddonID()
{
    return m_sourcesData[getSelectedID()].addonID;
}

string PluginsTableSourceWidget::addParameter()
{
    return "&plugin_id=" + boost::lexical_cast<string>(m_pluginsTablePluginWidget->getSelectedID());
}

vector<Wt::WInteractWidget *> PluginsTableSourceWidget::initRowWidgets(Wt::Json::Object jsonObject, vector<Wt::Json::Value> jsonResource, int cpt)
{    
    if(cpt == 0)
        m_sourcesData.clear();
    
    struct SourceData sourceData;
        
    vector<Wt::WInteractWidget *> rowWidgets;
    
    int sourceID = jsonObject.get("id");
    
    int addonID = ((Wt::Json::Object)jsonObject.get("addon")).get("id");    
    rowWidgets.push_back(new Wt::WText(getAddonName(addonID)));
    
    sourceData.addonID = addonID;
        
    Wt::WString sourceParametersString;
        
    if(!((Wt::Json::Value)jsonResource.at(cpt+1)).isNull())
    {
        Wt::Json::Array& jsonArrayParameters = jsonResource.at(cpt+1);
        for (int cpt(0); cpt < (int) jsonArrayParameters.size(); cpt++)
        {
            Wt::Json::Object jsonObjectParameter = jsonArrayParameters.at(cpt);

            int sourceParameterID = ((Wt::Json::Object)jsonObjectParameter.get("id")).get("source_parameter_id");
            Wt::WString sourceParameterValue = jsonObjectParameter.get("value");

            sourceParametersString += getSourceParameterName(sourceParameterID);
            sourceParametersString += ": ";
            sourceParametersString += sourceParameterValue;
            if(cpt != (int) jsonArrayParameters.size() - 1)
                sourceParametersString += "<br />";
            
            sourceData.parametersValue[sourceParameterID] = sourceParameterValue;
        }
    }
        
    Wt::WContainerWidget* containerWidget = new Wt::WContainerWidget();
    containerWidget->addWidget(new Wt::WText(sourceParametersString));
    containerWidget->setContentAlignment(Wt::AlignmentFlag::AlignLeft);
    rowWidgets.push_back(containerWidget);
        
    m_sourcesData[sourceID] = sourceData;
    
    return rowWidgets;
}

Wt::WString PluginsTableSourceWidget::getSourceParameterName(long long sourceParameterID)
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

Wt::WString PluginsTableSourceWidget::getAddonName(long long addonID)
{    
    map<long long, Wt::WString> mapAddonName;
    mapAddonName[Echoes::Dbo::EAddon::FILESYSTEM] = tr("Alert.plugins-source.filesystem");
    mapAddonName[Echoes::Dbo::EAddon::FILE] = tr("Alert.plugins-source.file");
    mapAddonName[Echoes::Dbo::EAddon::LOG] = tr("Alert.plugins-source.log");
    mapAddonName[Echoes::Dbo::EAddon::ODBC] = tr("Alert.plugins-source.odbc");
    mapAddonName[Echoes::Dbo::EAddon::SNMP] = tr("Alert.plugins-source.snmp");
    mapAddonName[Echoes::Dbo::EAddon::HASH] = tr("Alert.plugins-source.hash");
    mapAddonName[Echoes::Dbo::EAddon::XML] = tr("Alert.plugins-source.xml");
    mapAddonName[Echoes::Dbo::EAddon::PROCESS] = tr("Alert.plugins-source.process");
    
    return mapAddonName[addonID];
}

void PluginsTableSourceWidget::addPopupAddHandler(Wt::WInteractWidget* widget)
{
    widget->clicked().connect(boost::bind(&PluginsTableSourceWidget::addResourcePopup, this, 0));
}

void PluginsTableSourceWidget::addPopupModifHandler(Wt::WInteractWidget* widget, long long sourceID)
{
    widget->clicked().connect(boost::bind(&PluginsTableSourceWidget::addResourcePopup, this, sourceID));
}

void PluginsTableSourceWidget::addResourcePopup(long long sourceID)
{
    vector<Wt::WInteractWidget*>* inputName = new vector<Wt::WInteractWidget*>();
    vector<Wt::WText*> errorMessage;

    Wt::WDialog *dialog = new Wt::WDialog(tr("Alert.plugins-source.add-plugins-source"));
        
    Wt::WComboBox* addonComboBox = new Wt::WComboBox(dialog->contents());
    inputName->push_back(addonComboBox);
    addonComboBox->setModel(m_addonStandardItemModel);
    
    if(sourceID > 0)
        for(int row(0); row < m_addonStandardItemModel->rowCount(); row++)    
            if(boost::lexical_cast<long long>(m_addonStandardItemModel->item(row, 1)->text()) == m_sourcesData[sourceID].addonID)
                addonComboBox->setCurrentIndex(row);
            
    new Wt::WText("<br />", dialog->contents());
    
    Wt::WContainerWidget* paramsContainer = new Wt::WContainerWidget(dialog->contents());    
    boost::function<void (Wt::Json::Value)> functor = boost::bind(&PluginsTableSourceWidget::handleRequestPopupAdd, this, _1, paramsContainer, inputName, sourceID);
    
    addonComboBox->changed().connect(boost::bind(&PluginsTableSourceWidget::sendRequestPopupAdd, this, functor, addonComboBox));
        
    sendRequestPopupAdd(functor, addonComboBox);
    
    popupFinalization(dialog, 0);    

    dialog->finished().connect(boost::bind(&PluginsTableSourceWidget::popupCheck, this, inputName, errorMessage, dialog, sourceID));
    
    dialog->show();
}

void PluginsTableSourceWidget::sendRequestPopupAdd(boost::function<void (Wt::Json::Value)> functor, Wt::WComboBox* addonComboBox)
{
    string parameters = "&addon_id=" + m_addonStandardItemModel->item((addonComboBox->currentIndex() == -1 ? 0 : addonComboBox->currentIndex()), 1)->text().toUTF8();    
    
    sendHttpRequestGet("sources/parameters", parameters, functor);
}

void PluginsTableSourceWidget::handleRequestPopupAdd(Wt::Json::Value result, Wt::WContainerWidget* paramsContainer,
                                     vector<Wt::WInteractWidget*>* inputName, long long sourceID)
{
    paramsContainer->clear();
    
   
    vector<Wt::WInteractWidget*>::iterator it = inputName->begin();
    it++;
    while(it != inputName->end())
        it = inputName->erase(it);
    
    if(result.isNull())
        return;
        
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
}

void PluginsTableSourceWidget::setAddResourceMessage(Wt::Http::Message *message,vector<Wt::WInteractWidget*>* argument)
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

void PluginsTableSourceWidget::setModifResourceMessage(Wt::Http::Message *message,vector<Wt::WInteractWidget*>* argument)
{
    setAddResourceMessage(message, argument);
}