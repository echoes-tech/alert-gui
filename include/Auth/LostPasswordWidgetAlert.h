/**
 * @file LostPasswordWidgetAlert.h
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

#ifndef LOSTPASSWORDWIDGETALERT_H
#define	LOSTPASSWORDWIDGETALERT_H

#include <Wt/Auth/AuthService>
#include <Wt/Auth/AbstractUserDatabase>

#include <Wt/WLineEdit>
#include <Wt/WMessageBox>
#include <Wt/WPushButton>

#include <Wt/WMessageBox>

#include <Wt/WTemplate>

class AbstractUserDatabase;
class AuthService;

class LostPasswordWidgetAlert : public Wt::WTemplate {
public:
    /*! \brief Constructor
     */
    LostPasswordWidgetAlert(Wt::Auth::AbstractUserDatabase& users,
            const Wt::Auth::AuthService& auth,
            Wt::WContainerWidget *parent = 0);

protected:
    void send();
    void cancel();

private:
    Wt::Auth::AbstractUserDatabase& users_;
    const Wt::Auth::AuthService& baseAuth_;

 #ifndef WT_TARGET_JAVA
    static void deleteBox(Wt::WMessageBox *box);
 #else
    void deleteBox(WMessageBox *box);
 #endif
};


#endif	/* LOSTPASSWORDWIDGETALERT_H */
