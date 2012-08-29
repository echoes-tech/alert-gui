#include <Wt/WFormWidget>
#include <Wt/WGroupBox>


#include "EchoesHome.h"





/**
Class EchoesHome : étend la classe WContainerWidget.
*/
EchoesHome::EchoesHome(Wt::WContainerWidget *parent): 
    Wt::WContainerWidget(parent),
    session_("hostaddr=127.0.0.1 port=5432 dbname=echoes user=echoes password=toto"),
    admin_(0),
    monitoring_(0)
     
    {
        session_.login().changed().connect(this, &EchoesHome::onAuthEvent);
        
        try 
        {
            session_.createTables();
            std::cerr << "Created database." << std::endl;
        } catch (std::exception& e) {
            std::cerr << e.what() << std::endl;
            std::cerr << "Using existing database";
        }

        
        // Auth conf
        Wt::Auth::AuthModel *authModel = new Wt::Auth::AuthModel(Session::auth(),session_.users(), this);
        authModel->addPasswordAuth(&Session::passwordAuth());
        authModel->addOAuth(Session::oAuth());

        Wt::Auth::AuthWidget *authWidget = new Wt::Auth::AuthWidget(session_.login());
        authWidget->setModel(authModel);
        authWidget->setRegistrationEnabled(true);
        
        //admin
//        admin_ = initAdminWidget();
//        monitoring_ = new Wt::WLabel("Monitoring");
        
        Wt::WText *title = new Wt::WText("<h1>ECHOES Alert</h1>");
        addWidget(title);

        addWidget(authWidget);
        
        mainStack_ = new Wt::WStackedWidget();
//        mainStack_->setStyleClass("echoesalertstack");
        addWidget(mainStack_);

        links_ = new WContainerWidget();
        links_->setStyleClass("links");
        links_->hide();
        addWidget(links_);

        adminAnchor_ = new Wt::WAnchor("/admin", "Administration", links_);
        adminAnchor_->setLink(Wt::WLink(Wt::WLink::InternalPath, "/admin"));

        monitoringAnchor_ = new Wt::WAnchor("/monitoring", "Monitoring", links_);
        monitoringAnchor_->setLink(Wt::WLink(Wt::WLink::InternalPath, "/monitoring"));

        Wt::WApplication::instance()->internalPathChanged().connect(this, &EchoesHome::handleInternalPath);
        
        
        authWidget->processEnvironment();
    }


Wt::WTabWidget* EchoesHome::initAdminWidget()
{
    Wt::WTabWidget *res = new Wt::WTabWidget(mainStack_);
    
    Wt::WGroupBox * usersGroupBox = new Wt::WGroupBox("Users list");
    
    Wt::Dbo::QueryModel<Wt::Dbo::ptr<User> > *qm = new Wt::Dbo::QueryModel<Wt::Dbo::ptr<User> >();
    
    Wt::WTableView *tview = new Wt::WTableView(usersGroupBox);
    {
        Wt::Dbo::Transaction transaction(session_);
        Wt::Dbo::Query<Wt::Dbo::ptr<User>,Wt::Dbo::DynamicBinding> q = session_.find<User,Wt::Dbo::DynamicBinding>();
        qm->setQuery(q, false);
        qm->addColumn("USR_LAST_NAME", "Last name", Wt::ItemIsSelectable);
        qm->setColumnFlags(0,Wt::ItemIsUserCheckable);
        qm->addColumn("USR_FIRST_NAME", "First name", Wt::ItemIsSelectable);
        qm->addColumn("USR_MAIL", "E-mail", Wt::ItemIsEditable);
        
        
//        tview->resize(800, 300);
//        tview->setSelectionMode(Wt::SingleSelection);
        tview->setModel(qm);
        
        
    }
    
    
    

    
    
    
    res->addTab(new Wt::WText("<h2>TESTEUH 1</h2>"), "Welcome");
    res->addTab(new Wt::WText("<h2>TESTEUH 2</h2>"), "Probes");
    res->addTab(new Wt::WText("<h2>TESTEUH 3</h2>"), "Alerts");
    res->addTab(tview, "Users");
    res->addTab(new Wt::WText("<h2>TESTEUH 5</h2>"), "General");
    res->addTab(new Wt::WText("<h2>TESTEUH 6</h2>"), "Plugin-store");
    return res;
}

void EchoesHome::handleInternalPath(const std::string &internalPath)
{
  if (session_.login().loggedIn()) {
    if (internalPath == "/monitoring")
      showMonitoring();
    else if (internalPath == "/admin")
      showAdmin();
    else
      Wt::WApplication::instance()->setInternalPath("/monitoring",  true);
  }
}


void EchoesHome::showAdmin()
{
  if (!admin_)
  {
//    admin_ = new AdminWidget(&session_, mainStack_);
    admin_ = initAdminWidget();
  }

  mainStack_->setCurrentWidget(admin_);
//  admin_->show();

  monitoringAnchor_->removeStyleClass("selected-link");
  adminAnchor_->addStyleClass("selected-link");
}

void EchoesHome::showMonitoring()
{
  if (!monitoring_) {
//    monitoring_ = new MonitoringWidget(&session_, mainStack_);
      monitoring_ = new Wt::WLabel("Monitoring 2");
      monitoring_->show();
//    monitoring_->update();
  }

  mainStack_->setCurrentWidget(monitoring_);

  monitoringAnchor_->addStyleClass("selected-link");
  adminAnchor_->removeStyleClass("selected-link");
}


void EchoesHome::onAuthEvent()
{
    if (session_.login().loggedIn())
    {
        links_->show();
        handleInternalPath(Wt::WApplication::instance()->internalPath());
    }
    else
    {
        mainStack_->clear();
        admin_ = 0;
        monitoring_ = 0;
        links_->hide();
    }
}