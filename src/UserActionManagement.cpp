/* 
 * File:   UserAction.cpp
 * Author: tsa
 * 
 * Created on 26 octobre 2012, 15:27
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
        
        Wt::Dbo::ptr<Echoes::Dbo::UserAction> ptrUserAction = session->find<Echoes::Dbo::UserAction>().where("\"UAC_ID\" = ?").bind(enumAction);
        
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
