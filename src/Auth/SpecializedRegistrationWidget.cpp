/* 
 * Gui SpecializedRegistrationWidget.cpp
 * 
 * @author Echoes Technologies
 * @author Thomas SAQUET (TSA)
 * 
 * @date 13/08/2014
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS WITHOUT
 * COMPANY AUTHORIZATION.
 * AbstractPage
 * COPYRIGHT 2012-2014 BY ECHOES TECHNOLGIES SAS
 * 
 */

#include <boost/random.hpp>
#include <boost/random/random_device.hpp>

#include <tools/Enums.h>
#include <tools/Session.h>
#include <pack/OptionType.h>
#include <pack/Option.h>
#include <engine/EngGrp.h>
#include <engine/Engine.h>

#include "Auth/SpecializedRegistrationWidget.h"
#include "Enums.h"


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

    Echoes::Dbo::Group *grp = new Echoes::Dbo::Group();

    Wt::Dbo::ptr<Echoes::Dbo::GroupType> type;


    //triche
    type = ((Echoes::Dbo::UserDatabase*)user.database())->session_.find<Echoes::Dbo::GroupType>().where("\"OTY_ID\" = ?").bind(Echoes::Dbo::GroupType::Individual);
    grp->name = model()->valueText(model()->EmailField);

    grp->groupType = type;
    grp->token = generateToken();

    // creation du role
    Wt::Dbo::ptr<Echoes::Dbo::Group> pGrp = ((Echoes::Dbo::UserDatabase*)user.database())->session_.add<Echoes::Dbo::Group>(grp);

    Echoes::Dbo::UserRole *role = new Echoes::Dbo::UserRole();
    role->name = "default";
    role->group = pGrp;
    
    Wt::Dbo::ptr<Echoes::Dbo::UserRole> ptrRole = ((Echoes::Dbo::UserDatabase*)user.database())->session_.add<Echoes::Dbo::UserRole>(role);    
    
    ((Echoes::Dbo::UserDatabase*)user.database())->find(user).get()->user().modify()->userRole = ptrRole;
    
    // creation du premier media mail
    Echoes::Dbo::Media *media = new Echoes::Dbo::Media();
    Wt::Dbo::ptr<Echoes::Dbo::MediaType> mediaType = ((Echoes::Dbo::UserDatabase*)user.database())->session_.find<Echoes::Dbo::MediaType>().where(QUOTE(TRIGRAM_MEDIA_TYPE ID)" = ?").bind(Enums::EMedia::email);
    media->mediaType = mediaType;
    media->user = ((Echoes::Dbo::UserDatabase*)user.database())->find(user).get()->user();
    media->token = ((Echoes::Dbo::UserDatabase*)user.database())->find(user).get()->user()->token;
    media->value = model()->valueText(model()->LoginNameField);
    media->isConfirmed = false;
    media->isDefault = false;
    Wt::Dbo::ptr<Echoes::Dbo::Media> pMed = ((Echoes::Dbo::UserDatabase*)user.database())->session_.add<Echoes::Dbo::Media>(media);
    
    // ajout du media par defaut au grp
    pGrp.modify()->defaultMedia = pMed;
    
//    //TODO : hardcoded, should be changed when the pack selection will be available
    Wt::Dbo::ptr<Echoes::Dbo::Pack> ptrPack = ((Echoes::Dbo::UserDatabase*)user.database())->session_.find<Echoes::Dbo::Pack>().where(QUOTE(TRIGRAM_PACK ID)" = ?").bind(1);
    pGrp.modify()->pack = ptrPack;

    Wt::Dbo::ptr<Echoes::Dbo::OptionType> otyPtr = ((Echoes::Dbo::UserDatabase*)user.database())->session_.find<Echoes::Dbo::OptionType>().where(QUOTE(TRIGRAM_OPTION_TYPE ID)" = ?").bind(Echoes::Dbo::EOptionType::QUOTA_SMS);

    Echoes::Dbo::Option *option = new Echoes::Dbo::Option();
    option->optionType = otyPtr;
    option->group = pGrp;
    //FIXME : should be the default value found in the table POP
    option->value = "5";
    Wt::Dbo::ptr<Echoes::Dbo::Option> ptrOpt = ((Echoes::Dbo::UserDatabase*)user.database())->session_.add<Echoes::Dbo::Option>(option);

    
    ((Echoes::Dbo::UserDatabase*)user.database())->find(user).get()->user().modify()->group = pGrp;

    Echoes::Dbo::EngGrp *engGrp = new Echoes::Dbo::EngGrp();
    engGrp->pk.group = pGrp;
    engGrp->token = generateToken();
    
    //TODO : hardcoded, should be changed for multiple Engines
    Wt::Dbo::ptr<Echoes::Dbo::Engine> enginePtr = ((Echoes::Dbo::UserDatabase*)user.database())->session_.find<Echoes::Dbo::Engine>().where("\"ENG_ID\" = ?").bind(1);

    engGrp->pk.engine = enginePtr;
    
    Wt::Dbo::ptr<Echoes::Dbo::EngGrp> enoPtr = ((Echoes::Dbo::UserDatabase*)user.database())->session_.add<Echoes::Dbo::EngGrp>(engGrp);
    
}
