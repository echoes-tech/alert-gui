/**
 * @file UserActionManagement.h
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

#ifndef USERACTIONMANAGEMENT_H
#define	USERACTIONMANAGEMENT_H


#include "GlobalIncludeFile.h"
#include <Wt/WApplication>
#include <Wt/WContainerWidget>

class EchoesHome;

class UserActionManagement {
public:
    UserActionManagement();
    UserActionManagement(const UserActionManagement& orig);
    virtual ~UserActionManagement();
    static void registerUserAction(Enums::EAction enumAction, Wt::WString tableObject, long long id);
private:

};

#endif	/* USERACTIONMANAGEMENT_H */
