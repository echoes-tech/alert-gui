/* 
 * File:   UserAction.h
 * Author: tsa
 *
 * Created on 26 octobre 2012, 15:27
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

