#include "EchoesHome.h"



/**
Class EchoesHome : étend la classe WContainerWidget.
*/
EchoesHome::EchoesHome(Wt::WContainerWidget *parent): 
    Wt::WContainerWidget(parent),
    adminPageTabs(0),
    monitoringPage(0)
    {
//        session = new Session("hostaddr=127.0.0.1 port=5432 dbname=echoes user=echoes password=toto");
        session = new Session("hostaddr=172.16.3.101 port=5432 dbname=echoes user=echoes password=toto");
        this->session->login().changed().connect(this, &EchoesHome::onAuthEvent);
        
        try 
        {
            this->session->createTables();
            std::cerr << "Created database." << std::endl;
        } 
        catch (std::exception& e) 
        {
            std::cerr << e.what() << std::endl;
            std::cerr << "Using existing database";
        }

        initAuth();
        initHeader();
        initMainStack();
        setLinks();

        Wt::WApplication::instance()->internalPathChanged().connect(this, &EchoesHome::handleInternalPath);
        this->authWidget->processEnvironment();
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
    this->title = new Wt::WText("<h1>ECHOES Alert</h1>");
        
    this->topContainer = new Wt::WContainerWidget();
    this->topContainer->setStyleClass("top-container");

    this->topBoxLoggedInLayout = new Wt::WHBoxLayout();
    this->topBoxLoggedOutLayout = new Wt::WVBoxLayout();
    this->topRightLayout = new Wt::WVBoxLayout();
    this->resizeContainers(this->session->login().loggedIn());

    this->addWidget(this->topContainer);

    this->links = new WContainerWidget();
    this->linksLayout = new Wt::WHBoxLayout();
    this->links->setLayout(this->linksLayout);
    this->links->setStyleClass("links");
    this->links->hide();
}

void EchoesHome::initMainStack()
{
    this->mainStack = new Wt::WStackedWidget();
    this->mainStack->setStyleClass("echoesalertstack");
    this->initAdminWidget();
    this->initMonitoringWidget();
    this->addWidget(this->mainStack);
}

void EchoesHome::setLinks()
{
    this->adminAnchor = new Wt::WAnchor("/admin", "Administration", this->links);
    this->adminAnchor->setLink(Wt::WLink(Wt::WLink::InternalPath, "/admin"));
    this->linksLayout->addWidget(this->adminAnchor);

    this->monitoringAnchor = new Wt::WAnchor("/monitoring", "Monitoring", this->links);
    this->monitoringAnchor->setLink(Wt::WLink(Wt::WLink::InternalPath, "/monitoring"));
    this->linksLayout->addWidget(this->monitoringAnchor);
}

Wt::WPanel* EchoesHome::initMonitoringWidget()
{
    Wt::WPanel *res = new Wt::WPanel();
//    this->mainStack->addWidget(res);
//    Wt::WGroupBox * groupBox = new Wt::WGroupBox("Alert sent list");
//    Wt::Dbo::QueryModel<std::vector> *qm = new Wt::Dbo::QueryModel<std::vector>();
//    std::string queryString = "SELECT ale.\"ALE_NAME\" \"NAME\", mev.\"MEV_VALUE\" \"VALUE\", atr.\"ATR_SEND_DATE\" \"DATE\" FROM \"T_ALERT_TRACKING_ATR\" atr, \"T_ALERT_ALE\" ale , \"T_MEDIA_VALUE_MEV\" mev "
//            " WHERE atr.\"ATR_ALE_ALE_ID\" = ale.\"ALE_ID\" "
//            " AND atr.\"ATR_MEV_MEV_ID\" = mev.\"MEV_ID\" "
//            " AND mev.\"MEV_USR_USR_ID\" = ?";
//    
//    Wt::WTableView *tview = new Wt::WTableView(groupBox);
//    {
//        Wt::Dbo::Transaction transaction(*(this->session));
//        Wt::Dbo::Query<std::vector,Wt::Dbo::DynamicBinding> q = this->session->query<std::vector,Wt::Dbo::DynamicBinding>(queryString).bind(session->user().id());
//        qm->setQuery(q, false);
//        qm->addColumn("NAME", "Name", Wt::ItemIsSelectable);
//        qm->addColumn("VALUE", "Value", Wt::ItemIsSelectable);
//        qm->addColumn("DATE", "Date", Wt::ItemIsSelectable);
//        tview->setModel(qm)
//    }    
//    
//    
//    res->setCentralWidget(groupBox);
    return res;
}

Wt::WTabWidget* EchoesHome::initAdminWidget()
{
    Wt::WTabWidget *res = new Wt::WTabWidget(this);
    this->mainStack->addWidget(res);
    
    Wt::WGroupBox * usersGroupBox = new Wt::WGroupBox("Users list");
    
    Wt::Dbo::QueryModel<Wt::Dbo::ptr<User> > *qm = new Wt::Dbo::QueryModel<Wt::Dbo::ptr<User> >();
    
    Wt::WTableView *tview = new Wt::WTableView(usersGroupBox);
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
    
    uew = new UserEditionWidget();
    {
        Wt::Dbo::Transaction transaction(*(this->session));
        uem = new UserEditionModel(const_cast<User *>(this->session->user().get()));
    }
    uew->setModel(uem);
    uew->setSession(session);
        
    aew = new AlertEditionWidget();
    {
        Wt::Dbo::Transaction transaction(*(this->session));
        aem = new AlertEditionModel(const_cast<User *>(this->session->user().get()));
    }
    aew->setModel(aem);
    aew->setSession(session);
        
    pdw = new ProbeDownloadWidget();
    {
        Wt::Dbo::Transaction transaction(*(this->session));
        pdm = new ProbeDownloadModel(const_cast<User *>(this->session->user().get()),this->session);
    }
    pdw->setModel(pdm);
//    pdw->setSession(session);
    
    res->addTab(new Wt::WText(tr("welcome-text")), "Bienvenue");
    res->addTab(pdw, "Sondes");
    res->addTab(aew, "Alertes");
    res->addTab(usersGroupBox, "Utilisateurs");
    res->addTab(uew, "Medias");
//    res->addTab(new Wt::WText("<h2>TESTEUH 6</h2>"), "Plugin-store");
    return res;
}

void EchoesHome::openUserEdition()
{
    
}

void EchoesHome::handleInternalPath(const std::string &internalPath)
{
  if (this->session->login().loggedIn()) {
    if (internalPath == "/monitoring")
      showMonitoring();
    else if (internalPath == "/admin")
      showAdmin();
    else
      Wt::WApplication::instance()->setInternalPath("/admin",  true);
  }
}


void EchoesHome::showAdmin()
{
  if (!this->adminPageTabs)
  {
//    admin_ = new AdminWidget(&session_, mainStack_);
    this->adminPageTabs = initAdminWidget();
  }
  if (this->monitoringPage) 
  {
    this->monitoringPage->hide();
  }
  
  this->adminPageTabs->show();
  this->mainStack->setCurrentWidget(this->adminPageTabs);

  this->monitoringAnchor->removeStyleClass("selected-link");
  this->adminAnchor->addStyleClass("selected-link");
}

void EchoesHome::showMonitoring()
{
  if (!this->monitoringPage) 
  {
//    monitoring_ = new MonitoringWidget(&session_, mainStack_);
      this->monitoringPage = initMonitoringWidget();
//      this->monitoringPage->show();
//    monitoring_->update();
  }
  if (this->adminPageTabs)
  {
    this->adminPageTabs->hide();
  }
  
  this->monitoringPage->show();
  this->mainStack->setCurrentWidget(this->monitoringPage);

  this->monitoringAnchor->addStyleClass("selected-link");
  this->adminAnchor->removeStyleClass("selected-link");
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
        this->links->setWidth(Wt::WLength(200));
        this->authWidget->setWidth(Wt::WLength(200));
        
        
        this->topRightLayout = new Wt::WVBoxLayout();
        this->topRightLayout->addWidget(this->authWidget, 0, Wt::AlignRight);
        this->topRightLayout->addWidget(this->links, 0, Wt::AlignRight);
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
    this->topContainer->refresh();
    this->refresh();
}


void EchoesHome::onAuthEvent()
{
    resizeContainers(session->login().loggedIn());
    if (this->session->login().loggedIn())
    {
        
//        this->links->show();
        handleInternalPath(Wt::WApplication::instance()->internalPath());
    }
    else
    {
        this->mainStack->clear();
        this->adminPageTabs = 0;
        this->monitoringPage = 0;
//        this->links->hide();
    }
}