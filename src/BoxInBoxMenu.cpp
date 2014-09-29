/* 
 * File:   BoxInBoxMenu.cpp
 * Author: vpl
 * 
 * Created on 19 septembre 2014, 11:30
 */

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
#include <Wt/WCssDecorationStyle>

#include "BoxInBoxMenu.h"

BoxInBoxMenu::BoxInBoxMenu(Wt::WTable *holder) {
    Wt::log("info") << "Constructing non-formated BoxInBoxMenu";
    Wt::WApplication *app = Wt::WApplication::instance();
    app->messageResourceBundle().use(AbstractPage::xmlDirectory + "box_menu", false);
    m_currentIndex = 0;
    
    /* Setting default colors */
    m_colors.push_back(new Wt::WColor(102, 178, 255));
    m_colors.push_back(new Wt::WColor(255, 255, 255));
    m_colors.push_back(new Wt::WColor(0, 128, 255));
    
    m_formatted = false;
    m_rowNumber = 0;
    m_columnNumber = 0;
    
    m_table = holder;
}

BoxInBoxMenu::BoxInBoxMenu(Wt::WTable *holder
        , std::vector<Wt::WLength> widths, Wt::WLength height) {
    Wt::log("info") << "Constructing formated BoxInBoxMenu";
    Wt::WApplication *app = Wt::WApplication::instance();
    app->messageResourceBundle().use(AbstractPage::xmlDirectory + "box_menu", false);
    m_currentIndex = 0;
    
    /* Setting default colors */
    m_colors.push_back(new Wt::WColor(102, 178, 255));
    m_colors.push_back(new Wt::WColor(255, 255, 255));
    m_colors.push_back(new Wt::WColor(0, 128, 255));
    
    m_formatted = true;
    m_heightFormat = height;
    m_widthFormat = widths;
    
    m_columnNumber = m_widthFormat.size();
    m_rowNumber = 0;
    
    m_table = holder;
}

BoxInBoxMenu::BoxInBoxMenu(const BoxInBoxMenu& orig) {
}

BoxInBoxMenu::~BoxInBoxMenu() {
}

void BoxInBoxMenu::addRow(long long id, long long type
        , std::vector<Wt::WWidget*> widgets
        , std::map<int, bool> buttons
        , std::map<int, boost::function<void (long long id, long long index, BoxInBoxMenu*)>> interactions
        , BoxInBoxMenu *subMenu)
{    
    Wt::log("info") << "addRow: id " << id << " | type " << type;
    for (std::vector<struct Row>::const_iterator itR = m_rows.begin() ; itR != m_rows.end() ; itR++)
    {
        if (itR->id == id && itR->type != ERowType::HEADER)
        {
            Wt::log("warning") << "id already registered";
            if (subMenu != NULL)
            {
                subMenu->deleteAll();
            }
            return ;
        }
    }
    struct Row newRow;
    
    newRow.id = id;
    newRow.type = type;
    newRow.index = m_rows.size();
    newRow.subMenu = subMenu;
    newRow.widgets = widgets;
    newRow.interactions = interactions;
    
    if (m_columnNumber - 1 > 0)
    {
        while (newRow.widgets.size() < (unsigned long long) (m_columnNumber - 1))
        {
            newRow.widgets.push_back(new Wt::WText(""));
        }
    }
    
    /* Setting buttons */
    Wt::log("info") << "Setting buttons";
    for (std::map<int, bool>::const_iterator itB = buttons.begin() ; itB != buttons.end() ; itB++)
    {
        EInteractions::const_iterator itEB = EInteractions::begin() + itB->first;
        std::string templateButton(itEB->value());
        Wt::WTemplate *t = new Wt::WTemplate(Wt::WString::tr("Alert.box-menu." + templateButton + "-button"));
        switch (itB->first)
        {
            case EInteractions::ADD:
            {
                Wt::log("info") << "Add button";
                t->clicked().connect(bind([ = ] ()
                {
                    newRow.interactions.find(EInteractions::ADD)->second(m_rows.at(m_currentIndex).id, m_currentIndex, this);
                }));
                break;
            }
            case EInteractions::REMOVE:
            {
                t->clicked().connect(bind ([ = ] ()
                {
                    long long index = newRow.index;

                    while (index > -1)
                    {
                        if (index < (long long) m_rows.size())
                        {
                            if (newRow.id != m_rows.at(index).id)
                            {
                                index--;
                            }
                            else
                            {
                                Wt::log("info") << "clicked delete on index: " << index;
                                newRow.interactions.find(EInteractions::REMOVE)->second(m_rows.at(index).id, index, this);
                                break;
                            }
                        }
                        else
                        {
                            index--;
                        }
                    }
                }));
                break;
            }
        }
        newRow.widgets.push_back(t);
    }
    if (newRow.type == HEADER)
    {
        m_header = newRow.widgets;
    }
    if (newRow.interactions.find(EInteractions::SELECT) != newRow.interactions.end())
    {
        newRow.interactions.find(EInteractions::SELECT)->second(newRow.id, newRow.index, this);
    }
    m_rows.push_back(newRow);
    
    m_rowNumber++;
    
    if ((unsigned long long)m_columnNumber < newRow.widgets.size())
    {
        m_columnNumber = newRow.widgets.size();
    }
    
    m_currentIndex = m_rows.size() - 1;
    
    updateTable();
}

void BoxInBoxMenu::modifyRow(long long index, long long id, long long type, std::vector<Wt::WWidget*> widgets)
{
    m_rows.at(index).id = id;
    m_rows.at(index).type = type;
    m_rows.at(index).widgets.clear();
    m_rows.at(index).widgets = widgets;
    
    updateTable();
}

void BoxInBoxMenu::modifyRowCell(long long index, long long cell, Wt::WWidget *newWidget)
{
    m_rows.at(index).widgets.at(cell) = newWidget;
    
    updateTable();
}

void BoxInBoxMenu::deleteRow(long long index)
{
    Wt::log("info") << "delete row: " << index;
    /* delete from table */
    if (!m_rows.empty())
    {
        std::vector<struct Row>::iterator it = m_rows.begin();
        std::advance(it, index);

        for (long unsigned i = 0; i < it->widgets.size(); i++)
        {
            m_table->elementAt(index, i)->removeWidget(it->widgets.at(i));
        }
        it->widgets.clear();

        if (it->subMenu != NULL)
        {
            it->subMenu->deleteAll();
            Wt::log("info") << "subMenu erased";
        }

        m_rows.erase(it);
        Wt::log("info") << "row erased";
        m_rowNumber--;

        for (std::vector<struct Row>::iterator itR = (m_rows.begin() + index) ; itR != m_rows.end() ; itR++)
        {
            Wt::log("info") << "row id before: " << itR->index;
            itR->index--;
            Wt::log("info") << "row id after: " << itR->index;
        }
        /* set new current index */
        if (m_rowNumber > 0)
        {
            if (index == m_currentIndex)
            {
                m_currentIndex = index - 1;
            }
            else if (m_currentIndex > index)
            {
                m_currentIndex--;
            }
        }
        else
        {
            m_currentIndex = 0;
        }
    }
    Wt::log ("info") << "row number: " << m_rowNumber;
    updateTable();
}

void BoxInBoxMenu::updateRowColor(long long y, long long x)
{
    /* change color if focus is on index */         
    
    if (m_currentIndex == y && m_rows.at(y).type != ERowType::HEADER)
    {
        m_table->elementAt(y, x)->clicked().connect(bind([ = ] ()
        {
            if (y != m_currentIndex)
            {
                m_currentIndex = y;
                updateTable();
            }
            if (m_rows.at(y).interactions.find(EInteractions::SELECT) != m_rows.at(y).interactions.end())
            {
                Wt::log("info") << "Row selected";
                m_rows.at(y).interactions.find(EInteractions::SELECT)->second(m_rows.at(y).id, y, this);
            }
        }));
        if (m_rows.at(y).interactions.find(EInteractions::SELECT) != m_rows.at(y).interactions.end())
        {
            m_rows.at(y).interactions.find(EInteractions::SELECT)->second(m_rows.at(y).id, y, this);
        }
        m_table->elementAt(y, x)->decorationStyle().setBackgroundColor(*m_colors.at(HIGHLIGHT));
    }
    else
    {
        m_table->elementAt(y, x)->decorationStyle().setBackgroundColor(*m_colors.at(DEFAULT));
    }
    
    /* change color if mouse is over cell */
    if (m_rows.at(y).type != ERowType::HEADER)
    {
        m_table->elementAt(y, x)->addStyleClass("table table-hover", true);
    }
}

void BoxInBoxMenu::deleteAll()
{
    Wt::log("info") << "deleting " << m_rows.size() << " rows";
    while (m_rows.size())
    {
        deleteRow(0);
    }
    
    m_rows.clear();
    m_table->clear();
    m_table->hide();
}

void BoxInBoxMenu::updateTable()
{
    long long x = 0;
    long long y = 0;
    
    m_table->setStyleClass("table");
    m_table->addStyleClass("table-responsive");
    
    Wt::log("info") << "updateTable " << m_table->rowCount();
    
    for (std::vector<struct Row>::iterator itR = m_rows.begin() ; itR != m_rows.end() ; itR++)
    {
        x = 0;
        bool doRemove = false;
        if (y < m_table->rowCount())
        {
            doRemove = true;
        }
        for (std::vector<Wt::WWidget*>::const_iterator itC = itR->widgets.begin(); itC != itR->widgets.end() ; itC++)
        {
            if (doRemove)
            {
                m_table->elementAt(y, x)->removeWidget(*itC);
            }
            if (m_formatted == true)
            {
                m_table->elementAt(y, x)->setHeight(m_heightFormat);
                m_table->elementAt(y, x)->setWidth(m_widthFormat.at(x));
                m_table->elementAt(y, x)->setContentAlignment(Wt::AlignmentFlag::AlignCenter);
            }

            m_table->elementAt(y, x)->addWidget(*itC);

            updateRowColor(y, x);
            x++;
        }
        y++;
    }

    while (y < m_table->rowCount())
    {
        m_table->deleteRow(y);
        y++;
    }
    
    for (long long i = 1; i < m_rowNumber; i++)
    {
        if (m_rows.at(i).subMenu != NULL)
        {
            if (i == m_currentIndex)
            {
                Wt::log("info") << "update SubMenu";
                m_rows.at(i).subMenu->m_table->show();
                m_rows.at(i).subMenu->updateTable();
            }
            else
            {
                m_rows.at(i).subMenu->m_table->hide();
            }
        }
    }
    Wt::log("info") << "update table done";
}

long long BoxInBoxMenu::getIndex()
{
    return (m_currentIndex);
}

bool BoxInBoxMenu::rowIdExist(long long id)
{
    for (std::vector<struct Row>::const_iterator itR = m_rows.begin() ; itR != m_rows.end() ; itR++)
    {
        if (itR->id == id)
        {
            Wt::log("info") << "id | itR->id: " << id << " | " << itR->id;
            return (true);
        }
    }
    return (false);
}

BoxInBoxMenu* BoxInBoxMenu::getSubMenu(long long id)
{
    for (std::vector<struct Row>::const_iterator itR = m_rows.begin() ; itR != m_rows.end() ; itR++)
    {
        if (itR->id == id)
        {
            return (itR->subMenu);
        }
    }
    return (NULL);
}