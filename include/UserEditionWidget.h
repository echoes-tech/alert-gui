/**
 * @file UserEditionWidget.h
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

#ifndef USEREDITIONWIDGET_H
#define	USEREDITIONWIDGET_H

#include <Wt/WStandardItem>
#include <Wt/WStandardItemModel>
#include <Wt/WAbstractItemModel>

#include "AbstractPage.h"

typedef std::map<long long, std::string>        MapLongString;

class AbstractPage;

class UserEditionWidget :
public AbstractPage
{
public:
    UserEditionWidget(Echoes::Dbo::Session *session, std::string apiUrl, int type);

//    Wt::WValidator              *editValidator(int who);
    
    void                        addResource(std::vector<Wt::WInteractWidget*>* argument);
    void                        modifResource(std::vector<Wt::WInteractWidget*>* arguments, long long id);

    virtual Wt::WComboBox       *popupAdd(Wt::WDialog *dialog);

protected:
    virtual void                handleJsonGet(vectors_Json jsonResources);

private:

    Echoes::Dbo::Session        *session_;
    std::string                 apiUrl_;
    Wt::Json::Value             result_;
    int                         type_;  
    MapLongString               mediasTokens;
    Wt::WStandardItemModel      *usersModel_;
};



#endif	/* USEREDITIONWIDGET_H */
