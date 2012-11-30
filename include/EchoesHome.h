#ifndef ECHOESHOME_H_
#define ECHOESHOME_H_

#include "GlobalIncludeFile.h"

#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WServer>

#include <Wt/Auth/AuthModel>
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
#include <Wt/WPanel>

#include <Wt/Dbo/Query>
#include <Wt/Dbo/QueryModel>
#include <Wt/WTableView>
#include <Wt/WTable>

class EchoesHome : public Wt::WContainerWidget 
{
public:

    EchoesHome(Wt::WContainerWidget *parent = 0);
    Session *getSession();
private:
    Session *session;
    void openUserEdition();
    void tabSelected();
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
    Wt::WContainerWidget *monitoringPage;
    Wt::WContainerWidget *links;
    Wt::WHBoxLayout *linksLayout;
    Wt::WAnchor *adminAnchor;
    Wt::WAnchor *monitoringAnchor;
    
    AlertListWidget *alertGroupBox;
    
    UserEditionModel *uem;
    UserEditionWidget *uew;

    AlertEditionModel *aem;
    AlertEditionWidget *aew;
    
    AssetManagementModel *amm;
    AssetManagementWidget *amw;
    
    OptionManagementModel *omm;
    OptionManagementWidget *omw;
    
    
    
    void initAuth();
    void initHeader();
    void initMainStack();
    void setLinks();
    void onAuthEvent();
    void showAdmin();
    void showMonitoring();
    void resizeContainers(bool loggedIn);
    
    Wt::WContainerWidget *initMonitoringWidget();
    Wt::WTabWidget* initAdminWidget();
    
    virtual void refresh();
    

};

#endif // ECHOESHOME_H_
