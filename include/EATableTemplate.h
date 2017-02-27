/**
 * @file EATableTemplate.h
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

#ifndef EATABLETEMPLATE_H
#define	EATABLETEMPLATE_H

#include <Wt/WApplication>
#include <Wt/WTemplate>
#include <Wt/WPushButton>
#include <Wt/WLineEdit>
#include <Wt/WString>
#include <Wt/WText>
#include <Wt/WTable>
#include <Wt/WComboBox>
#include <Wt/WTemplate>
#include <Wt/WNavigationBar>

class EATableTemplate : public Wt::WTemplate
{
public:
    EATableTemplate(const Wt::WString& tableName, Wt::WContainerWidget* parent);
    EATableTemplate(const EATableTemplate& orig);
    virtual ~EATableTemplate();
private:
    void                        createTable();
//    void                        createEmptyResourceTable();
    Wt::WContainerWidget        *createTableHeader();
    Wt::WContainerWidget        *createTableBody();
    Wt::WContainerWidget        *createTableFooter();
    void                        addTableColumnHeader();
    Wt::WComboBox               *createDisplayedLinesComboBox();
    void                        fillTableBody();
    void                        searchInTable();
    
    virtual void                        headerPlusButtonBehaviour();
    
    Wt::WTable                          *m_mainTable;
    Wt::WString                         m_tableName;
    int                                 m_numberOfLinesToDisplay;
    Wt::WPushButton                     *m_headerPlusButton;
    Wt::WLineEdit                       *m_searchWidget;
    bool                                m_hasModifButton;
    bool                                m_hasDeleteButton;
    Wt::WContainerWidget                *m_footer;
    std::vector<std::pair <int, std::string>>     m_titles;

};

#endif	/* EATABLETEMPLATE_H */
