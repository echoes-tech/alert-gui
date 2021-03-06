/**
 * @file UnitsWidget.h
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

#ifndef UNITSWIDGET_H
#define	UNITSWIDGET_H



#include <Wt/WStandardItem>
#include <Wt/WStandardItemModel>
#include <tools/Enums.h>

#include "Conf.h"
#include "GlobalIncludeFile.h"
#include "AbstractPage.h"


class AbstractPage;

class UnitsWidget :
public AbstractPage 
{
public:
    UnitsWidget(Echoes::Dbo::Session *session, std::string apiUrl);
    virtual ~UnitsWidget();
protected:
    virtual Wt::WComboBox       *popupAdd(Wt::WDialog *dialog);
private:
    Echoes::Dbo::Session        *session_;
    std::string                 apiUrl_;
    
    Wt::WComboBox               *m_unitTypeComboBox;
    Wt::WStandardItemModel      *m_unitTypeModel;
    
    virtual void setAddResourceMessage(Wt::Http::Message *message, std::vector<Wt::WInteractWidget*>* argument);
    virtual void setModifResourceMessage(Wt::Http::Message *message, std::vector<Wt::WInteractWidget*>* argument);
    void fillModel();
};

#endif	/* UNITSWIDGET_H */
