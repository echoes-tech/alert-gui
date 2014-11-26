/* 
 * File:   BoxInBoxMenu.h
 * Author: vpl
 *
 * Created on 19 septembre 2014, 11:30
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
