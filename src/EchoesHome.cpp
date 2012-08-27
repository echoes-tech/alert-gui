#include <Wt/WFormWidget>

#include "EchoesHome.h"





/**
Class EchoesHome : étend la classe WApplication.
@param env : variables d'environnement
*/
EchoesHome::EchoesHome(Wt::WContainerWidget *parent)
    : Wt::WContainerWidget(parent),

    session_("hostaddr=127.0.0.1 port=5432 dbname=echoes user=echoes password=toto") 
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
        Wt::Auth::AuthModel *authModel = new Wt::Auth::AuthModel(Session::auth(),
                                session_.users(), this);
        authModel->addPasswordAuth(&Session::passwordAuth());
        authModel->addOAuth(Session::oAuth());

        Wt::Auth::AuthWidget *authWidget = new Wt::Auth::AuthWidget(session_.login());
        authWidget->setModel(authModel);
        authWidget->setRegistrationEnabled(true);

//        root()->addWidget(authWidget);
        
        //admin
        admin_ = new Wt::WLabel("Admin");
        monitoring_ = new Wt::WLabel("Monitoring");
        
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

        Wt::WApplication::instance()->internalPathChanged()
            .connect(this, &EchoesHome::handleInternalPath);
        
        
        authWidget->processEnvironment();
    }



/**
Initialisation de l'application.
@return WTemplate
*/
//void *EchoesHome::initAuthWidget()
//{
//    Wt::Auth::AuthService *testAuthService = new Wt::Auth::AuthService();

//    this->authModel = new Wt::Auth::AuthModel(Session::auth() ,this->session_->users(), this);
////    this->authModel = new Wt::Auth::AuthModel(Session::auth(),this->session_->users(), this);
//    this->authModel->addPasswordAuth(&Session::passwordAuth());
////    this->authModel->addOAuth(Session::oAuth());
//
//    this->authWidget = new Wt::Auth::AuthWidget(this->session_->login());
//    this->authWidget->setModel(this->authModel);
//    this->authWidget->setRegistrationEnabled(true);
    
//}


///**
//Initialisation de l'appli, test des tabs.
//@return WTabWidget
//*/
//WWidget *EchoesHome::initTabs()
//{
//    // on créée un contenair qui va être utilisé par le WTabWidget
//    WContainerWidget *result = new WContainerWidget();
//    tabList = new WTabWidget(AlignTop | AlignJustify, result);
//
//    // on créée une animation pour animer les transitions entre les tabs
//    WAnimation animation(WAnimation::SlideInFromRight, WAnimation::EaseIn);
//    tabList->contentsStack()->setTransitionAnimation(animation, true);
//
//    // on créée les différents tabs
//    tabList->addTab(createTab("home.examples.hello","hello"),tr("hello-world"))->setPathComponent("");
//    tabList->addTab(createTab("home.examples.chart","charts"),tr("charts"));
//
//    return result;
//}

///**
//Création d'un onglet.
//@param textKey la clé qui permet de retrouver le texte de l'onglet dans le template
//@param sourceDir
//@return le container qui contient le texte
//*/
//WWidget *EchoesHome::createTab(const char *textKey, const char *sourceDir)
//{
//    WContainerWidget *result = new WContainerWidget();
//
//    // texte contenu dans l'onglet
//    WText *w = new WText(tr(textKey), result);
//    w->setInternalPathEncoding(true);
//    return result;
//
//}

///**
//Méthode utilisée pour transformer une chaîne classique en objet WString. A utiliser en entrée des méthodes WT qui demandent une WString.
//@param key chaîne passée en entrée
//@return objet WString
//*/
//WString EchoesHome::tr(const char *key)
//{
//    return WString::tr(key);
//}


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
//    admin_ = new AdminWidget(&session_, mainStack_);
    admin_ = new Wt::WLabel("Admin 2");

  mainStack_->setCurrentWidget(admin_);
//  admin_->update();

  monitoringAnchor_->removeStyleClass("selected-link");
  adminAnchor_->addStyleClass("selected-link");
}

void EchoesHome::showMonitoring()
{
  if (!monitoring_) {
//    monitoring_ = new MonitoringWidget(&session_, mainStack_);
      monitoring_ = new Wt::WLabel("Admin 2");
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