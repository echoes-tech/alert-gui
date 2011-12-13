#ifndef ECHOESHOME_H_
#define ECHOESHOME_H_

#include <Wt/WApplication>

using namespace Wt;

class EchoesHome : public WApplication
{
public:
    EchoesHome(const WEnvironment& env);

protected:
    WString tr(const char *key);
    WTabWidget *tabList;
private:

    WWidget *homePage_;
    WWidget *initHome();
    WWidget *initTabs();
    WWidget *createTab(const char *textKey, const char *sourceDir);

};

WApplication *createEchoesHomeApplication(const WEnvironment& env);

#endif // ECHOESHOME_H_
