/* 
 * GUI PluginsTablePluginWidget.cpp
 * 
 * @author ECHOES Technologies (MLA)
 * 
 * @date 09/07/2014
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS WITHOUT
 * COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012-2013 BY ECHOES TECHNOLGIES SAS
 * 
 */

#include "PluginsTablePluginWidget.h"

using namespace std;

PluginsTablePluginWidget::PluginsTablePluginWidget(Echoes::Dbo::Session *session, string apiUrl)
: AbstractPage(session, apiUrl, "plugins-plugin", true)
{
    setButtonModif(true);
    setButtonSup(true);
    

    std::vector<std::pair <int, string>>titles;
    titles.push_back(make_pair(setValidatorType(ETypeJson::text, 0, EMandatory::isMandatory), "name"));
    titles.push_back(make_pair(setValidatorType(ETypeJson::text, 0, 0), "desc"));
    titles.push_back(make_pair(setValidatorType(ETypeJson::object, 0, EMandatory::isMandatory), "asset"));

setTitles(titles);
    
    list<list<pair<string, vector<string>>>> listsUrl;
    list<pair<string, vector<string>>> listUrl;
    
    listUrl.push_back(pair<string, vector<string>>("plugins", vector<string>()));
    listsUrl.push_back(listUrl);
    listUrl.clear();
    
    setUrl(listsUrl);
    
    m_assetComboBox = new Wt::WComboBox();
    m_assetsStandardItemModel = new Wt::WStandardItemModel(0,2,this);
    
    boost::function<void (Wt::Json::Value)> functorFillModel = boost::bind(&PluginsTablePluginWidget::fillModel, this, _1);    
    string resource = "assets";
    sendHttpRequestGet(resource, vector<string>(), functorFillModel);
}

void PluginsTablePluginWidget::fillModel(Wt::Json::Value result)
{
    m_assetsStandardItemModel->clear();
    
    try
    {
        Wt::Json::Array& jsonArray = result;   
    
        for (int cpt(0); cpt < (int) jsonArray.size(); cpt++)
        {
            Wt::Json::Object jsonObject = jsonArray.at(cpt);
            long long alertId = jsonObject.get("id");
            Wt::WString alertName = jsonObject.get("name");

            Wt::WStandardItem *itemId = new Wt::WStandardItem();
            Wt::WStandardItem *itemName = new Wt::WStandardItem();

            vector<Wt::WStandardItem*> rowVector;

            itemName->setText(alertName);
            rowVector.push_back(itemName);
            itemId->setText(boost::lexical_cast<string>(alertId));
            rowVector.push_back(itemId);     

            m_assetsStandardItemModel->appendRow(rowVector);
        }
        
    }
    catch (Wt::Json::ParseError const& e)
    {
        Wt::log("warning") << "[PluginsTablePluginWidget] Problems parsing JSON";
        Wt::WMessageBox::show(tr("Alert.global.database-error-title"), tr("Alert.global.database-error"), Wt::Ok);
    }
    catch (Wt::Json::TypeException const& e)
    {
        Wt::log("warning") << "[PluginsTablePluginWidget] JSON Type Exception";
    }
    
}

Wt::WComboBox *PluginsTablePluginWidget::popupAdd(Wt::WDialog *dialog)
{
    m_assetComboBox = new Wt::WComboBox(dialog->contents());
    m_assetComboBox->setModel(m_assetsStandardItemModel);
    m_assetComboBox->setCurrentIndex(0);
 
    return m_assetComboBox;
}

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
}

