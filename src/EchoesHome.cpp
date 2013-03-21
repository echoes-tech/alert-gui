#include "EchoesHome.h"

/**
Class EchoesHome : étend la classe WContainerWidget.
*/
EchoesHome::EchoesHome(Wt::WContainerWidget *parent): 
    Wt::WContainerWidget(parent),
    adminPageTabs(0),
    monitoringPage(0)
    {
        initSession();
        initAuth();
        initHeader();
        initMainPageWidget();
        processEnvironment();
    }

Session* EchoesHome::getSession()
{
    return this->session;
}


void EchoesHome::initSession()
{
    std::string dbHost = "127.0.0.1";
    std::string dbPort = "5432";
    std::string dbName = "echoes";
    std::string dbUser = "echoes";
    std::string dbPassword = "toto";
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
}

void EchoesHome::initHeader()
{
    this->title = new Wt::WText(tr("echoes-alert-title"));
        
    this->topContainer = new Wt::WContainerWidget();
    this->topContainer->setId("header");
    
    this->topContainer->setHeight(Wt::WLength(78));

    this->topBoxLoggedInLayout = new Wt::WHBoxLayout();
    this->topBoxLoggedOutLayout = new Wt::WVBoxLayout();
    this->topRightLayout = new Wt::WVBoxLayout();
    this->resizeContainers(this->session->login().loggedIn());

    this->addWidget(this->topContainer);
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
    this->mainPageWidget = new MainWidget(this->session);
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
    

    
    alertGroupBox = new AlertListWidget(this->session);
    
    // user edition widget
    uew = new UserEditionWidget();
    try
    {
        Wt::Dbo::Transaction transaction(*(this->session));
        uem = new UserEditionModel(const_cast<User *>(this->session->user().get()));
        uem->setSession(session);
        transaction.commit();
    }
    catch (Wt::Dbo::Exception e)
    {
        Wt::log("error") << e.what();
    }
    uew->setModel(uem);
    uew->setSession(session);
        
    aew = new AlertEditionWidget();
    try
    {
        Wt::Dbo::Transaction transaction(*(this->session));
        aem = new AlertEditionModel(const_cast<User *>(this->session->user().get()));
        aem->setSession(session);
        transaction.commit();
    }
    catch (Wt::Dbo::Exception e)
    {
        Wt::log("error") << e.what();
    }
    aew->setModel(aem);
    aew->setSession(session);
    
    try
    {
        Wt::Dbo::Transaction transaction(*(this->session));
        amm = new AssetManagementModel();
        transaction.commit();
    }
    catch (Wt::Dbo::Exception e)
    {
        Wt::log("error") << e.what();
    }
    amw = new AssetManagementWidget(amm,this->session);
    
    try
    {
        Wt::Dbo::Transaction transaction(*(this->session));
        omm = new OptionManagementModel();
        transaction.commit();
    }
    catch (Wt::Dbo::Exception e)
    {
        Wt::log("error") << e.what();
    }
    omw = new OptionManagementWidget(omm,this->session);
    
    
    
    return res;
}

void EchoesHome::handleInternalPath(const std::string &internalPath)
{
    if (this->session->login().loggedIn()) 
    {
        std::string internalPathUnslashed = "";
        int nbrSlashes = std::count_if(internalPath.begin(),internalPath.end(),std::bind1st(std::equal_to<char>(),'/'));
        internalPathUnslashed.resize(internalPath.size()-nbrSlashes);
        std::remove_copy(internalPath.begin(),internalPath.end(),internalPathUnslashed.begin(),'/');
        bool displayed = false;
        for (Enums::EPageType::const_iterator i = Enums::EPageType::begin(); i != Enums::EPageType::end(); ++i)
        {
//            std::string test = i->value();
            if (internalPathUnslashed.compare(i->value()) == 0)
            {
                if (this->mainPageWidget->getMenu()->currentIndex() != i->index())
                {
                    this->mainPageWidget->getMenu()->itemAt(i->index())->setFromInternalPath(internalPath);
                }
                UserActionManagement::registerUserAction(Enums::display,internalPathUnslashed,0);
                showPage(i->index());
                displayed = true;
                break;
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


void EchoesHome::showPage(int type)
{
    if (this->mainPageWidget->isHidden())
    {
        this->mainPageWidget->show();
    }
    this->mainPageWidget->testMenu(type);
}

void EchoesHome::resizeContainers(bool loggedIn)
{
    if (loggedIn)
    {
        this->topBoxLoggedInLayout = new Wt::WHBoxLayout();
        
        this->topBoxLoggedOutLayout->removeWidget(title);
        this->topBoxLoggedOutLayout->removeWidget(authWidget);
        this->topBoxLoggedInLayout->addWidget(title,0,Wt::AlignMiddle);
        
        this->title->setHeight(Wt::WLength(81));
        this->authWidget->setHeight(Wt::WLength(20));
//        this->links->setWidth(Wt::WLength(200));
        this->authWidget->setWidth(Wt::WLength(300));
        
        
//        this->topRightLayout = new Wt::WVBoxLayout();
//        this->topRightLayout->addWidget(this->authWidget, 0, Wt::AlignRight);
        this->addWidget(this->authWidget);
        
//        this->topRightLayout->addWidget(this->links, 0, Wt::AlignRight);
        this->topBoxLoggedInLayout->addLayout(this->topRightLayout, Wt::AlignRight);
        this->title->setHeight(Wt::WLength(81));
        this->authWidget->setHeight(Wt::WLength(20));
        this->topContainer->setLayout(this->topBoxLoggedInLayout);
        this->topContainer->setHeight(Wt::WLength(94));
    }
    else
    {
        this->topBoxLoggedOutLayout = new Wt::WVBoxLayout();
        this->topBoxLoggedInLayout->removeWidget(this->title);
//        this->topBoxLoggedInLayout->removeWidget(this->authWidget);
        this->removeWidget(this->authWidget);
        this->topBoxLoggedOutLayout->addWidget(this->title,1,Wt::AlignCenter);
        this->title->setHeight(Wt::WLength(81));
        this->authWidget->setHeight(Wt::WLength(320));
        this->topBoxLoggedOutLayout->addWidget(this->authWidget,1,Wt::AlignCenter);
        this->topContainer->setLayout(this->topBoxLoggedOutLayout);
        this->topContainer->setHeight(Wt::WLength(400));
    }
}


void EchoesHome::onAuthEvent()
{
    resizeContainers(session->login().loggedIn());
    if (this->session->login().loggedIn())
    {
        UserActionManagement::registerUserAction(Enums::login,"success",1);
        this->mainPageWidget->show();
        this->mainPageWidget->getSideBarContainer()->show();
        handleInternalPath(Wt::WApplication::instance()->internalPath());
    }
    else
    {
        UserActionManagement::registerUserAction(Enums::logout,"",0);
        this->mainPageWidget->hide();
        this->mainPageWidget->getSideBarContainer()->hide();
    }
}

void EchoesHome::refresh()
{
    this->alertGroupBox->refresh();
    if (this->aew->isCreated())
    {
        this->aew->updateServerSelectionBox(this->session->user().id());
    }else
    {
        this->aew->refresh();
    }
}