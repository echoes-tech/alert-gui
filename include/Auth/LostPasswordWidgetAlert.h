/* 
 * File:   LostPasswordWidgetAlert.h
 * Author: tsa
 *
 * Created on 23 janvier 2013, 15:41
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