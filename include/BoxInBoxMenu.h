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

class BoxInBoxMenu {
public:
    BoxInBoxMenu(Wt::WTable *holder);
    BoxInBoxMenu(Wt::WTable *holder
        , std::vector<Wt::WLength> widths, Wt::WLength height);
    BoxInBoxMenu(const BoxInBoxMenu& orig);
    virtual ~BoxInBoxMenu();
    
    void deleteAll();
    void deleteRow(long long index);
    void addRow(long long id, long long type
    , std::vector<Wt::WWidget*> widgets
    , std::map<int, bool> buttons
    , std::map<int, boost::function<void (long long id, long long index, BoxInBoxMenu*)>> interactions
    , BoxInBoxMenu *subMenu);
    void modifyRow(long long index, long long id, long long type, std::vector<Wt::WWidget*> widgets);
    void modifyRowCell(long long index, long long cell, Wt::WWidget *newWidget);
    void updateRowColor(long long y, long long x);
    void updateTable();
    long long getIndex();
    bool rowIdExist(long long id);
    BoxInBoxMenu* getSubMenu(long long id);
    
    
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
        BoxInBoxMenu                    *subMenu;
        std::vector<Wt::WWidget*>       widgets; //cells
        std::map<int, boost::function<void (long long id, long long index, BoxInBoxMenu*)>> interactions;
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

