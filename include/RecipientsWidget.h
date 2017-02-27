/**
 * @file RecipientsWidget.h
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

#ifndef RECIPIENTSWIDGET_H
#define	RECIPIENTSWIDGET_H

#include <Wt/WTabWidget>
#include <Wt/WMenuItem>
#include <Wt/WInteractWidget>

#include "GlobalIncludeFile.h"

#include "AbstractPage.h"
#include "UserEditionWidget.h"

class UserEditionWidget;

class RecipientsWidget :
public Wt::WContainerWidget 
{
public:
    RecipientsWidget(Echoes::Dbo::Session *session, std::string apiUrl);

    void    update();
private:
    Echoes::Dbo::Session               *session_;
    std::string           apiUrl_;
    bool                  newClass_;
};

#endif	/* RECIPIENTSWIDGET_H */
