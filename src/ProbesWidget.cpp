/**
 * @file ProbesWidget.cpp
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

#include <Wt/WTimer>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include "ProbesWidget.h"

using namespace std;

int ProbesWidget::m_stateColumn = 2;

ProbesWidget::ProbesWidget(Echoes::Dbo::Session *session, string apiUrl)
: AbstractPage(session, apiUrl, "probe")
{
    setButtonModif(false);
    setButtonSup(false);
    
    m_hasAddButton = false;
        
    std::vector<std::pair <int, string>>titles;
    titles.push_back(make_pair(setValidatorType(ETypeJson::text, 0, EMandatory::is), "name"));
    titles.push_back(make_pair(setValidatorType(ETypeJson::text, 0, EMandatory::is), "asset"));
    titles.push_back(make_pair(setValidatorType(ETypeJson::boolean, 0, EMandatory::is), "alive"));
    setTitles(titles);

    
    
    

    list<list<pair<string, vector<string>>>> listsUrl;
    list<pair<string, vector<string>>> listUrl;
    
    listUrl.push_back(pair<string, vector<string>>("probes", vector<string>())); 
    listsUrl.push_back(listUrl);
    listUrl.clear();
    
    listUrl.push_back(pair<string, vector<string>>("probes", vector<string>())); 
    listUrl.push_back(pair<string, vector<string>>("probes/:id", vector<string>()));    
    listsUrl.push_back(listUrl);
    listUrl.clear();
    
    listUrl.push_back(pair<string, vector<string>>("probes", vector<string>())); 
    listUrl.push_back(pair<string, vector<string>>("probes/:id/alive", vector<string>()));    
    listsUrl.push_back(listUrl);
    listUrl.clear();

    setUrl(listsUrl);
    

}

ProbesWidget::~ProbesWidget()
{
}

void ProbesWidget::handleJsonGetHeartbeat(vector<Wt::Json::Value> jsonResource)
{
    try
    {
        if (jsonResource.size() == 1)
        {
            Wt::Json::Object jsonProbe = jsonResource.at(0);
            
            long long probeId = jsonProbe.get("id");
            
            m_mapProbesAlive[probeId] = jsonProbe.get("probe_heartbeat");
            updateText(probeId);
        }
        else if (jsonResource.size() > 0)
        {
            Wt::Json::Array& jsonArray = (*jsonResource.begin());
            if (!jsonArray.empty())
            {
                for (int cpt(0); cpt < (int) jsonArray.size(); cpt++)
                {
                    Wt::Json::Object jsonMasterLevel = jsonArray.at(cpt);
                    Wt::Json::Object jsonProbe = jsonResource.at(cpt + 1);

                    long long probeId = jsonMasterLevel.get("id");

                    m_mapProbesAlive[probeId] = jsonProbe.get("probe_heartbeat");
                }
            }
        }
    }
    catch (Wt::Json::ParseError const& e)
    {
        Wt::log("warning") << "[ProbesWidget][handleJsonGetHeartbeat] Problems parsing JSON";
        Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
    }
    catch (Wt::Json::TypeException const& e)
    {
        Wt::log("warning") << "[ProbesWidget][handleJsonGetHeartbeat] JSON Type Exception";
//        Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
    }  
}

//void ProbesWidget::getResourceList()
//{
//    clearStructures();
//    recursiveGetResources();
//    fillBodyTable();
//}


void ProbesWidget::handleJsonGet(vectors_Json jsonResources)
{
    /* If simple request (/probe/current_id/alive) size < 4 else normal treatment*/
    if (jsonResources.empty())
    {
        return ;
    }
    else if (jsonResources.size() < 4)
    {
        vector<Wt::Json::Value> jsonResource = jsonResources.at(0);
        handleJsonGetHeartbeat(jsonResource);
    }
    else
    {
        vector<Wt::Json::Value> jsonResource = jsonResources.at(2);

        handleJsonGetHeartbeat(jsonResource);

        jsonResource = jsonResources.at(1);

        try
        {
            if (jsonResource.size() > 0)
            {
                Wt::Json::Array& jsonArray = (*jsonResource.begin());
                if (!jsonArray.empty())
                {
                    for (int cpt(0); cpt < (int) jsonArray.size(); cpt++)
                    {
                        Wt::Json::Object jsonProbe = jsonResource.at(cpt + 1);
                        long long probeId = jsonProbe.get("id");
                        string probeName = jsonProbe.get("name");

                        Wt::Json::Object jsonAsset = jsonProbe.get("asset");
                        string assetName = jsonAsset.get("name");

                        m_mapProbesTimer[probeId] = jsonProbe.get("timer");
                        
                        /* Set m_rowsTable */
                        vector<Wt::WInteractWidget *> nameW;

                        Wt::WText *pName = new Wt::WText(probeName);
                        pName->setObjectName("text");
                        nameW.push_back(pName);

                        Wt::WText *aName = new Wt::WText(assetName);
                        aName->setObjectName("text");
                        nameW.push_back(aName);

                        m_rowsTable.insert(make_pair(probeId, nameW));
                    }
                }
            }

        }
        catch (Wt::Json::ParseError const& e)
        {
            Wt::log("warning") << "[ProbesWidget][handleJsonGet] Problems parsing JSON";
            Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
        }
        catch (Wt::Json::TypeException const& e)
        {
            Wt::log("warning") << "[ProbesWidget][handleJsonGet] JSON Type Exception";
            //        Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
        }
        fillTable();
    }
}

void ProbesWidget::updateAliveStates(long long id, int rowTable, int columnTable)
{     
    if (!Wt::WApplication::instance()->internalPathMatches("/probes"))
    {
        m_autoUpdate = true;
        m_mapProbesWTimer[id]->stop();
        return;
    }
    
    list<list<pair<string, vector<string>>>> listsUrl;
    list<pair<string, vector<string>>> listUrl;
    
    string probe = "probes/" + boost::lexical_cast<string>(id) + "/alive";
    listUrl.push_back(pair<string, vector<string>>(probe, vector<string>())); 
    listsUrl.push_back(listUrl);
    listUrl.clear();

    vectors_Json jsonResource = vectors_Json();
    
    boost::function<void (vectors_Json)> functorHandleRequestProbeAlive = boost::bind(&ProbesWidget::handleJsonGet, this, _1);    
//    
    recursiveGetResources(listsUrl, functorHandleRequestProbeAlive, jsonResource);


    Wt::WApplication::instance()->triggerUpdate();
}

void ProbesWidget::updateText(long long probeId)
{
    if (m_mapProbesAlive[probeId])
    {        
        m_mapProbesWText[probeId]->setTextFormat(Wt::XHTMLUnsafeText);
        m_mapProbesWText[probeId]->setText("<i class='icon-ok'></i>");
    }
    else
    {
        m_mapProbesWText[probeId]->setTextFormat(Wt::XHTMLUnsafeText);
        m_mapProbesWText[probeId]->setText("<i class='icon-remove'></i>");
    }
	Wt::WApplication::instance()->triggerUpdate();
}

void ProbesWidget::threadSafeFunctionCall(long long id, int rowTable, int columnTable)
{
    Wt::WServer::instance()->post(Wt::WApplication::instance()->sessionId()
            , boost::bind(&ProbesWidget::updateAliveStates, this, id, rowTable, columnTable));
}

int ProbesWidget::addCustomResourceTable(long long probeId, int rowTable, int columnTable)
{
    if (columnTable == m_stateColumn)
    {        
        m_autoUpdate = false;
        /* Check if past element still present */
        if (m_mapProbesWText.find(probeId) != m_mapProbesWText.end())
        {
            m_mapProbesWText.erase(probeId);
        }
        if (m_mapProbesWTimer.find(probeId) != m_mapProbesWTimer.end())
        {
            m_mapProbesWTimer.erase(probeId);
        }
                
        /* Create new CheckBox */
        Wt::WText   *text = new Wt::WText();

        m_mapProbesWText.insert(make_pair(probeId, text));
        updateText(probeId);

        /* Create assiociated ressource table */
        getResourceTable()->elementAt(rowTable, columnTable)->addWidget(m_mapProbesWText[probeId]);
        getResourceTable()->elementAt(rowTable, columnTable)->setContentAlignment(Wt::AlignCenter);
        getResourceTable()->elementAt(rowTable, columnTable)->setWidth(Wt::WLength(5, Wt::WLength::Percentage));
        
        threadSafeFunctionCall(probeId, rowTable, columnTable);
        
        /* Set Timer value and Timer */
        if (m_mapProbesTimer[probeId] < 1)
        {
            m_mapProbesDelay.insert(make_pair(probeId, 60000));
        }
        else
        {
            m_mapProbesDelay.insert(make_pair(probeId, m_mapProbesTimer[probeId] * 1000));
        }
        
        Wt::WTimer *timer = new Wt::WTimer;
        m_mapProbesWTimer.insert(make_pair(probeId, timer));
        timer->setInterval(m_mapProbesDelay[probeId]);
        timer->timeout().connect(boost::bind(&ProbesWidget::threadSafeFunctionCall, this, probeId, rowTable, columnTable));
        timer->start();
        
        return ++columnTable;
    }
    else
    {
        return columnTable;
    }
    
}
