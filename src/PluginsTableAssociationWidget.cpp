/* 
 * GUI PluginsTableAssociationWidget.cpp
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


#include <stddef.h>


#include <Wt/Json/Object>


#include <Wt/Json/Array>


#include "PluginsTableAssociationWidget.h"

using namespace std;

PluginsTableAssociationWidget::PluginsTableAssociationWidget(Echoes::Dbo::Session *session, string apiUrl,
        PluginsTableFilterWidget* pluginsTableFilterWidget)
: AbstractPage(session, apiUrl, "plugins-association")
{
    setButtonModif(false);
    setButtonSup(true);
    
    multimap<int, string> listTitles;
    listTitles.insert(make_pair(ETypeJson::text, "filter_index"));
    listTitles.insert(make_pair(ETypeJson::text, "information"));
    listTitles.insert(make_pair(ETypeJson::text, "asset"));
    setTitles(listTitles);
    
    m_pluginsTableFilterWidget = pluginsTableFilterWidget;
    m_selectedFilterID = 0;
}

void PluginsTableAssociationWidget::updatePage()
{    
    if(m_selectedFilterID != m_pluginsTableFilterWidget->getSelectedID())
    {
        m_selectedFilterID = m_pluginsTableFilterWidget->getSelectedID();
        setSelectedID(0);
    }    
    
    setAddButtonEnable(m_pluginsTableFilterWidget->getSelectedID() != 0);
    
    list<list<pair<string, vector<string>>>> listsUrl;
    if(m_pluginsTableFilterWidget->getSelectedID() != 0)
    {
        list<pair<string, vector<string>>> listUrl;
        vector<string> listParameter;

        listParameter.push_back("filter_id=" + boost::lexical_cast<string>(m_pluginsTableFilterWidget->getSelectedID()));    
        listUrl.push_back(pair<string, vector<string>>("information_datas", listParameter));  
        listParameter.clear();
        listUrl.push_back(pair<string, vector<string>>("information_datas/:id", listParameter));    
        listsUrl.push_back(listUrl);
        listUrl.clear();
    }    
    setUrl(listsUrl);
    
    AbstractPage::updatePage();
}

vector<Wt::WInteractWidget*> PluginsTableAssociationWidget::initRowWidgets(Wt::Json::Object jsonObject, vector<Wt::Json::Value> jsonResource, int cpt)
{    
    vector<Wt::WInteractWidget *> rowWidgets;
    
    try
    {
        Wt::Json::Object jsonDetailedIda = jsonResource.at(cpt + 1);

        int filterIndex = jsonObject.get("filter_field_index");

        Wt::Json::Object informationJsonObject = jsonDetailedIda.get("information");
        string informationName = informationJsonObject.get("name");

        Wt::Json::Object assetJsonObject = jsonDetailedIda.get("asset");
        string assetName = assetJsonObject.get("name");
        
        rowWidgets.push_back(new Wt::WText(boost::lexical_cast<string>(filterIndex)));
        rowWidgets.push_back(new Wt::WText(informationName));
        rowWidgets.push_back(new Wt::WText(assetName));
    }
    catch (Wt::Json::ParseError const& e)
    {
        Wt::log("warning") << "[PluginsTableAssociationWidget] Problems parsing JSON";
        Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
    }
    catch (Wt::Json::TypeException const& e)
    {
        Wt::log("warning") << "[PluginsTableAssociationWidget] JSON Type Exception";
        //            Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
    }
    
    return rowWidgets;
}

void PluginsTableAssociationWidget::addPopupAddHandler(Wt::WInteractWidget* widget)
{
    widget->clicked().connect(boost::bind(&PluginsTableAssociationWidget::addResourcePopup, this));
}

void PluginsTableAssociationWidget::addResourcePopup()
{
    vector<Wt::WInteractWidget*>* inputName = new vector<Wt::WInteractWidget*>();
    vector<Wt::WText*> errorMessage;

    Wt::WDialog *dialog = new Wt::WDialog(tr("Alert.plugins-association.add-plugins-association"));
    dialog->setMinimumSize(Wt::WLength(300), Wt::WLength::Auto);
        
    new Wt::WText(tr("Alert.plugins-association.add-asset") + "<br />", dialog->contents());
    Wt::WComboBox* assetComboBox = new Wt::WComboBox(dialog->contents());
    m_assetStandardItemModel = new Wt::WStandardItemModel();
    assetComboBox->setModel(m_assetStandardItemModel);
    addEnumToModel(m_assetStandardItemModel, 0, "");  
    inputName->push_back(assetComboBox);
    new Wt::WText("<br />", dialog->contents());
    
    new Wt::WText(tr("Alert.plugins-association.add-info") + "<br />", dialog->contents());
    Wt::WComboBox* infoComboBox = new Wt::WComboBox(dialog->contents());
    m_infoStandardItemModel = new Wt::WStandardItemModel();
    infoComboBox->setModel(m_infoStandardItemModel);
    addEnumToModel(m_infoStandardItemModel, 0, "");  
    inputName->push_back(infoComboBox);
    new Wt::WText("<br />", dialog->contents());
    
    new Wt::WText(tr("Alert.plugins-association.add-filter-index") + "<br />", dialog->contents());
    Wt::WComboBox* filterIndexComboBox = new Wt::WComboBox(dialog->contents());
    m_filterIndexStandardItemModel = new Wt::WStandardItemModel();
    filterIndexComboBox->setModel(m_filterIndexStandardItemModel);
    addEnumToModel(m_filterIndexStandardItemModel, 0, "");  
    inputName->push_back(filterIndexComboBox);
    new Wt::WText("<br />", dialog->contents());   
    
    boost::function<void (vectors_Json)> functorFillInfoAndFilterIndexComboBox = boost::bind(&PluginsTableAssociationWidget::fillInfoAndFilterIndexComboBox,
                                                                                             this, _1, dialog);      
    
    boost::function<void()> functorSendRequestInfoAndFilterIndexComboBox = boost::bind(&PluginsTableAssociationWidget::sendRequestInfoAndFilterIndexComboBox, this,
                                                                               functorFillInfoAndFilterIndexComboBox, assetComboBox);  
    
    boost::function<void (Wt::Json::Value)> functorFillHostModel = boost::bind(&PluginsTableAssociationWidget::fillHostComboBox, this, _1,
                                                                               functorSendRequestInfoAndFilterIndexComboBox, dialog);    
    sendHttpRequestGet("assets/", vector<string>(), functorFillHostModel);
    
    assetComboBox->changed().connect(boost::bind(&PluginsTableAssociationWidget::sendRequestInfoAndFilterIndexComboBox, this,
                                                 functorFillInfoAndFilterIndexComboBox, assetComboBox));
    
    popupFinalization(dialog, 0);    

    dialog->finished().connect(boost::bind(&PluginsTableAssociationWidget::popupCheck, this, inputName, errorMessage, dialog, 0));
    
    dialog->show();
}

void PluginsTableAssociationWidget::fillHostComboBox(Wt::Json::Value result, boost::function<void()> functorSendRequestInfoAndFilterComboBox,
                                                     Wt::WDialog* dialog)
{        
    m_assetStandardItemModel->clear();
        
    if(result.isNull())
    {
        addEnumToModel(m_assetStandardItemModel, 0, "");     
        Wt::WWidget* saveButton = dialog->footer()->children()[0];
        saveButton->setDisabled(true);
        return;
    }
    
    try
    {
        Wt::Json::Array& jsonArray = result;       
        for (size_t i(0); i < jsonArray.size(); i++)
        {
            Wt::Json::Object jsonObject = jsonArray.at(i);
            addEnumToModel(m_assetStandardItemModel, jsonObject.get("id"), jsonObject.get("name"));
        }
    }
    catch (Wt::Json::ParseError const& e)
    {
        Wt::log("warning") << "[PluginsTableAssociationWidget] Problems parsing JSON";
        Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
    }
    catch (Wt::Json::TypeException const& e)
    {
        Wt::log("warning") << "[PluginsTableAssociationWidget] JSON Type Exception";
        //            Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
    }
    
    functorSendRequestInfoAndFilterComboBox();
}

void PluginsTableAssociationWidget::sendRequestInfoAndFilterIndexComboBox(boost::function<void (vectors_Json)> functorFillInfoAndFilterComboBox,
                                                                     Wt::WComboBox* assetComboBox)
{        
    long long assetID = boost::lexical_cast<long long>(m_assetStandardItemModel->item((assetComboBox->currentIndex() == -1 ? 0 : assetComboBox->currentIndex()), 1)->text().toUTF8());
    
    if(assetID == 0)
    {
        return;
    }
    
    list<list<pair<string, vector<string>>>> listsUrl;
    list<pair<string, vector<string>>> listUrl;
    vector<string> listParameter;
    
    listUrl.push_back(pair<string, vector<string>>("informations", vector<string>()));
    listsUrl.push_back(listUrl);
    listUrl.clear();
         
    listUrl.push_back(pair<string, vector<string>>("filters/" + boost::lexical_cast<string>(m_pluginsTableFilterWidget->getSelectedID()), listParameter));
    listsUrl.push_back(listUrl);
    listUrl.clear();

    listParameter.push_back("asset_id=" + m_assetStandardItemModel->item((assetComboBox->currentIndex() == -1 ? 0 : assetComboBox->currentIndex()), 1)->text().toUTF8());    
    listUrl.push_back(pair<string, vector<string>>("information_datas", listParameter));  
    listParameter.clear();
    listsUrl.push_back(listUrl);
    listUrl.clear();

    listParameter.push_back("filter_id=" + boost::lexical_cast<string>(m_pluginsTableFilterWidget->getSelectedID()));  
    listParameter.push_back("asset_id=" + m_assetStandardItemModel->item((assetComboBox->currentIndex() == -1 ? 0 : assetComboBox->currentIndex()), 1)->text().toUTF8());    
    listUrl.push_back(pair<string, vector<string>>("information_datas", listParameter));  
    listParameter.clear();
    listsUrl.push_back(listUrl);
    listUrl.clear();
    
    recursiveGetResources(listsUrl, functorFillInfoAndFilterComboBox);
}

void PluginsTableAssociationWidget::fillInfoAndFilterIndexComboBox(vectors_Json jsonResources, Wt::WDialog* dialog)
{
    m_infoStandardItemModel->clear();
    m_filterIndexStandardItemModel->clear();
    
    try
    {
        Wt::Json::Array associationArrayAsset = Wt::Json::Array();    
        if(!jsonResources.at(2).at(0).isNull())
        {
            associationArrayAsset = jsonResources.at(2).at(0);
        }

        Wt::Json::Array associationArrayAssetFilter = Wt::Json::Array();    
        if(!jsonResources.at(3).at(0).isNull())
        {
            associationArrayAssetFilter = jsonResources.at(3).at(0);
        }

        if(!jsonResources.at(0).at(0).isNull())
        { 
            Wt::WStandardItemModel* tmpInfoStandardItemModel = new Wt::WStandardItemModel();
            Wt::Json::Array informationsArray = jsonResources.at(0).at(0);
            for (size_t i(0); i < informationsArray.size(); i++)
            {
                Wt::Json::Object information = informationsArray.at(i);
                long long informationID = information.get("id");
                long long informationUnitID = ((Wt::Json::Object)information.get("information_unit")).get("id");

                bool alreadyUsed = false;

                for (size_t j(0); j < associationArrayAsset.size(); j++)
                {
                    Wt::Json::Object association = associationArrayAsset.at(j);
                    try
                    {
                        long long associationInformationID = ((Wt::Json::Object)association.get("information")).get("id");
                        alreadyUsed |= informationID == associationInformationID;
                    }
                    catch (Wt::Json::TypeException const& e)
                    {
                        Wt::log("warning") << "[PluginsTableAssociationWidget] Get association Info Id - JSON Type Exception";
                    }
                }

                if(!alreadyUsed)
                {
                    addEnumToModel(tmpInfoStandardItemModel, informationID, information.get("name"), boost::lexical_cast<string>(informationUnitID));                
                }
            }
            tmpInfoStandardItemModel->sort(0);
            while(tmpInfoStandardItemModel->rowCount() != 0)
            {
                m_infoStandardItemModel->appendRow(tmpInfoStandardItemModel->takeRow(0));
            }        
        }

        if(!jsonResources.at(1).at(0).isNull())
        { 
            Wt::Json::Object filter = jsonResources.at(1).at(0);
            for (int filterIndex(1); filterIndex <= (int)filter.get("nb_value"); filterIndex++)
            {                        
                bool alreadyUsed = false;
                for (int j(0); j < (int) associationArrayAssetFilter.size(); j++)
                {
                    Wt::Json::Object association = associationArrayAssetFilter.at(j);    
                    int associationFilterFieldIndex = association.get("filter_field_index");

                    alreadyUsed |= filterIndex == (int)associationFilterFieldIndex;
                }

                if(!alreadyUsed)
                {
                    addEnumToModel(m_filterIndexStandardItemModel, filterIndex, boost::lexical_cast<string>(filterIndex));                
                }
            }
        }
    }
    catch (Wt::Json::ParseError const& e)
    {
        Wt::log("warning") << "[PluginsTableAssociationWidget] Problems parsing JSON";
        Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
    }
    catch (Wt::Json::TypeException const& e)
    {
        Wt::log("warning") << "[PluginsTableAssociationWidget] JSON Type Exception";
        //            Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
    }
    
    Wt::WWidget* saveButton = dialog->footer()->children()[0];
    saveButton->setDisabled(m_infoStandardItemModel->rowCount() == 0 || m_filterIndexStandardItemModel->rowCount() == 0);
    
    if(m_infoStandardItemModel->rowCount() == 0)
    {
        addEnumToModel(m_infoStandardItemModel, 0, "");          
    }    
    
    if(m_filterIndexStandardItemModel->rowCount() == 0)
    {
        addEnumToModel(m_filterIndexStandardItemModel, 0, "");          
    }  
}
    

void PluginsTableAssociationWidget::setAddResourceMessage(Wt::Http::Message *message,vector<Wt::WInteractWidget*>* argument)
{
    vector<Wt::WInteractWidget*>::iterator it = argument->begin();
    
    string assetID = m_assetStandardItemModel->item(((Wt::WComboBox*)(*it++))->currentIndex(), 1)->text().toUTF8();
    string informationID = m_infoStandardItemModel->item(((Wt::WComboBox*)(*it))->currentIndex(), 1)->text().toUTF8();
    string informationUnitID = m_infoStandardItemModel->item(((Wt::WComboBox*)(*it++))->currentIndex(), 2)->text().toUTF8();
    string filterFieldIndexID = m_filterIndexStandardItemModel->item(((Wt::WComboBox*)(*it++))->currentIndex(), 1)->text().toUTF8();
    string filterID = boost::lexical_cast<string>(m_selectedFilterID);
        
    message->addBodyText("[{");
    message->addBodyText("\n\"asset_id\": " + assetID);
    message->addBodyText(",\n\"information_id\": " + informationID);
    message->addBodyText(",\n\"information_unit_id\": " + informationUnitID);
    message->addBodyText(",\n\"filter_field_index_id\": " + filterFieldIndexID);
    message->addBodyText(",\n\"filter_id\": " + filterID);
    message->addBodyText("\n}]");
        
    list<list<pair<string, vector<string>>>> listsUrl;
    list<pair<string, vector<string>>> listUrl;
  
    listUrl.push_back(pair<string, vector<string>>("assets/" + assetID + "/plugins", vector<string>()));   
    listsUrl.push_back(listUrl);
    listUrl.clear();
    
    setUrl(listsUrl);
}