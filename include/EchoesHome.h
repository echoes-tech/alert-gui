/* 
 * Header of ECHOES Home
 * @author ECHOES Technologies (TSA)
 * @date 15/11/2012
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS WITHOUT
 * COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012 BY ECHOES TECHNOLGIES SAS
 * 
 */

#ifndef ECHOESHOME_H
#define ECHOESHOME_H

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
        Echoes::Dbo::Session *getSession();
        std::string getApiUrl() const;

        // This globale is version of web site. Init in main.c.
        static std::string     version_g;
        
    private:
        Echoes::Dbo::Session *session;
        std::string _apiUrl;

        void concatApiUrl();
        void setApiUrl(std::string apiUrl);

        void handleInternalPath(const std::string &internalPath);

        Wt::Auth::AuthWidget *authWidget;
        Wt::Auth::AuthModel *authModel;
        Wt::WText *title;
    //    Wt::WContainerWidget *topContainer;
        Wt::WHBoxLayout *topBoxLoggedInLayout;
        Wt::WVBoxLayout *topBoxLoggedOutLayout;
        Wt::WVBoxLayout *topRightLayout;
        Wt::WStackedWidget *mainStack;
    //    Wt::WLabel *admin_;
        Wt::WTabWidget *adminPageTabs;
        Wt::WContainerWidget *monitoringPage;
        MainWidget *mainPageWidget;
        Wt::WContainerWidget *links;
        Wt::WHBoxLayout *linksLayout;
        Wt::WAnchor *adminAnchor;
        Wt::WAnchor *monitoringAnchor;

        Wt::WAnchor *testAnchor;

        void initSession();
        void initAuth();
        void initHeader();
        void processEnvironment();

        void onAuthEvent();
        void showPage(int page, Enums::EMenuRoot menuRoot = Enums::main);

        Wt::WTabWidget* initAdminWidget();
        void initMainPageWidget();

        virtual void refresh();
        virtual void deleteContent();
};

#endif // ECHOESHOME_H
