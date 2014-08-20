/* 
 * Gui SpecializedAuthWidget.h
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

