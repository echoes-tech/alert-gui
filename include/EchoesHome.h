#ifndef ECHOESHOME_H_
#define ECHOESHOME_H_

#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WServer>

#include <Wt/Auth/AuthModel>
#include "AuthWidget.h"
#include <Wt/Auth/PasswordService>
#include <Wt/WLabel>
#include <Wt/WText>
#include <Wt/WStackedWidget>
#include <Wt/WAnchor>
#include <Wt/WFormWidget>
#include <Wt/WTabWidget>
#include <Wt/WGroupBox>
#include <Wt/WHBoxLayout>
#include <Wt/WVBoxLayout>

#include "UserEditionWidget.h"
#include "AlertEditionWidget.h"



#include <tools/Session.h>

#include <Wt/Dbo/Query>
#include <Wt/Dbo/QueryModel>
#include <Wt/WTableView>


class EchoesHome : public Wt::WContainerWidget 
{
public:

    EchoesHome(Wt::WContainerWidget *parent = 0);
private:
    Session *session;
    void openUserEdition();
    void handleInternalPath(const std::string &internalPath);
    
    Wt::Auth::AuthWidget *authWidget;
    Wt::Auth::AuthModel *authModel;
    Wt::WText *title;
    Wt::WContainerWidget *topContainer;
    Wt::WHBoxLayout *topBoxLoggedInLayout;
    Wt::WVBoxLayout *topBoxLoggedOutLayout;
    Wt::WVBoxLayout *topRightLayout;
    Wt::WStackedWidget *mainStack;
//    Wt::WLabel *admin_;
    Wt::WTabWidget *adminPageTabs;
    Wt::WLabel *monitoringPage;
    Wt::WContainerWidget *links;
    Wt::WHBoxLayout *linksLayout;
    Wt::WAnchor *adminAnchor;
    Wt::WAnchor *monitoringAnchor;
    
    
    
    UserEditionModel *uem;
    UserEditionWidget *uew;

    AlertEditionModel *aem;
    AlertEditionWidget *aew;
    
    
    
    void initAuth();
    void initHeader();
    void initMainStack();
    void setLinks();
    void onAuthEvent();
    void showAdmin();
    void showMonitoring();
    void resizeContainers(bool loggedIn);
    
    Wt::WTabWidget* initAdminWidget();

};

#endif // ECHOESHOME_H_
