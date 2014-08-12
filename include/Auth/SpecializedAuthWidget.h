/* 
 * File:   SpecializedAuthWidget.h
 * Author: tsa
 *
 * Created on 12 août 2014, 16:53
 */

#ifndef SPECIALIZEDAUTHWIDGET_H
#define	SPECIALIZEDAUTHWIDGET_H

#include <Wt/Auth/AuthWidget>

class SpecializedAuthWidget : public Wt::Auth::AuthWidget
{
    public:
        
        SpecializedAuthWidget (const Wt::Auth::AuthService &baseAuth, Wt::Auth::AbstractUserDatabase &users, Wt::Auth::Login& login, Wt::WContainerWidget* parent=0)
        : Wt::Auth::AuthWidget(baseAuth, users, login, parent)
        {
        }
        
        SpecializedAuthWidget (Wt::Auth::Login& login, Wt::WContainerWidget* parent=0)
        : Wt::Auth::AuthWidget(login, parent)
        {
        }

        virtual Wt::WWidget *createRegistrationView(const Wt::Auth::Identity& id);
};

#endif	/* SPECIALIZEDAUTHWIDGET_H */

