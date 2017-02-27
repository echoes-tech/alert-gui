/**
 * @file PluginsTablePluginWidget.cpp
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
