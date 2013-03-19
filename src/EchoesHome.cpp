#include "EchoesHome.h"

/**
Class EchoesHome : étend la classe WContainerWidget.
*/
EchoesHome::EchoesHome(Wt::WContainerWidget *parent): 
    Wt::WContainerWidget(parent),
    adminPageTabs(0),
    monitoringPage(0)
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
        session = new Session("hostaddr=" + dbHost + " port=" + dbPort + " dbname=" + dbName + " user=" + dbUser + " password=" + dbPassword);
        this->session->login().changed().connect(this, &EchoesHome::onAuthEvent);


        initAuth();
        
        this->testPage = initTestWidget();
        
        initHeader();
        initMainStack();


        Wt::WApplication::instance()->internalPathChanged().connect(this, &EchoesHome::handleInternalPath);
        this->authWidget->processEnvironment();
    }

Session* EchoesHome::getSession()
{
    return this->session;
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
    this->topContainer->setStyleClass("top-container");

    this->topBoxLoggedInLayout = new Wt::WHBoxLayout();
    this->topBoxLoggedOutLayout = new Wt::WVBoxLayout();
    this->topRightLayout = new Wt::WVBoxLayout();
    this->resizeContainers(this->session->login().loggedIn());

    this->addWidget(this->topContainer);
}

void EchoesHome::initMainStack()
{

    this->addWidget(this->testPage);
}

void EchoesHome::setLinks()
{

}

Wt::WContainerWidget* EchoesHome::initMonitoringWidget()
{
    MonitoringWidget *res = new MonitoringWidget(this->session);
    this->mainStack->addWidget(res);
    res->hide();
    return res;
}

TestWidget* EchoesHome::initTestWidget()
{
    TestWidget *res = new TestWidget(this->session);
//    this->mainStack->addWidget(res);
    res->hide();
    return res;
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
        tview->doubleClicked().connect(boost::bind(&EchoesHome::openUserEdition, this));
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
    
    
    
    res->addTab(new Wt::WText(tr("welcome-text")), tr("Alert.admin.welcome-tab"));
    res->addTab(amw, tr("Alert.admin.asset-tab"));
    res->addTab(uew, tr("Alert.admin.medias-tab"));
    res->addTab(aew, tr("Alert.admin.alert-creation-tab"));
    res->addTab(alertGroupBox, tr("Alert.admin.alert-list-tab"));
    res->addTab(usersGroupBox, tr("Alert.admin.users-tab"));
    res->addTab(omw, tr("Alert.admin.options-tab"));
    
    res->currentChanged().connect(boost::bind(&EchoesHome::tabSelected, this));
    
    return res;
}

void EchoesHome::tabSelected()
{
    UserActionManagement::registerUserAction(Enums::display,adminPageTabs->tabText(adminPageTabs->currentIndex()),adminPageTabs->currentIndex());
}

void EchoesHome::openUserEdition()
{
    
}

void EchoesHome::handleInternalPath(const std::string &internalPath)
{
  if (this->session->login().loggedIn()) 
  {
 
    if (internalPath == "/assets/")
    {
        UserActionManagement::registerUserAction(Enums::display,"/assets",0);
        if (this->testPage->getMenu()->currentIndex() != Enums::ASSET)
        {
            this->testPage->getMenu()->select(Enums::ASSET);
        }
        showTest(Enums::ASSET);
    }
    else if (internalPath == "/welcome/")
    {
        UserActionManagement::registerUserAction(Enums::display,"/welcome",0);
        if (this->testPage->getMenu()->currentIndex() != Enums::WELCOME)
        {
            this->testPage->getMenu()->select(Enums::WELCOME);
        }
        showTest(Enums::WELCOME);
    }
    else
    {
      //Todo 404
        UserActionManagement::registerUserAction(Enums::display,"/welcome/ (default)",0);
        Wt::WApplication::instance()->setInternalPath("/welcome/",  true);
        if (this->testPage->getMenu()->currentIndex() != Enums::WELCOME)
        {
            this->testPage->getMenu()->select(Enums::WELCOME);
        }
        showTest(Enums::WELCOME);
    }
  }
}


void EchoesHome::showAdmin()
{

//    this->monitoringPage->hide();
//
//
//    this->testPage->hide();
//
//
//    this->adminPageTabs->show();
//    this->mainStack->setCurrentWidget(this->adminPageTabs);
//
//    this->testAnchor->removeStyleClass("selected-link");
//    this->monitoringAnchor->removeStyleClass("selected-link");
//    this->adminAnchor->addStyleClass("selected-link");
}

void EchoesHome::showMonitoring()
{

//  if (this->adminPageTabs)
//  {
//    this->adminPageTabs->hide();
//  }
//  if (this->testPage) 
//  {
//    this->testPage->hide();
//  }
//  
//  this->monitoringPage->show();
//  this->mainStack->setCurrentWidget(this->monitoringPage);
//
//  this->testAnchor->removeStyleClass("selected-link");
//  this->monitoringAnchor->addStyleClass("selected-link");
//  this->adminAnchor->removeStyleClass("selected-link");
}

void EchoesHome::showTest(int type)
{
  
//  this->adminPageTabs->hide();
//
//
//  this->monitoringPage->hide();

  
  this->testPage->show();
  this->testPage->testMenu(type);
//  this->mainStack->setCurrentWidget(this->testPage);

//  this->testAnchor->addStyleClass("selected-link");
//  this->monitoringAnchor->removeStyleClass("selected-link");
//  this->adminAnchor->removeStyleClass("selected-link");
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
        this->authWidget->setWidth(Wt::WLength(200));
        
        
        this->topRightLayout = new Wt::WVBoxLayout();
        this->topRightLayout->addWidget(this->authWidget, 0, Wt::AlignRight);
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
        this->topBoxLoggedInLayout->removeWidget(this->authWidget);
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
        this->testPage->show();
//        this->links->show();
        handleInternalPath(Wt::WApplication::instance()->internalPath());
    }
    else
    {
        UserActionManagement::registerUserAction(Enums::logout,"",0);
        this->testPage->clear();
//        this->adminPageTabs = 0;
//        this->monitoringPage = 0;
//        this->links->hide();
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