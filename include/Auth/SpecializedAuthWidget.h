/**
 * @file SpecializedAuthWidget.h
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

#ifndef SPECIALIZEDAUTHWIDGET_H
#define	SPECIALIZEDAUTHWIDGET_H

#include <Wt/Auth/AuthWidget>

class SpecializedAuthWidget : public Wt::Auth::AuthWidget
{
    public:
        
        SpecializedAuthWidget (const Wt::Auth::AuthService &baseAuth, Wt::Auth::AbstractUserDatabase &users, Wt::Auth::Login& login, Wt::WContainerWidget* parent=0)
        : Wt::Auth::AuthWidget(baseAuth, users, login, parent) {}
        
        SpecializedAuthWidget (Wt::Auth::Login& login, Wt::WContainerWidget* parent=0)
        : Wt::Auth::AuthWidget(login, parent) {}

        virtual Wt::WWidget *createRegistrationView(const Wt::Auth::Identity& id);
};

#endif	/* SPECIALIZEDAUTHWIDGET_H */
