#ifndef ECHOESHOME_H_
#define ECHOESHOME_H_

#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WServer>

#include <Wt/Auth/AuthModel>
#include "AuthWidget.h"
#include <Wt/Auth/PasswordService>

#include <tools/Session.h>

class EchoesHome : public Wt::WApplication 
{
public:

    EchoesHome(const Wt::WEnvironment& env)
    : Wt::WApplication(env),
    session_("hostaddr=127.0.0.1 port=5432 dbname=echoes user=echoes password=toto") 
    {
        session_.login().changed().connect(this, &EchoesHome::authEvent);
        
        try 
        {
            session_.createTables();
            std::cerr << "Created database." << std::endl;
        } catch (std::exception& e) {
            std::cerr << e.what() << std::endl;
            std::cerr << "Using existing database";
        }

        useStyleSheet("css/style.css");

        Wt::Auth::AuthModel *authModel = new Wt::Auth::AuthModel(Session::auth(),
                                session_.users(), this);
        authModel->addPasswordAuth(&Session::passwordAuth());
        authModel->addOAuth(Session::oAuth());

        Wt::Auth::AuthWidget *authWidget = new Wt::Auth::AuthWidget(session_.login());
        authWidget->setModel(authModel);
        authWidget->setRegistrationEnabled(true);

        root()->addWidget(authWidget);
        
        authWidget->processEnvironment();
    }

    void authEvent() {
        if (session_.login().loggedIn()) 
        {
            Wt::log("notice") << "User " << session_.login().user().id()
                    << " logged in.";
//            setCookie("test", "value",600);
        }
        else 
        {
            Wt::log("notice") << "User logged out.";
        }
    }
    Session session_;
private:

};


Wt::WApplication *createEchoesHomeApplication(const Wt::WEnvironment& env) 
{
    // On instancie la classe EchoesHome qui permet d'afficher le site.
    return new EchoesHome(env);
}

#endif // ECHOESHOME_H_
