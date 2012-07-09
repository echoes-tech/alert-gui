#ifndef ECHOESHOME_H_
#define ECHOESHOME_H_

#include "Session.h"
#include "PostgresConnector.h"
#include "User.h"
#include <Wt/WApplication>

using namespace Wt;

class EchoesHome : public WApplication
{
public:
    EchoesHome(const WEnvironment& env);

protected:
    WString tr(const char *key);
    WTabWidget *tabList;
    Session *session_;
    PostgresConnector *pgc;
    void handleInternalPath(const std::string &internalPath);
private:

    WWidget *homePage_;
    WWidget *authWidget_;
    WWidget *initHome();
    WWidget *authWidget();
    WWidget *initTabs();
    WWidget *createTab(const char *textKey, const char *sourceDir);
    void doNothing();
    void onAuthEvent();

};

WApplication *createEchoesHomeApplication(const WEnvironment& env);

#endif // ECHOESHOME_H_
