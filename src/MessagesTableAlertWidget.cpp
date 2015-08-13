/* 
 * GUI MessagesTableAlertWidget.cpp
 * 
 * @author ECHOES Technologies (CDE)
 * 
 * @date 04/08/2015
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS WITHOUT
 * COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012-2013 BY ECHOES TECHNOLGIES SAS
 * 
 */

#include "MessagesTableAlertWidget.h"

using namespace std;

MessagesTableAlertWidget::MessagesTableAlertWidget(Echoes::Dbo::Session *session, string apiUrl)
: AbstractPage(session, apiUrl, "messages-alert", true)
{
    setButtonModif(false);
    setButtonSup(false);
    m_hasAddButton = false;
    
    std::vector<std::pair <int, string>>titles;
    titles.push_back(make_pair(setValidatorType(ETypeJson::text, 0, EMandatory::isMandatory), "name"));
    titles.push_back(make_pair(setValidatorType(ETypeJson::text, 0, EMandatory::isNotMandatory), "statement"));
    titles.push_back(make_pair(setValidatorType(ETypeJson::text, 0, EMandatory::isNotMandatory), "supporter"));
    titles.push_back(make_pair(setValidatorType(ETypeJson::widget, 0, EMandatory::isMandatory), "take"));
    titles.push_back(make_pair(setValidatorType(ETypeJson::widget, 0, EMandatory::isMandatory), "resolved"));

    setTitles(titles);
    
    list<list<pair<string, vector<string>>>> listsUrl;
    list<pair<string, vector<string>>> listUrl;
    
    listUrl.push_back(pair<string, vector<string>>("alerts", vector<string>()));
    listsUrl.push_back(listUrl);
    listUrl.clear();
    
    setUrl(listsUrl);
    
    m_assetComboBox = new Wt::WComboBox();
    m_assetsStandardItemModel = new Wt::WStandardItemModel(0,2,this);
    
    boost::function<void (Wt::Json::Value)> functorFillModel = boost::bind(&MessagesTableAlertWidget::fillModel, this, _1);    
    string resource = "alerts";
    sendHttpRequestGet(resource, vector<string>(), functorFillModel);
}

int MessagesTableAlertWidget::addCustomButtonsToResourceTable(long long id, int rowTable, int columnTable)
{
    // Wt::WLink *link;= generateScript(id, ((Wt::WText*)getResourceTable()->elementAt(rowTable, 0)->widget(0))->text());
    Wt::WAnchor *assignementButton = new Wt::WAnchor();//(link, "");
    assignementButton->setAttributeValue("class", "btn btn-info");
    assignementButton->setTextFormat(Wt::XHTMLUnsafeText);
    assignementButton->setText("<span class='input-group-btn'><i class='icon-medkit icon-white'></i></span>");
    assignementButton->clicked().connect(boost::bind(&MessagesTableAlertWidget::takeAssignement, this));
    getResourceTable()->elementAt(rowTable, columnTable)->addWidget(assignementButton); 
    getResourceTable()->elementAt(rowTable, columnTable)->setWidth(Wt::WLength(Wt::WLength(5, Wt::WLength::Percentage)));
    getResourceTable()->elementAt(rowTable, columnTable)->setContentAlignment(Wt::AlignCenter);
    ++columnTable;
    
    //Wt::WLink *link2;// = generateScript(id, ((Wt::WText*)getResourceTable()->elementAt(rowTable, 0)->widget(0))->text());
    Wt::WAnchor *resolvedButton = new Wt::WAnchor();//link2, "");
    resolvedButton->setAttributeValue("class", "btn btn-info");
    resolvedButton->setTextFormat(Wt::XHTMLUnsafeText);
    resolvedButton->setText("<span class='input-group-btn'><i class='icon-chevron-down icon-white'></i></span>");
    resolvedButton->clicked().connect(boost::bind(&MessagesTableAlertWidget::setResolved, this));
    getResourceTable()->elementAt(rowTable, columnTable)->addWidget(resolvedButton); 
    getResourceTable()->elementAt(rowTable, columnTable)->setWidth(Wt::WLength(Wt::WLength(5, Wt::WLength::Percentage)));
    getResourceTable()->elementAt(rowTable, columnTable)->setContentAlignment(Wt::AlignCenter);
    return ++columnTable;
}

void MessagesTableAlertWidget::takeAssignement()
{
    
}

void MessagesTableAlertWidget::setResolved()
{
    
}

void MessagesTableAlertWidget::fillModel(Wt::Json::Value result)
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
            Wt::WString alertState = "empty";
            Wt::WString alertAssign = "empty";

            Wt::WStandardItem *itemId = new Wt::WStandardItem();
            Wt::WStandardItem *itemName = new Wt::WStandardItem();
            Wt::WStandardItem *itemState = new Wt::WStandardItem();
            Wt::WStandardItem *itemAssign = new Wt::WStandardItem();

            vector<Wt::WStandardItem*> rowVector;

            itemName->setText(alertName);
            rowVector.push_back(itemName);
            itemName->setText(alertState);
            rowVector.push_back(itemState);
            itemName->setText(alertAssign);
            rowVector.push_back(itemAssign);
            itemId->setText(boost::lexical_cast<string>(alertId));
            rowVector.push_back(itemId);     

            m_assetsStandardItemModel->appendRow(rowVector);
        }
        
    }
    catch (Wt::Json::ParseError const& e)
    {
        Wt::log("warning") << "[MessagesTableAlertWidget] Problems parsing JSON";
        Wt::WMessageBox::show(tr("Alert.alerts.database-error-title"), tr("Alert.alerts.database-error"), Wt::Ok);
    }
    catch (Wt::Json::TypeException const& e)
    {
        Wt::log("warning") << "[MessagesTableAlertWidget] JSON Type Exception";
    }
    
}

Wt::WComboBox *MessagesTableAlertWidget::popupAdd(Wt::WDialog *dialog)
{
    m_assetComboBox = new Wt::WComboBox(dialog->contents());
    m_assetComboBox->setModel(m_assetsStandardItemModel);
    m_assetComboBox->setCurrentIndex(0);
 
    return m_assetComboBox;
}
/*
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
}*/

