/**
 * @file UserActionManagement.cpp
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

#include "UserActionManagement.h"

UserActionManagement::UserActionManagement() {
}

UserActionManagement::UserActionManagement(const UserActionManagement& orig) {
}

UserActionManagement::~UserActionManagement() {
}

void UserActionManagement::registerUserAction(Enums::EAction enumAction, Wt::WString tableObject, long long id)
{
    try
    {
        Wt::WApplication *app = Wt::WApplication::instance();
        Echoes::Dbo::Session *session = dynamic_cast<EchoesHome*>((app->root()->widget(0)))->getSession();
        Wt::Dbo::Transaction transaction(*session);
        
        Wt::Dbo::ptr<Echoes::Dbo::UserActionType> ptrUserAction = session->find<Echoes::Dbo::UserActionType>().where(QUOTE(TRIGRAM_USER_ACTION_TYPE ID)" = ?").bind(enumAction);
        
        Echoes::Dbo::UserHistoricalAction *userHistoricalAction = new Echoes::Dbo::UserHistoricalAction();
        userHistoricalAction->tableObject = tableObject;
        userHistoricalAction->tableObjectId = id;
        
        Wt::Dbo::ptr<Echoes::Dbo::UserHistoricalAction> ptrUserHistoricalAction = session->add<Echoes::Dbo::UserHistoricalAction>(userHistoricalAction);
        ptrUserHistoricalAction.modify()->userAction = ptrUserAction;
        ptrUserHistoricalAction.modify()->user = session->user();
        ptrUserHistoricalAction.modify()->dateTime = Wt::WDateTime::currentDateTime();
        ptrUserHistoricalAction.modify()->actionAfter = 0;
        ptrUserHistoricalAction.modify()->actionBefore = 0;
        ptrUserHistoricalAction.modify()->actionRelative = 0;
        
        transaction.commit();
    }
    catch (Wt::Dbo::Exception e)
    {
        Wt::log("error") << "[UserAction] : " << e.what();
    }
}
