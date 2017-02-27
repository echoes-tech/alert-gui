/**
 * @file SpecializedRegistrationWidget.cpp
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

#include <boost/random.hpp>
#include <boost/random/random_device.hpp>

#include <tools/Enums.h>
#include <tools/Session.h>
#include <pack/OptionType.h>
#include <pack/Option.h>
#include <engine/EngOrg.h>
#include <engine/Engine.h>

#include "Auth/SpecializedRegistrationWidget.h"


std::string SpecializedRegistrationWidget::generateToken()
{
    std::string res = "";
    std::string chars(
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "1234567890"
//        "!@#$%^&*()"
//        "`~-_=+[{]{\\|;:'\",<.>/? "
    );
    boost::random::random_device rng;
    boost::random::uniform_int_distribution<> index_dist(0, chars.size() - 1);
    for(int i = 0; i < 25; ++i) 
    {
        res += chars[index_dist(rng)];
    }
    return res;
}  

void SpecializedRegistrationWidget::registerUserDetails(Wt::Auth::User& user)
{
    ((Echoes::Dbo::UserDatabase*)user.database())->find(user).get()->user().modify()->eMail = model()->valueText(model()->LoginNameField);
    ((Echoes::Dbo::UserDatabase*)user.database())->find(user).get()->user().modify()->token = generateToken();
    
    ((Echoes::Dbo::UserDatabase*)user.database())->find(user).get()->user().modify()->lastName = model()->valueText(model()->LoginNameField);

    Echoes::Dbo::Organization *org = new Echoes::Dbo::Organization();

    Wt::Dbo::ptr<Echoes::Dbo::OrganizationType> type;


    //triche
    type = ((Echoes::Dbo::UserDatabase*)user.database())->session_.find<Echoes::Dbo::OrganizationType>().where("\"OTY_ID\" = ?").bind(Echoes::Dbo::OrganizationType::Individual);
    org->name = model()->valueText(model()->EmailField);

    org->organizationType = type;
    org->token = generateToken();

    Wt::Dbo::ptr<Echoes::Dbo::Organization> ptrOrg = ((Echoes::Dbo::UserDatabase*)user.database())->session_.add<Echoes::Dbo::Organization>(org);

    Echoes::Dbo::UserRole *role = new Echoes::Dbo::UserRole();
    role->name = "default";
    role->organization = ptrOrg;
    
    Wt::Dbo::ptr<Echoes::Dbo::UserRole> ptrRole = ((Echoes::Dbo::UserDatabase*)user.database())->session_.add<Echoes::Dbo::UserRole>(role);    
    
    ((Echoes::Dbo::UserDatabase*)user.database())->find(user).get()->user().modify()->userRole = ptrRole;
    
//    //TODO : hardcoded, should be changed when the pack selection will be available
    Wt::Dbo::ptr<Echoes::Dbo::Pack> ptrPack = ((Echoes::Dbo::UserDatabase*)user.database())->session_.find<Echoes::Dbo::Pack>().where(QUOTE(TRIGRAM_PACK ID)" = ?").bind(1);
    ptrOrg.modify()->pack = ptrPack;

    Wt::Dbo::ptr<Echoes::Dbo::OptionType> otyPtr = ((Echoes::Dbo::UserDatabase*)user.database())->session_.find<Echoes::Dbo::OptionType>().where(QUOTE(TRIGRAM_OPTION_TYPE ID)" = ?").bind(Echoes::Dbo::EOptionType::QUOTA_SMS);

    Echoes::Dbo::Option *option = new Echoes::Dbo::Option();
    option->optionType = otyPtr;
    option->organization = ptrOrg;
    //FIXME : should be the default value found in the table POP
    option->value = "5";
    Wt::Dbo::ptr<Echoes::Dbo::Option> ptrOpt = ((Echoes::Dbo::UserDatabase*)user.database())->session_.add<Echoes::Dbo::Option>(option);

    
    ((Echoes::Dbo::UserDatabase*)user.database())->find(user).get()->user().modify()->organization = ptrOrg;

    Echoes::Dbo::EngOrg *engOrg = new Echoes::Dbo::EngOrg();
    engOrg->pk.organization = ptrOrg;
    engOrg->token = generateToken();
    
    //TODO : hardcoded, should be changed for multiple Engines
    Wt::Dbo::ptr<Echoes::Dbo::Engine> enginePtr = ((Echoes::Dbo::UserDatabase*)user.database())->session_.find<Echoes::Dbo::Engine>().where("\"ENG_ID\" = ?").bind(1);

    engOrg->pk.engine = enginePtr;
    
    Wt::Dbo::ptr<Echoes::Dbo::EngOrg> enoPtr = ((Echoes::Dbo::UserDatabase*)user.database())->session_.add<Echoes::Dbo::EngOrg>(engOrg);
    
}
