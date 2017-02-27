/**
 * @file PluginsTableAssociationWidget.h
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

#ifndef PLUGINSTABLEASSOCIATIONWIDGET_H
#define	PLUGINSTABLEASSOCIATIONWIDGET_H

#include <Wt/WStandardItem>
#include <Wt/WStandardItemModel>

#include <tools/Enums.h>

#include "AbstractPage.h"
#include "PluginsTableFilterWidget.h"

class AbstractPage;

class PluginsTableAssociationWidget :
public AbstractPage
{
    struct SourceData
    {
        long long addonID;
        std::map<long long, Wt::WString> parametersValue;
    };

public:
                                                PluginsTableAssociationWidget(Echoes::Dbo::Session *session, std::string apiUrl,
                                                    PluginsTableFilterWidget* pluginsTableFilterWidget);
    void                                        updatePage();
    
protected:    
    void                                        setAddResourceMessage(Wt::Http::Message *message, std::vector<Wt::WInteractWidget*>* argument);
    void                                        addPopupAddHandler(Wt::WInteractWidget* widget);
    void                                        handleRequestPopupAdd(Wt::Json::Value result, Wt::WContainerWidget* paramsContainer,
                                                    std::vector<Wt::WInteractWidget*>* inputName, long long sourceID);
    void                                        addResourcePopup();
    std::vector<Wt::WInteractWidget*>           initRowWidgets(Wt::Json::Object jsonObject, std::vector<Wt::Json::Value> jsonResource, int cpt);
    void                                        fillHostComboBox(Wt::Json::Value result, boost::function<void()> functorSendRequestInfoAndFilterComboBox,
                                                    Wt::WDialog* dialog);
    void                                        sendRequestInfoAndFilterIndexComboBox(boost::function<void (vectors_Json)> functorFillInfoAndFilterComboBox,
                                                    Wt::WComboBox* assetComboBox);
    void                                        fillInfoAndFilterIndexComboBox(vectors_Json jsonResources, Wt::WDialog* dialog);
    
private:
    Echoes::Dbo::Session                        *session_;
    std::string                                 apiUrl_;
    PluginsTableFilterWidget*                   m_pluginsTableFilterWidget;
    long long                                   m_selectedFilterID;
    std::map<long long, SourceData>             m_sourcesData;
    
    Wt::WStandardItemModel*                     m_assetStandardItemModel;
    Wt::WStandardItemModel*                     m_infoStandardItemModel;
    Wt::WStandardItemModel*                     m_filterIndexStandardItemModel;
};



#endif	/* PLUGINSTABLEASSOCIATIONWIDGET_H */
