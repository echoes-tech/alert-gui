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



#include <tools/Session.h>

#include <Wt/Dbo/Query>
#include <Wt/Dbo/QueryModel>
#include <Wt/WTableView>

class AdminWidget : public Wt::WLabel
{
    public:
        AdminWidget();
};

class MonitoringWidget : public Wt::WLabel
{
    public:
        MonitoringWidget();
};

class EchoesHome : public Wt::WContainerWidget 
{
public:

    EchoesHome(Wt::WContainerWidget *parent = 0);
    void handleInternalPath(const std::string &internalPath);
    Session session_;
private:
    Wt::WStackedWidget *mainStack_;
//    Wt::WLabel *admin_;
    Wt::WTabWidget *admin_;
    Wt::WLabel *monitoring_;
    Wt::WContainerWidget *links_;
    Wt::WAnchor *adminAnchor_;
    Wt::WAnchor *monitoringAnchor_;
    
    
    
    void onAuthEvent();
    void showAdmin();
    void showMonitoring();
    
    Wt::WTabWidget* initAdminWidget();

};

#endif // ECHOESHOME_H_
