#ifndef ECHOESHOME_H_
#define ECHOESHOME_H_

#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WServer>

#include <Wt/Auth/AuthModel>
#include <Wt/Auth/AuthWidget>
#include <Wt/Auth/PasswordService>

#include <tools/Session.h>

class EchoesHome : public Wt::WApplication
{
public:
  EchoesHome(const Wt::WEnvironment& env)
    : Wt::WApplication(env),
      session_()
  {
    session_.login().changed().connect(this, &EchoesHome::authEvent);
    
    useStyleSheet("css/style.css");

    Wt::Auth::AuthWidget *authWidget
      = new Wt::Auth::AuthWidget(Session::auth(), session_.users(),
				 session_.login());

    authWidget->model()->addPasswordAuth(&Session::passwordAuth());
    authWidget->model()->addOAuth(Session::oAuth());
    authWidget->setRegistrationEnabled(true);

    authWidget->processEnvironment();

    root()->addWidget(authWidget);
  }

  void authEvent() {
    if (session_.login().loggedIn())
      Wt::log("notice") << "User " << session_.login().user().id()
			<< " logged in.";
    else
      Wt::log("notice") << "User logged out.";
  }

private:
  Session session_;
};

//using namespace Wt;
//
//class EchoesHome : public WApplication {
//public:
//    EchoesHome(const WEnvironment& env);
//protected:
//    WString tr(const char *key);
//    WTabWidget *tabList;
//    //    PostgresConnector *pgc;
//    void handleInternalPath(const std::string &internalPath);
//
//    void authEvent();
//private:
//    Session session_;
//
//    //    WWidget *homePage_;
//    Wt::Auth::AuthWidget *authWidget;
//
//    Wt::Auth::AuthModel *authModel;
//    //    Wt::Auth::AuthWidget *authWidget;
//
//    WWidget *initHome();
//    void *initAuthWidget();
//    WWidget *initTabs();
//    WWidget *createTab(const char *textKey, const char *sourceDir);
//
//};

Wt::WApplication *createEchoesHomeApplication(const Wt::WEnvironment& env)
{
    // On instancie la classe EchoesHome qui permet d'afficher le site.
    return new EchoesHome(env);
}

#endif // ECHOESHOME_H_
