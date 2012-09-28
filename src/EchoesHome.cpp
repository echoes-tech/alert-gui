#include "EchoesHome.h"



/**
Class EchoesHome : étend la classe WContainerWidget.
*/
EchoesHome::EchoesHome(Wt::WContainerWidget *parent): 
    Wt::WContainerWidget(parent),
    adminPageTabs(0),
    monitoringPage(0)
    {
//    std::string Utils::connection = "hostaddr=172.16.3.101 port=5432 dbname=echoes user=echoes password=toto";
            session = new Session("hostaddr=172.16.3.101 port=5432 dbname=echoes user=echoes password=toto");
//        session = new Session("hostaddr=127.0.0.1 port=5432 dbname=echoes user=echoes password=toto");
//        session = new Session("hostaddr=92.243.5.246 port=5432 dbname=echoes user=echoes password=9Mw5#vvdC56Gzs#goD3M");
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
    this->mainStack->clear();
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

Wt::WGroupBox* EchoesHome::initMonitoringWidget()
{
    
    Wt::WGroupBox * alertGroupBox = new Wt::WGroupBox("Alert sent list");
    this->mainStack->addWidget(alertGroupBox);
    
    Wt::Dbo::QueryModel<boost::tuple<Wt::Dbo::ptr<Alert>,Wt::Dbo::ptr<MediaValue>,Wt::Dbo::ptr<AlertTracking> > > *qm = new Wt::Dbo::QueryModel<boost::tuple<Wt::Dbo::ptr<Alert>,Wt::Dbo::ptr<MediaValue>,Wt::Dbo::ptr<AlertTracking> > >();
    
    
    Wt::WTableView *tview = new Wt::WTableView(alertGroupBox);
    try
    {
        Wt::Dbo::Transaction transaction(*(this->session));
        //TODO : don't understand why the two lines below are needed, clean this
        Wt::Dbo::ptr<User> tempUser = this->session->find<User>().where("\"USR_ID\" = ?").bind(this->session->user().id());
        Wt::Dbo::ptr<Organization> tempOrga = tempUser->currentOrganization;
        std::string queryString = "SELECT ale, mev, atr FROM \"T_ALERT_TRACKING_ATR\" atr, \"T_ALERT_ALE\" ale , \"T_MEDIA_VALUE_MEV\" mev "
            " WHERE atr.\"ATR_ALE_ALE_ID\" = ale.\"ALE_ID\" "
            " AND atr.\"ATR_MEV_MEV_ID\" = mev.\"MEV_ID\" "
            " AND mev.\"MEV_USR_USR_ID\" IN"
            "("
                "SELECT \"T_USER_USR_USR_ID\" FROM \"TJ_USR_ORG\" WHERE \"T_ORGANIZATION_ORG_ORG_ID\" = " + boost::lexical_cast<std::string>(this->session->user().get()->currentOrganization.id()) + ""
            ")";
        Wt::Dbo::Query<boost::tuple<Wt::Dbo::ptr<Alert>,Wt::Dbo::ptr<MediaValue>,Wt::Dbo::ptr<AlertTracking> >,Wt::Dbo::DynamicBinding> q = this->session->query<boost::tuple<Wt::Dbo::ptr<Alert>,Wt::Dbo::ptr<MediaValue>,Wt::Dbo::ptr<AlertTracking> >,Wt::Dbo::DynamicBinding>(queryString);
        qm->setQuery(q, false);
        qm->addColumn("ATR_SEND_DATE", "Date", Wt::ItemIsSelectable);
        qm->addColumn("ALE_NAME", "Name", Wt::ItemIsSelectable);
        qm->addColumn("MEV_VALUE", "Value", Wt::ItemIsSelectable);
        
        tview->setModel(qm);
    }  
    catch (Wt::Dbo::Exception e)
    {
        Wt::log("error") << e.what();
    }
    
    
//    res->setCentralWidget(groupBox);
    return alertGroupBox;
}

Wt::WTabWidget* EchoesHome::initAdminWidget()
{
    // main widget
    Wt::WTabWidget *res = new Wt::WTabWidget(this);
    this->mainStack->addWidget(res);
    
    
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
    
    // alert list widget
    Wt::WGroupBox *alertGroupBox = new Wt::WGroupBox("Alert list");
    
    Wt::Dbo::QueryModel<boost::tuple<Wt::Dbo::ptr<Alert>, Wt::Dbo::ptr<AlertCriteria>, Wt::Dbo::ptr<AlertValue> > > *qmAlertList = new Wt::Dbo::QueryModel<boost::tuple<Wt::Dbo::ptr<Alert>, Wt::Dbo::ptr<AlertCriteria>, Wt::Dbo::ptr<AlertValue> > >();
    
    Wt::WTableView *tviewAlertList = new Wt::WTableView(alertGroupBox);
    
    
    try        
    {
        Wt::Dbo::Transaction transaction(*(this->session));
        
        //TODO : don't understand why the two lines below are needed, clean this
        Wt::Dbo::ptr<User> tempUser = this->session->find<User>().where("\"USR_ID\" = ?").bind(this->session->user().id());
        Wt::Dbo::ptr<Organization> tempOrga = tempUser->currentOrganization;
        std::string queryString = "SELECT ale, acr, ava FROM \"T_ALERT_ALE\" ale, \"T_ALERT_VALUE_AVA\" ava, \"T_ALERT_CRITERIA_ACR\" acr WHERE \"ALE_ID\" IN "
        "("
            "SELECT \"AMS_ALE_ALE_ID\" FROM \"T_ALERT_MEDIA_SPECIALIZATION_AMS\" WHERE \"AMS_MEV_MEV_ID\" IN "
            "("
                "SELECT \"MEV_ID\" FROM \"T_MEDIA_VALUE_MEV\" WHERE \"MEV_USR_USR_ID\" IN "
                "("
                    "SELECT \"T_USER_USR_USR_ID\" FROM \"TJ_USR_ORG\" WHERE \"T_ORGANIZATION_ORG_ORG_ID\" = " + boost::lexical_cast<std::string>(this->session->user().get()->currentOrganization.id()) + ""
                ")"
           " )"
        ") "
        "AND ale.\"ALE_AVA_AVA_ID\" = ava.\"AVA_ID\" "
        "AND ava.\"AVA_ACR_ACR_ID\" = acr.\"ACR_ID\" ";
        Wt::Dbo::Query
                <
                    boost::tuple
                    <
                        Wt::Dbo::ptr<Alert>,
                        Wt::Dbo::ptr<AlertCriteria>,
                        Wt::Dbo::ptr<AlertValue> 
                    > 
                    ,Wt::Dbo::DynamicBinding
                > q = this->session->query
                <
                    boost::tuple
                    <
                        Wt::Dbo::ptr<Alert>, 
                        Wt::Dbo::ptr<AlertCriteria>, 
                        Wt::Dbo::ptr<AlertValue> 
                    >,Wt::Dbo::DynamicBinding
                >(queryString);
        qmAlertList->setQuery(q, false);
        qmAlertList->addColumn("ALE_NAME", "Alert name", Wt::ItemIsSelectable);
//        qm->setColumnFlags(0,Wt::ItemIsUserCheckable);
        qmAlertList->addColumn("ACR_NAME", "Criteria", Wt::ItemIsSelectable);
        qmAlertList->addColumn("AVA_VALUE", "Alert Value", Wt::ItemIsSelectable);

        tviewAlertList->setSelectionMode(Wt::SingleSelection);
        tviewAlertList->setModel(qmAlertList);  
    }
    catch (Wt::Dbo::Exception e)
    {
        Wt::log("error") << e.what();
    }
    
    
    
    // user edition widget
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
    

    {
        Wt::Dbo::Transaction transaction(*(this->session));
        amm = new AssetManagementModel(const_cast<User *>(this->session->user().get()));
    }
    amw = new AssetManagementWidget(amm,session);
    
    res->addTab(new Wt::WText(tr("welcome-text")), "Bienvenue");
    res->addTab(amw, "Sondes");
    res->addTab(aew, "Alertes");
    res->addTab(alertGroupBox, "Liste des alertes");
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
        
        this->links->show();
        handleInternalPath(Wt::WApplication::instance()->internalPath());
    }
    else
    {
        this->mainStack->clear();
        this->adminPageTabs = 0;
        this->monitoringPage = 0;
        this->links->hide();
    }
}