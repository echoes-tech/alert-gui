/**
 * @file TrundleTable.h
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

#ifndef BOXINBOXMENU_H
#define	BOXINBOXMENU_H


#include <vector>

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/system/error_code.hpp>

#include "tools/Session.h"

#include "GlobalIncludeFile.h"

#include <Wt/WWidget>
#include <Wt/WTemplate>
#include <Wt/WTable>
#include <Wt/WColor>
#include <Wt/WStandardItemModel>
#include <Wt/WCssDecorationStyle>

class TrundleTable {
public:
    TrundleTable(Wt::WTable *holder);
    TrundleTable(Wt::WTable *holder
        , std::vector<Wt::WLength> widths, Wt::WLength height);
    TrundleTable(const TrundleTable& orig);
    virtual ~TrundleTable();
    
    void deleteAll();
    void deleteRow(long long index);
    void addRow(long long id, long long type
    , std::vector<Wt::WWidget*> widgets
    , std::map<int, bool> buttons
    , std::map<int, boost::function<void (long long id, long long index, TrundleTable*)>> interactions
    , TrundleTable *subMenu);
    void modifyRow(long long index, long long id, long long type, std::vector<Wt::WWidget*> widgets);
    void modifyRowCell(long long index, long long cell, Wt::WWidget *newWidget);
    void updateRowColor(long long y, long long x);
    void updateTable();
    long long getIndex();
    bool rowIdExist(long long id);
    TrundleTable* getSubMenu(long long id);
    int getColumnSpan(std::vector<Wt::WWidget*>::const_iterator itC, std::vector<Wt::WWidget*>::const_iterator itEnd);
    
    
    enum ERowType {
        HEADER = 0,
        ITEM = 1
    };
    
    enum EColor {
        HIGHLIGHT = 0,
        DEFAULT = 1,
        HOVER = 2
    };
    
    BOOST_ENUM_VALUES(EInteractions, const char*,
                      (ADD) ("add")
                      (MODIF) ("modif")
                      (REMOVE) ("remove")
                      (SELECT) ("select")
                      )
    
    struct Row {
        long long                       id;
        long long                       type;
        long long                       index;
        TrundleTable                    *subMenu;
        std::vector<Wt::WWidget*>       widgets; //cells
        std::map<int, boost::function<void (long long id, long long index, TrundleTable*)>> interactions;
    };
    
    std::vector<Wt::WWidget*>       m_header;
    Wt::WTable                      *m_table;
    long long                       parentId;
    
private:
    bool m_formatted;
    long long m_columnNumber;
    long long m_rowNumber;
    long long m_currentIndex;
    Wt::WLength m_heightFormat;
    std::vector<Wt::WLength> m_widthFormat;
    std::vector<Wt::WColor*> m_colors;
    std::vector<struct Row> m_rows;
};

#endif	/* BOXINBOXMENU_H */
