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
    titles.insert(make_pair(ETypeJson::integer, "id"));
    setTitles(titles);

    lists_string lListUrl;
    list_string listUrl;
    listUrl.push_back("sources");
    lListUrl.push_back(listUrl);
    listUrl.clear();
    
    setUrl(lListUrl);
    
    m_addonStandardItemModel = new Wt::WStandardItemModel(0,2,this);
    fillModel();
    
    m_abstractPage = abstractPage;
    m_selectedPluginID = 0;
}

void PluginsTableSourceWidget::fillModel()
{
    m_addonStandardItemModel->clear();
    addEnumToModel(m_addonStandardItemModel, Echoes::Dbo::EAddon::FILESYSTEM, tr("Alert.plugins.filesystem"));
    addEnumToModel(m_addonStandardItemModel, Echoes::Dbo::EAddon::FILE, tr("Alert.plugins.file"));
    addEnumToModel(m_addonStandardItemModel, Echoes::Dbo::EAddon::LOG, tr("Alert.plugins.log"));
    addEnumToModel(m_addonStandardItemModel, Echoes::Dbo::EAddon::ODBC, tr("Alert.plugins.odbc"));
    addEnumToModel(m_addonStandardItemModel, Echoes::Dbo::EAddon::SNMP, tr("Alert.plugins.snmp"));
    addEnumToModel(m_addonStandardItemModel, Echoes::Dbo::EAddon::HASH, tr("Alert.plugins.hash"));
    addEnumToModel(m_addonStandardItemModel, Echoes::Dbo::EAddon::XML, tr("Alert.plugins.xml"));
    addEnumToModel(m_addonStandardItemModel, Echoes::Dbo::EAddon::PROCESS, tr("Alert.plugins.process"));
}

void PluginsTableSourceWidget::updatePage(bool getResources)
{
    if(m_selectedPluginID != m_abstractPage->getSelectedID())
    {
        m_selectedPluginID = m_abstractPage->getSelectedID();
        setSelectedID(0);
    }
    AbstractPage::updatePage(getResources);
}

string PluginsTableSourceWidget::addParameter()
{
    return "&plugin_id=" + boost::lexical_cast<string>(m_abstractPage->getSelectedID());
}

Wt::WString PluginsTableSourceWidget::getSourceParameterName(long long id)
{    
    map<long long, Wt::WString> mapSourceParameterName;
    mapSourceParameterName[Echoes::Dbo::ESourceParameter::CONNECTION_STRING] = tr("Alert.plugins.connection-string");
    mapSourceParameterName[Echoes::Dbo::ESourceParameter::COMMUNITY] = tr("Alert.plugins.community");
    mapSourceParameterName[Echoes::Dbo::ESourceParameter::VERSION] = tr("Alert.plugins.version");
    mapSourceParameterName[Echoes::Dbo::ESourceParameter::AUTH_PROTO] = tr("Alert.plugins.auth-proto");
    mapSourceParameterName[Echoes::Dbo::ESourceParameter::AUTH_PATH] = tr("Alert.plugins.auth-path");
    mapSourceParameterName[Echoes::Dbo::ESourceParameter::PRIV_PROTO] = tr("Alert.plugins.priv-proto");
    mapSourceParameterName[Echoes::Dbo::ESourceParameter::PRIV_PATH] = tr("Alert.plugins.priv-path");
    mapSourceParameterName[Echoes::Dbo::ESourceParameter::HOST] = tr("Alert.plugins.host");
    mapSourceParameterName[Echoes::Dbo::ESourceParameter::USER] = tr("Alert.plugins.user");
    
    return mapSourceParameterName[id];
}

void PluginsTableSourceWidget::setModifResourceMessage(Wt::Http::Message *message,vector<Wt::WInteractWidget*> argument)
{
    vector<Wt::WInteractWidget*>::iterator it = argument.begin();
    
    message->addBodyText("{");
    message->addBodyText("\n\"name\": \"" + ((Wt::WLineEdit*)(*it++))->text().toUTF8() + "\"");
    message->addBodyText(",\n\"desc\": \"" + ((Wt::WLineEdit*)(*it++))->text().toUTF8() + "\"");
    message->addBodyText("\n}");
}

void PluginsTableSourceWidget::addPopupAddHandler(Wt::WInteractWidget* widget)
{
    widget->clicked().connect(boost::bind(&PluginsTableSourceWidget::addResourcePopup, this));
}

void PluginsTableSourceWidget::addResourcePopup()
{
    vector<Wt::WInteractWidget*>* inputName = new vector<Wt::WInteractWidget*>();
    vector<Wt::WText*> errorMessage;

    Wt::WDialog *dialog = new Wt::WDialog(tr("Alert.plugins-source.add-plugins-source"));
        
    Wt::WComboBox* addonComboBox = new Wt::WComboBox(dialog->contents());
    inputName->push_back(addonComboBox);
    addonComboBox->setModel(m_addonStandardItemModel);
    
    new Wt::WText("<br />", dialog->contents());
    
    Wt::WContainerWidget* paramsContainer = new Wt::WContainerWidget(dialog->contents());    
    
    addonComboBox->changed().connect(boost::bind(&PluginsTableSourceWidget::sendRequestPopupAdd, this,
                                        addonComboBox, paramsContainer, inputName));
        
    sendRequestPopupAdd(addonComboBox, paramsContainer, inputName);
    
    popupFinalization(dialog, 0);    

    dialog->finished().connect(boost::bind(&PluginsTableSourceWidget::popupCheck, this, inputName, errorMessage, dialog, -1));
    
    dialog->show();
}

void PluginsTableSourceWidget::sendRequestPopupAdd(Wt::WComboBox* addonComboBox, Wt::WContainerWidget* paramsContainer,
                                                          vector<Wt::WInteractWidget*>* inputName)
{
    string apiAddress = getApiUrl() + "/" + "sources/parameters"
            + "?login=" + Wt::Utils::urlEncode(m_session->user()->eMail.toUTF8())
            + "&token=" + m_session->user()->token.toUTF8()
            + "&addon_id=" + m_addonStandardItemModel->item((addonComboBox->currentIndex() == -1 ? 0 : addonComboBox->currentIndex()), 1)->text().toUTF8();
    
    boost::function<void (Wt::Json::Value)> functor;
    functor = boost::bind(&PluginsTableSourceWidget::handleRequestPopupAdd, this, _1, paramsContainer, inputName);
    
    sendHttpRequestGet(apiAddress, functor);
}

void PluginsTableSourceWidget::handleRequestPopupAdd(Wt::Json::Value result, Wt::WContainerWidget* paramsContainer,
                                     vector<Wt::WInteractWidget*>* inputName)
{
    paramsContainer->clear();
    
    Wt::WInteractWidget* iwComboBox = *(inputName->begin());
    inputName->clear();
    inputName->push_back(iwComboBox);
    
    Wt::Json::Array& jsonArray = result;   
    
    for (int cpt(0); cpt < (int) jsonArray.size(); cpt++)
    {
        Wt::Json::Object jsonObject = jsonArray.at(cpt);
        
        long long id = jsonObject.get("id");
        new Wt::WText(getSourceParameterName(id) + " : <br />", paramsContainer);
        
        Wt::WLineEdit* lineEdit = new Wt::WLineEdit(paramsContainer);
        
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
    message->addBodyText("\n\"plugin_id\": " + boost::lexical_cast<string>(m_abstractPage->getSelectedID()));
    message->addBodyText(",\n\"addon_id\": " + m_addonStandardItemModel->item(((Wt::WComboBox*)(*it++))->currentIndex(), 1)->text().toUTF8());
    
    while(it != argument->end())
    {
        string name = ((Wt::WLineEdit*)(*it))->attributeValue("name").toUTF8();
        string value = ((Wt::WLineEdit*)(*it++))->text().toUTF8();
        message->addBodyText(",\n\"" + name + "\": \"" + value + "\"");
    }
    
    message->addBodyText("\n}");
}