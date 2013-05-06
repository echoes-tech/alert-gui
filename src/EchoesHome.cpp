/* 
 * ECHOES Home
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

#include "EchoesHome.h"

using namespace std;

/**
Class EchoesHome : étend la classe WContainerWidget.
 */
EchoesHome::EchoesHome(Wt::WContainerWidget *parent) : Wt::WContainerWidget(parent), adminPageTabs(0), monitoringPage(0)
{
    initSession();

    string apiHost = "", apiPort = "";
    Wt::WApplication::readConfigurationProperty("api-host", apiHost);
    Wt::WApplication::readConfigurationProperty("api-port", apiPort);
#ifdef NDEBUG
    setApiUrl("https://" + apiHost + ":" + apiPort);
#else
    setApiUrl("http://" + apiHost + ":" + apiPort);
#endif
    
    initAuth();
    initHeader();
    initMainPageWidget();
    processEnvironment();
}

Session* EchoesHome::getSession()
{
    return this->session;
}

void EchoesHome::setApiUrl(string apiUrl) {
    _apiUrl = apiUrl;
    return;
}

string EchoesHome::getApiUrl() const {
    return _apiUrl;
}

void EchoesHome::initSession()
{
    string dbHost = "127.0.0.1";
    string dbPort = "5432";
    string dbName = "echoes";
    string dbUser = "echoes";
    string dbPassword = "toto";
    Wt::WApplication::readConfigurationProperty("db-host", dbHost);
    Wt::WApplication::readConfigurationProperty("db-port", dbPort);
    Wt::WApplication::readConfigurationProperty("db-name", dbName);
    Wt::WApplication::readConfigurationProperty("db-user", dbUser);
    Wt::WApplication::readConfigurationProperty("db-password", dbPassword);
    this->session = new Session("hostaddr=" + dbHost + " port=" + dbPort + " dbname=" + dbName + " user=" + dbUser + " password=" + dbPassword);
    this->session->login().changed().connect(this, &EchoesHome::onAuthEvent);
    
}

void EchoesHome::initAuth()
{
    this->authModel = new Wt::Auth::AuthModel(Session::auth(),this->session->users(), this);
    this->authModel->addPasswordAuth(&Session::passwordAuth());
    this->authModel->addOAuth(Session::oAuth());

    this->authWidget = new Wt::Auth::AuthWidget(this->session->login());
    this->authWidget->setModel(this->authModel);
    this->authWidget->setRegistrationEnabled(true);
    this->addWidget(this->authWidget);
}

void EchoesHome::initHeader()
{
//    Wt::WApplication *app = Wt::WApplication::instance();
    this->title = new Wt::WText(tr("echoes-alert-title"));
    this->title->setId("header");
    this->title->hide();


    this->topBoxLoggedInLayout = new Wt::WHBoxLayout();
    this->topBoxLoggedOutLayout = new Wt::WVBoxLayout();
    this->topRightLayout = new Wt::WVBoxLayout();

    this->addWidget(this->title);
    
}

Wt::WContainerWidget* EchoesHome::initMonitoringWidget()
{
    MonitoringWidget *res = new MonitoringWidget(this->session);
    this->mainStack->addWidget(res);
    res->hide();
    return res;
}

void EchoesHome::initMainPageWidget()
{
    this->mainPageWidget = new MainWidget(this->session, _apiUrl);
    this->mainPageWidget->hide();
    this->addWidget(this->mainPageWidget->getSideBarContainer());
    this->addWidget(this->mainPageWidget);
    Wt::WApplication::instance()->internalPathChanged().connect(this, &EchoesHome::handleInternalPath);
}

void EchoesHome::processEnvironment()
{
    this->authWidget->processEnvironment();
}

Wt::WTabWidget* EchoesHome::initAdminWidget()
{
    // main widget
    Wt::WTabWidget *res = new Wt::WTabWidget(this);
    this->mainStack->addWidget(res);
    res->hide();
    
    
    //user list widget
    Wt::WGroupBox * usersGroupBox = new Wt::WGroupBox("Users list");
    
    Wt::Dbo::QueryModel<Wt::Dbo::ptr<User> > *qm = new Wt::Dbo::QueryModel<Wt::Dbo::ptr<User> >();
    
    Wt::WTableView *tview = new Wt::WTableView(usersGroupBox);
    try
    {
        Wt::Dbo::Transaction transaction(*(this->session));
        Wt::Dbo::Query<Wt::Dbo::ptr<User>,Wt::Dbo::DynamicBinding> q = this->session->find<User,Wt::Dbo::DynamicBinding>().where("\"USR_ID\" = ?").bind(this->session->user().id());
        qm->setQuery(q, false);
        qm->addColumn("USR_LAST_NAME", "Last name", Wt::ItemIsEditable);
//        qm->setColumnFlags(0,Wt::ItemIsUserCheckable);
        qm->addColumn("USR_FIRST_NAME", "First name", Wt::ItemIsEditable);
        qm->addColumn("USR_MAIL", "E-mail", Wt::ItemIsSelectable);

        tview->setSelectionMode(Wt::SingleSelection);
//        tview->doubleClicked().connect(boost::bind(&EchoesHome::openUserEdition, this));
        tview->setModel(qm);   
    }
    catch (Wt::Dbo::Exception e)
    {
        Wt::log("error") << e.what();
    }


    return res;
}

void EchoesHome::handleInternalPath(const string &internalPath)
{
    if (this->session->login().loggedIn()) 
    {
        vector<string> internalPathSplitResult;
        vector<string> internalPathWithoutBlank;
        boost::split(internalPathSplitResult, internalPath, boost::is_any_of("/"), boost::token_compress_on);
        for (vector<string>::const_iterator i = internalPathSplitResult.begin() ; i != internalPathSplitResult.end() ; i++)
        {
            if (i->compare("") != 0)
            {
                internalPathWithoutBlank.push_back(*i);
            }
        }
        
        bool displayed = false;
        switch (internalPathWithoutBlank.size())
        {
        case 1:
        {
            for (Enums::EPageType::const_iterator i = Enums::EPageType::begin(); i != Enums::EPageType::end(); ++i)
            {
    //            string test = i->value();
                if (!boost::starts_with(i->value(), "submenu_"))
                {
                    if (internalPathWithoutBlank[0].compare(i->value()) == 0)
                    {
                        if (this->mainPageWidget->getMenu()->currentIndex() != i->index())
                        {
                            this->mainPageWidget->getMenu()->itemAt(i->index())->setFromInternalPath(internalPath);
                        }
                        UserActionManagement::registerUserAction(Enums::display,internalPathWithoutBlank[0],0);
                        showPage(i->index());
                        displayed = true;
                        break;
                    }
                }
            }
            if ((!displayed) && (boost::starts_with(internalPathWithoutBlank[0], "submenu_")))
            {
                displayed = true;
            }
            break;
        }
        case 2:
        {
            for (Enums::EAlertSubmenu::const_iterator i = Enums::EAlertSubmenu::begin(); i != Enums::EAlertSubmenu::end(); ++i)
            {
                if (internalPathWithoutBlank[1].compare(i->value()) == 0)
                {
                    if (this->mainPageWidget->getAlertSubmenu()->currentIndex() != i->index())
                    {
                        this->mainPageWidget->getAlertSubmenu()->itemAt(i->index())->setFromInternalPath(internalPath);
                    }
                    UserActionManagement::registerUserAction(Enums::display,internalPathWithoutBlank[0],0);
                    showPage(i->index(), Enums::alerts);
                    displayed = true;
                    break;
                }
            }
            if (!displayed)
            {
                for (Enums::EAccountSubmenu::const_iterator i = Enums::EAccountSubmenu::begin(); i != Enums::EAccountSubmenu::end(); ++i)
                {
                    if (internalPathWithoutBlank[1].compare(i->value()) == 0)
                    {
                        if (this->mainPageWidget->getAccountSubmenu()->currentIndex() != i->index())
                        {
                            this->mainPageWidget->getAccountSubmenu()->itemAt(i->index())->setFromInternalPath(internalPath);
                        }
                        UserActionManagement::registerUserAction(Enums::display,internalPathWithoutBlank[0],0);
                        showPage(i->index(), Enums::accounts);
                        displayed = true;
                        break;
                    }
                }
            }
            break;
        }
        default:
        {
            
        }
        }
        
        
        
        if (!displayed)
        {
            UserActionManagement::registerUserAction(Enums::display,"/welcome/ (default)",0);
            Wt::WApplication::instance()->setInternalPath("/welcome",  false);
            if (this->mainPageWidget->getMenu()->currentIndex() != Enums::EPageType::WELCOME)
            {
                this->mainPageWidget->getMenu()->itemAt(Enums::EPageType::WELCOME)->setFromInternalPath(internalPath);
            }
            showPage(Enums::EPageType::WELCOME);
        }

    }
}


void EchoesHome::showPage(int type, Enums::EMenuRoot menuRoot)
{
    if (this->mainPageWidget->isHidden())
    {
        this->mainPageWidget->show();
    }
    this->mainPageWidget->doActionMenu(type,menuRoot);
}

void EchoesHome::onAuthEvent()
{
    if (this->session->login().loggedIn())
    {
        UserActionManagement::registerUserAction(Enums::login,"success",1);
        this->title->show();
        this->mainPageWidget->createUI();
        this->mainPageWidget->show();
        this->mainPageWidget->getSideBarContainer()->show();
        handleInternalPath(Wt::WApplication::instance()->internalPath());
    }
    else
    {
        UserActionManagement::registerUserAction(Enums::logout,"",0);
        this->mainPageWidget->hide();
        this->mainPageWidget->getSideBarContainer()->hide();
        this->title->hide();
    }
}

void EchoesHome::refresh()
{
//    this->alertGroupBox->refresh();
    this->mainPageWidget->refresh();
    if (this->mainPageWidget->aew->isCreated())
    {
        this->mainPageWidget->aew->updateServerSelectionBox(this->session->user().id());
    }else
    {
        this->mainPageWidget->aew->refresh();
    }
}