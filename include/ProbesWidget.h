/**
 * @file ProbesWidget.h
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

#ifndef PROBESWIDGET_H
#define	PROBESWIDGET_H

#include <Wt/WStandardItem>
#include <Wt/WStandardItemModel>
#include <tools/Enums.h>

#include "Conf.h"
#include "GlobalIncludeFile.h"
#include "AbstractPage.h"


class AbstractPage;

class ProbesWidget :
public AbstractPage 
{
public:
    ProbesWidget(Echoes::Dbo::Session *session, std::string apiUrl);
    virtual ~ProbesWidget();
    static int m_stateColumn;
    
private:
    Echoes::Dbo::Session        *session_;
    std::string                 apiUrl_;
    
    Wt::WComboBox               *m_unitTypeComboBox;
    Wt::WStandardItemModel      *m_unitTypeModel;
    Wt::WStandardItemModel      *m_probes;
    Wt::WStandardItemModel      *m_assets;
    Wt::WIOService              *m_thread;
    
    std::map<long long, bool>           m_mapProbesAlive;
    std::map<long long, int>            m_mapProbesTimer;
    std::map<long long, int>            m_mapProbesDelay;
    std::map<long long, Wt::WText*>     m_mapProbesWText;
    std::map<long long, Wt::WTimer*>    m_mapProbesWTimer;

    
    boost::mutex                m_mutex;
        
    void fillModel(Wt::WStandardItemModel * model, std::map<long long, std::string> m_mapNames);
    void fillModels();
    void addEnumToModel(Enums::EInformationUnitType enumToAdd, Wt::WString name);
    
    void handleJsonGet(vectors_Json jsonResources);
    void handleJsonGetHeartbeat(vector<Wt::Json::Value> jsonResource);
    void updateText(long long id);
    void updateAliveStates(long long id, int rowTable, int columnTable);
    void threadSafeFunctionCall(long long id, int rowTable, int columnTable);
    int addCustomResourceTable(long long id, int rowTable, int columnTable);
protected:
    void modifResource(vector<Wt::WInteractWidget*>* arguments, long long id);
};

#endif	/* PROBESWIDGET_H */
