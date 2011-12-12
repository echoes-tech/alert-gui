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
    WWidget *linkSourceBrowser(const std::string& example);

    WWidget *homePage_;
    WWidget *initHome();
    WWidget *initTab();
    WWidget *example(const char *textKey, const std::string& sourceDir);
    WWidget *firstTab();

    WWidget *wrapView(WWidget *(EchoesHome::*createWidget)());

};

WApplication *createEchoesHomeApplication(const WEnvironment& env);

#endif // ECHOESHOME_H_
