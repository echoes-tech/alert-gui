/**
 * @file PluginsTableFilterWidget.h
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

#ifndef PLUGINSTABLEFILTERWIDGET_H
#define	PLUGINSTABLEFILTERWIDGET_H

#include <Wt/WStandardItem>
#include <Wt/WStandardItemModel>

#include <tools/Enums.h>

#include "AbstractPage.h"
#include "PluginsTableSearchWidget.h"


class AbstractPage;

class PluginsTableFilterWidget :
public AbstractPage
{    
    struct FilterData
    {
        long long filterTypeID;
        std::map<long long, Wt::WString> parametersValue;
        int nbValue, posKeyValue;
    };
    
public:
                                                PluginsTableFilterWidget(Echoes::Dbo::Session *session, std::string apiUrl,
                                                    PluginsTableSearchWidget* pluginsTableSearchWidget);
    void                                        updatePage();
    
protected:                                 
    std::vector<Wt::WInteractWidget*>           initRowWidgets(Wt::Json::Object jsonObject, std::vector<Wt::Json::Value> jsonResource, int cpt);
    Wt::WString                                 getFilterParameterName(long long searchParameterID);
    Wt::WString                                 getFilterTypeName(long long searchTypeID);
    void                                        addPopupAddHandler(Wt::WInteractWidget* widget);
    void                                        addPopupModifHandler(Wt::WInteractWidget* widget, long long typeID);
    void                                        addResourcePopup(long long filterID);
    void                                        sendRequestPopupAdd(boost::function<void (Wt::Json::Value)> functor, Wt::WComboBox* filterTypeComboBox,
                                                    long long filterID);    
    void                                        handleRequestPopupAdd(Wt::Json::Value result, Wt::WContainerWidget* paramsContainer,
                                                    std::vector<Wt::WInteractWidget*>* inputName, long long filterID);
    void                                        fillModel();
    void                                        setAddResourceMessage(Wt::Http::Message *message, std::vector<Wt::WInteractWidget*>* argument);
    void                                        setModifResourceMessage(Wt::Http::Message *message, std::vector<Wt::WInteractWidget*>* argument);
    
private:
    Echoes::Dbo::Session                        *session_;
    std::string                                 apiUrl_;
    PluginsTableSearchWidget*                   m_pluginsTableSearchWidget;
    long long                                   m_selectedSearchID;
    Wt::WStandardItemModel*                     m_filterTypeStandardItemModel;
    std::map<long long, FilterData>             m_filtersData;
};



#endif	/* PLUGINSTABLEFILTERWIDGET_H */
