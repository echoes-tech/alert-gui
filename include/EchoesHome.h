#ifndef ECHOESHOME_H_
#define ECHOESHOME_H_

#include <Wt/WApplication>
#include <Wt/WContainerWidget>


using namespace Wt;

class EchoesHome : public WApplication
{
public:
    EchoesHome(const WEnvironment& env);

protected:
    WString tr(const char *key);
private:
    WWidget *homePage_;
    WWidget *initHome();

};

WApplication *createEchoesHomeApplication(const WEnvironment& env);

#endif // ECHOESHOME_H_
