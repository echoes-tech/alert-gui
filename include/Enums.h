/* 
 * File:   Enums.h
 * Author: tsa
 *
 * Created on 14 septembre 2012, 15:19
 */

#ifndef ENUMS_H
#define	ENUMS_H

#include "enum.hpp"
#include <string.h>

class Enums {
public:
    Enums();
    Enums(const Enums& orig);
    virtual ~Enums();
    enum EMedia
    {
        email = 1, 
        sms = 2,
        mobileapp = 3
    };
    enum EAction
    {
        login = 1, 
        registration = 2, 
        logout = 3,
        display = 4,
        download = 5,
        add = 6,
        del = 7
    };
    enum EInformationUnitType
    {
        text = 1, 
        number = 2
    };
    enum EOption
    {
        quotaAsset = 1,
        quotaSms = 2
    };
    
    enum EMenuRoot
    {
        main,
        alerts,
        accounts
    };
    
    BOOST_ENUM_VALUES(EPageType, const char*,
    (WELCOME)("welcome")
    (ASSET)("assets")
    (SUBMENU_ALERT)("submenu_alerts")
    (RECIPIENTS)("recipients")
    (ALERTS)("alerts-test")
    (SUBMENU_ACCOUNT)("submenu_accounts")
    (PLUGIN)("plugins")
    )
    
    BOOST_ENUM_VALUES(EAlertSubmenu, const char*,
    (ALERT)("alerts")
    )

//    (USER)("users")
    
    BOOST_ENUM_VALUES(EAccountSubmenu, const char*,
    (OPTION)("options")
    (ROLE)("role")
    )
private:

};

#endif	/* ENUMS_H */

