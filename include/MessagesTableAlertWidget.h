/**
 * @file MessagesTableAlertWidget.h
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

#ifndef MESSAGESTABLEALERTWIDGET_H
#define	MESSAGESTABLEALERTWIDGET_H

#include <Wt/WStandardItem>
#include <Wt/WStandardItemModel>
#include <Wt/WAbstractItemModel>
#include <Wt/WTimer>

#include <tools/Enums.h>

#include "AbstractPage.h"

class AbstractPage;

class MessagesTableAlertWidget :
public AbstractPage
{
    struct AlertData
    {
        long long alertID;
        Wt::WString name;
        long long stateID;
        Wt::WString user;
    };
    
public:
    MessagesTableAlertWidget(Echoes::Dbo::Session *session, std::string apiUrl);
    void                                        updatePage();
    long long                                   getSelectedAlertID();
    Wt::WString                                 getSelectedAlertName();
protected:
    //virtual Wt::WComboBox       *popupAdd(Wt::WDialog *dialog);
    std::vector<Wt::WInteractWidget*>           initRowWidgets(Wt::Json::Object jsonObject, std::vector<Wt::Json::Value> jsonResource, int cpt);
    Wt::WString                                 getStateName(long long stateID);
    Wt::WString                                 getStateColor(long long stateID);
    
    
private:
    void                        takeAssignement(long long id);
    void                        setResolved(long long id);
    void                        forward(long long id);
    Echoes::Dbo::Session        *session_;
    std::string                 apiUrl_;
    Wt::WStandardItemModel      *m_assetsStandardItemModel;
    Wt::WComboBox               *m_assetComboBox;
    Wt::WTimer*                 timer_refresh;
    Wt::WStandardItemModel      *usersModel_;
    virtual int                 addCustomButtonsToResourceTable(long long id, int rowTable, int columnTable);
    std::map<long long, AlertData>             m_alertsData;
    std::map<long long, std::string>            m_listUsr;
    void                        assignementCallBack(boost::system::error_code err, const Wt::Http::Message& response, Wt::Http::Client *client);
    void                        resolveCallBack(boost::system::error_code err, const Wt::Http::Message& response, Wt::Http::Client *client);
    void                        refresh();
    void                        handleJsonGet(vectors_Json jsonResources);
    void                        customButtonFooter(Wt::WDialog *dialog);
    void                        popupForward(std::vector<Wt::WInteractWidget*>* inputName, std::vector<Wt::WText*> errorMessage,
                                    Wt::WDialog *dialog, long long id);
    void                        messageBoxError();
    //void fillModel(Wt::Json::Value result);
    //virtual void setAddResourceMessage(Wt::Http::Message *message, std::vector<Wt::WInteractWidget*>* argument);
    //virtual void setModifResourceMessage(Wt::Http::Message *message, std::vector<Wt::WInteractWidget*>* argument);
};



#endif	/* MESSAGESTABLEALERTWIDGET_H */
