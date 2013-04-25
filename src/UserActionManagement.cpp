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
        Session *session = dynamic_cast<EchoesHome*>((app->root()->widget(0)))->getSession();
        Wt::Dbo::Transaction transaction(*session);
        
        Wt::Dbo::ptr<UserAction> ptrUserAction = session->find<UserAction>().where("\"UAC_ID\" = ?").bind(enumAction);
        
        UserHistoricalAction *userHistoricalAction = new UserHistoricalAction();
        userHistoricalAction->tableObject = tableObject;
        userHistoricalAction->tableObjectId = id;
        
        Wt::Dbo::ptr<UserHistoricalAction> ptrUserHistoricalAction = session->add<UserHistoricalAction>(userHistoricalAction);
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
