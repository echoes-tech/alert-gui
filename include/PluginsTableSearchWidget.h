/**
 * @file PluginsTableSearchWidget.h
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

#ifndef PLUGINSTABLESEARCHWIDGET_H
#define	PLUGINSTABLESEARCHWIDGET_H

#include <Wt/WStandardItem>
#include <Wt/WStandardItemModel>

#include <tools/Enums.h>

#include "AbstractPage.h"
#include "PluginsTableSourceWidget.h"


class AbstractPage;

class PluginsTableSearchWidget :
public AbstractPage
{    
    struct SearchData
    {
        long long searchTypeID;
        std::map<long long, Wt::WString> parametersValue;
        int period;
    };
    
public:
                                                PluginsTableSearchWidget(Echoes::Dbo::Session *session, std::string apiUrl,
                                                    PluginsTableSourceWidget* pluginsTableSourceWidget);
    void                                        updatePage();
    
protected:    
    void                                        setAddResourceMessage(Wt::Http::Message *message, std::vector<Wt::WInteractWidget*>* argument);
    void                                        setModifResourceMessage(Wt::Http::Message *message, std::vector<Wt::WInteractWidget*>* argument);
    void                                        addResourcePopup(long long searchID);
    void                                        addPopupAddHandler(Wt::WInteractWidget* widget);
    void                                        addPopupModifHandler(Wt::WInteractWidget* widget, long long searchID);
    void                                        sendRequestPopupAdd(boost::function<void (Wt::Json::Value)> functor, Wt::WComboBox* searchTypeComboBox,
                                                    long long searchID);
    void                                        handleRequestPopupAdd(Wt::Json::Value result, Wt::WContainerWidget* paramsContainer,
                                                    std::vector<Wt::WInteractWidget*>* inputName, long long searchID);
    void                                        fillModel(Wt::Json::Value result, Wt::WComboBox* searchTypeComboBox, long long searchID,
                                                    boost::function<void (Wt::Json::Value)> functorHandleRequestPopupAdd);
    Wt::WString                                 getSearchParameterName(long long searchParameterID);
    Wt::WString                                 getSearchTypeName(long long searchTypeID);
    std::vector<Wt::WInteractWidget*>           initRowWidgets(Wt::Json::Object jsonObject, std::vector<Wt::Json::Value> jsonResource, int cpt);
    
private:
    Echoes::Dbo::Session                        *session_;
    std::string                                 apiUrl_;
    PluginsTableSourceWidget*                   m_pluginsTableSourceWidget;
    long long                                   m_selectedSourceID;
    Wt::WStandardItemModel*                     m_searchTypeStandardItemModel;
    std::map<long long, SearchData>             m_searchesData;
};



#endif	/* PLUGINSTABLESEARCHWIDGET_H */
