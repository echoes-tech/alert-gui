/**
 * @file EchoesHome.h
 * @author Thomas Saquet, Florent Poinsaut
 * @date 
 * @brief File containing example of doxygen usage for quick reference.
 *
 * Alert - GUI is a part of the Alert software
 * Copyright (C) 2013-2017
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 * 
 */

#ifndef ECHOESHOME_H
#define ECHOESHOME_H

#include "GlobalIncludeFile.h"

#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WServer>

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

#include "Conf.h"
#include "Auth/SpecializedAuthModel.h"
#include "Auth/SpecializedAuthWidget.h"

class EchoesHome : public Wt::WContainerWidget 
{
    public:
        EchoesHome(Wt::WContainerWidget *parent = 0);
        virtual ~EchoesHome();
        Echoes::Dbo::Session *getSession();
        std::string getApiUrl() const;

        // This globale is version of web site. Init in main.c.
        static std::string version_g;
        
        Wt::WWidget * displayPasswordChangeWidget();

    private:
        Echoes::Dbo::Session *session;
        std::string m_apiUrl;

        void concatApiUrl();
        void setApiUrl(std::string apiUrl);

        void handleInternalPath(const std::string &internalPath);

        SpecializedAuthWidget *authWidget;
        SpecializedAuthModel *authModel;
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
        void showPage(int page);

//        Wt::WTabWidget* initAdminWidget();
        void initMainPageWidget();

        virtual void refresh();
        virtual void deleteContent();
};

#endif // ECHOESHOME_H
