/* 
 * File:   BoxInBoxMenu.cpp
 * Author: vpl
 * 
 * Created on 19 septembre 2014, 11:30
 */

#include <vector>
#include <typeinfo>

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
#include <bits/stl_map.h>
#include <bits/stl_vector.h>

#include "TrundleTable.h"

TrundleTable::TrundleTable(Wt::WTable *holder)
{
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

TrundleTable::TrundleTable(Wt::WTable *holder, std::vector<Wt::WLength> widths, Wt::WLength height)
{
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

TrundleTable::TrundleTable(const TrundleTable& orig)
{
}

TrundleTable::~TrundleTable()
{
}

void TrundleTable::addRow(long long id, long long type
                          , std::vector<Wt::WWidget*> widgets
                          , std::map<int, bool> buttons
                          , std::map<int, boost::function<void (long long id, long long index, TrundleTable*) >> interactions
                          , TrundleTable *subMenu)
{
    for (std::vector<struct Row>::const_iterator itR = m_rows.begin(); itR != m_rows.end(); itR++)
    {
        if (itR->id == id && itR->type != ERowType::HEADER)
        {
            if (subMenu != NULL)
            {
                subMenu->deleteAll();
            }
            return;
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
            newRow.widgets.push_back(NULL);
        }
    }

    /* Setting buttons */
    for (std::map<int, bool>::const_iterator itB = buttons.begin(); itB != buttons.end(); itB++)
    {
        EInteractions::const_iterator itEB = EInteractions::begin() + itB->first;
        std::string templateButton(itEB->value());
        Wt::WTemplate *t = new Wt::WTemplate(Wt::WString::tr("Alert.box-menu." + templateButton + "-button"));
        switch (itB->first)
        {
        case EInteractions::ADD:
        {
            t->clicked().connect(bind([ = ] ()
            {
                newRow.interactions.find(EInteractions::ADD)->second(m_rows.at(m_currentIndex).id, m_currentIndex, this);
            }));
            break;
        }
        case EInteractions::REMOVE:
        {
            t->clicked().connect(bind([ = ] ()
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

    if ((unsigned long long) m_columnNumber < newRow.widgets.size())
    {
        m_columnNumber = newRow.widgets.size();
    }

    m_currentIndex = m_rows.size() - 1;

    updateTable();
}

void TrundleTable::modifyRow(long long index, long long id, long long type, std::vector<Wt::WWidget*> widgets)
{
    m_rows.at(index).id = id;
    m_rows.at(index).type = type;
    m_rows.at(index).widgets.clear();
    m_rows.at(index).widgets = widgets;

    updateTable();
}

void TrundleTable::modifyRowCell(long long index, long long cell, Wt::WWidget *newWidget)
{
    m_rows.at(index).widgets.at(cell) = newWidget;

    updateTable();
}

void TrundleTable::deleteRow(long long index)
{
    /* delete from table */
    if (!m_rows.empty())
    {
        std::vector<struct Row>::iterator it = m_rows.begin();
        std::advance(it, index);

        for (long unsigned i = 0; i < it->widgets.size(); i++)
        {
            if (it->widgets.at(i) != NULL)
            {
                m_table->elementAt(index, i)->removeWidget(it->widgets.at(i));
            }
        }
        it->widgets.clear();

        if (it->subMenu != NULL)
        {
            it->subMenu->deleteAll();
        }

        m_rows.erase(it);
        m_rowNumber--;

        for (std::vector<struct Row>::iterator itR = (m_rows.begin() + index); itR != m_rows.end(); itR++)
        {
            itR->index--;
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
    updateTable();
}

void TrundleTable::updateRowColor(long long y, long long x)
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

void TrundleTable::deleteAll()
{
    while (m_rows.size())
    {
        if (m_rows.at(0).interactions.find(EInteractions::REMOVE) != m_rows.at(0).interactions.end())
        {
            m_rows.at(0).interactions.at(EInteractions::REMOVE)(m_rows.at(0).id, m_rows.at(0).index, this);
        }
        else
        {
            deleteRow(0);
        }
    }

    m_rows.clear();
    m_table->clear();
    m_table->hide();
}

int TrundleTable::getColumnSpan(std::vector<Wt::WWidget*>::const_iterator itC, std::vector<Wt::WWidget*>::const_iterator itEnd)
{
    int ret = 1;

    itC++;
    while (itC != itEnd)
    {
        if (*itC != NULL)
        {
            break;
        }
        ret++;
        itC++;
    }
    return (ret);
}

void TrundleTable::updateTable()
{
    long long x = 0;
    long long y = 0;

    m_table->setStyleClass("table");
    m_table->addStyleClass("table-responsive");

    for (std::vector<struct Row>::iterator itR = m_rows.begin(); itR != m_rows.end(); itR++)
    {
        x = 0;
        bool doRemove = false;
        if (y < m_table->rowCount())
        {
            doRemove = true;
        }
        for (std::vector<Wt::WWidget*>::const_iterator itC = itR->widgets.begin(); itC != itR->widgets.end(); itC++)
        {
            if (*itC == NULL)
            {
                continue;
            }
            long columnSpan = getColumnSpan(itC, itR->widgets.end());
            m_table->elementAt(y, x)->setColumnSpan(columnSpan);
            if (doRemove)
            {
                m_table->elementAt(y, x)->removeWidget(*itC);
            }

            if ((Wt::WText*) dynamic_cast<Wt::WText*> (*itC))
            {
                if (!((Wt::WText*)(*itC))->text().toUTF8().empty())
                {
                    m_table->elementAt(y, x)->setPadding(Wt::WLength(5), Wt::Left);
                    m_table->elementAt(y, x)->addWidget(*itC);
                }
            }
            else
            {
                m_table->elementAt(y, x)->addWidget(*itC);
            }
            if (m_formatted)
            {
                m_table->elementAt(y, x)->resize(m_widthFormat.at(x), m_heightFormat);
            }
            m_table->elementAt(y, x)->setVerticalAlignment(Wt::AlignmentFlag::AlignMiddle);
            updateRowColor(y, x);
            x += columnSpan;
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
                m_rows.at(i).subMenu->m_table->show();
                m_rows.at(i).subMenu->updateTable();
            }
            else
            {
                m_rows.at(i).subMenu->m_table->hide();
            }
        }
    }
}

long long TrundleTable::getIndex()
{
    return (m_currentIndex);
}

bool TrundleTable::rowIdExist(long long id)
{
    for (std::vector<struct Row>::const_iterator itR = m_rows.begin(); itR != m_rows.end(); itR++)
    {
        if (itR->id == id)
        {
            return (true);
        }
    }
    return (false);
}

TrundleTable* TrundleTable::getSubMenu(long long id)
{
    for (std::vector<struct Row>::const_iterator itR = m_rows.begin(); itR != m_rows.end(); itR++)
    {
        if (itR->id == id)
        {
            return (itR->subMenu);
        }
    }
    return (NULL);
}