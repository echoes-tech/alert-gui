/**
 * @file MessagesTableMessageWidget.h
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

#ifndef MESSAGESTABLEMESSAGEWIDGET_H
#define	MESSAGESTABLEMESSAGEWIDGET_H

#include <Wt/WStandardItem>
#include <Wt/WStandardItemModel>

#include <tools/Enums.h>

#include "AbstractPage.h"

#include "MessagesTableAlertWidget.h"

class AbstractPage;

class MessagesTableMessageWidget :
public AbstractPage
{
    struct MessageData
    {
        long long messageID;
        long long stateID;
        Wt::WString date;
        Wt::WString dest;
    };

public:
    MessagesTableMessageWidget(Echoes::Dbo::Session *session, std::string apiUrl, MessagesTableAlertWidget* alertWidget);
    void                                        updatePage();
    long long                                   getSelectedMessageAddonID();
    Wt::WString                                 getSelectedMessageRecipient();
protected:    
    //void                                        setAddResourceMessage(Wt::Http::Message *message, std::vector<Wt::WInteractWidget*>* argument);
    //void                                        setModifResourceMessage(Wt::Http::Message *message, std::vector<Wt::WInteractWidget*>* argument);
    /*void                                        addResourcePopup(long long sourceID);
    void                                        sendRequestPopupAdd(boost::function<void (Wt::Json::Value)> functor, Wt::WComboBox* addonComboBox,
                                                    long long sourceID);
    void                                        addPopupAddHandler(Wt::WInteractWidget* widget);
    void                                        addPopupModifHandler(Wt::WInteractWidget* widget, long long sourceID);
    void                                        handleRequestPopupAdd(Wt::Json::Value result, Wt::WContainerWidget* paramsContainer,
                                                    std::vector<Wt::WInteractWidget*>* inputName, long long sourceID);
    Wt::WString                                 getSourceParameterName(long long sourceParameterID);
     * 
    */
    Wt::WContainerWidget*                      createTableFirstHeader();
    Wt::WString                                 getStateName(long long stateID);
    std::vector<Wt::WInteractWidget*>           initRowWidgets(Wt::Json::Object jsonObject, std::vector<Wt::Json::Value> jsonResource, int cpt);
    
private:
    Echoes::Dbo::Session                        *session_;
    std::string                                 apiUrl_;
    MessagesTableAlertWidget*                   m_messagesTableAlertWidget;
    long long                                   m_selectedAlertID;
    Wt::WStandardItemModel*                     m_addonStandardItemModel;
    std::map<long long, MessageData>             m_messagesData;
};



#endif	/* MESSAGESTABLEMESSAGEWIDGET_H */
