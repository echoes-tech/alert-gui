/* 
 * ECHOES Home
 * @author ECHOES Technologies (TSA)
 * @date 15/11/2012
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS WITHOUT
 * COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012-2013 BY ECHOES TECHNOLGIES SAS
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
    concatApiUrl();
    initAuth();
    initHeader();
    initMainPageWidget();
    processEnvironment();
}

EchoesHome::~EchoesHome()
{
}

Echoes::Dbo::Session* EchoesHome::getSession()
{
    return this->session;
}

void EchoesHome::concatApiUrl()
{
    string apiUrl = "http";
    if (conf.isApiHttps())
    {
        apiUrl += "s";
    }
    apiUrl += "://" + conf.getApiHost() + ":" + boost::lexical_cast<string>(conf.getApiPort());

    setApiUrl(apiUrl);
}

void EchoesHome::setApiUrl(string apiUrl) {
    m_apiUrl = apiUrl;
    return;
}

string EchoesHome::getApiUrl() const {
    return m_apiUrl;
}

void EchoesHome::initSession()
{
    this->session = new Echoes::Dbo::Session(conf.getSessConnectParams());
    this->session->login().changed().connect(this, &EchoesHome::onAuthEvent);
}

void EchoesHome::initAuth()
{
    std::cout << "== initAuth ==" << std::endl;
    this->authModel = new SpecializedAuthModel(Echoes::Dbo::Session::auth(),this->session->users(), this);
    this->authModel->addPasswordAuth(&Echoes::Dbo::Session::passwordAuth());
    this->authModel->addOAuth(Echoes::Dbo::Session::oAuth());
    
    this->authWidget = new SpecializedAuthWidget(this->session->login());
    this->authWidget->setModel(this->authModel);
    this->authWidget->setRegistrationEnabled(true);
    this->addWidget(this->authWidget);
    
    Wt::WFormModel *LoginNameModel = new Wt::WFormModel();
    LoginNameModel->setObjectName("LoginNameField");
    this->authWidget->updateModelField(LoginNameModel, this->authModel->LoginNameField);
    
    Wt::WFormModel *PasswordModel = new Wt::WFormModel();
    PasswordModel->setObjectName("PasswordField");
    this->authWidget->updateModelField(PasswordModel, this->authModel->PasswordField);
        
    this->authWidget->childrenChanged().connect(bind([ = ] () {
        bool isLogin = true;
        for (auto it = this->authWidget->children().begin() ; it != this->authWidget->children().end() ; ++it)
        {
            Wt::WLineEdit *lineEdit;
            lineEdit = (Wt::WLineEdit*) dynamic_cast<Wt::WLineEdit*> (*it);
            
            if (lineEdit != NULL)
            {
                if (isLogin == true)
                {
                    lineEdit->setObjectName("login");
                    isLogin = false;
                }
                else
                {
                    lineEdit->setObjectName("password");
                }
            }
            
            Wt::WPushButton *pushButton;
            pushButton = (Wt::WPushButton*) dynamic_cast<Wt::WPushButton*> (*it);
            if (pushButton != NULL)
            {
                pushButton->setObjectName("pushButton");
            }
        }
    }));
}

void EchoesHome::initHeader()
{
//    Wt::WApplication *app = Wt::WApplication::instance();
    this->title = new Wt::WText(tr("echoes-alert-title"));
    this->title->setInline(false);
    this->title->setId("header");
    this->title->hide();

    // Todo : vérifier si les layouts sont toujours pertinents (loggué / déloggué) depuis bootstrap
    this->topBoxLoggedInLayout = new Wt::WHBoxLayout();
    this->topBoxLoggedOutLayout = new Wt::WVBoxLayout();
    this->topRightLayout = new Wt::WVBoxLayout();

    this->addWidget(this->title);
    
}

void EchoesHome::initMainPageWidget()
{
    this->mainPageWidget = new MainWidget(this->session, m_apiUrl);
    this->mainPageWidget->hide();
    // Why : besoin d'accéder à l'objet de l'extérieur pour gérer les affichages 
    // dans le menu indépendemment du container central
    this->addWidget(this->mainPageWidget->getSideBarContainer());
    this->addWidget(this->mainPageWidget);
    this->addWidget(this->mainPageWidget->getFooterContainer());
    this->mainPageWidget->getSideBarContainer()->hide();
    
    // Todo : à déplacer
    Wt::WApplication::instance()->internalPathChanged().connect(this, &EchoesHome::handleInternalPath);
}

void EchoesHome::processEnvironment()
{
    this->authWidget->processEnvironment();
}

void EchoesHome::handleInternalPath(const string &internalPath)
{
    if (this->session->login().loggedIn()) 
    {
//        initMainPageWidget();
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
            for (vector<Enums::EPageType>::const_iterator i = mainPageWidget->getPageDisplayVector()->begin(); i != mainPageWidget->getPageDisplayVector()->end(); ++i)
            {
                map<unsigned int, unsigned int> *mapFromPageTypeToMenuIndex = mainPageWidget->getMenuIndexFromPageType();
                map<unsigned int, string> *mapFromMenuIndexToValue = mainPageWidget->getValueFromMenuIndex();
                unsigned int menuIndex = (*mapFromPageTypeToMenuIndex)[i->index()];
                string value = (*mapFromMenuIndexToValue)[(*mapFromPageTypeToMenuIndex)[i->index()]];
                if (internalPathWithoutBlank[0].compare(value) == 0)
                {
                
                    if ((unsigned)this->mainPageWidget->getMenu()->currentIndex() != menuIndex)
                    {
                        
                        if(menuIndex <= (unsigned)this->mainPageWidget->getMenu()->count())
                        {
                            this->mainPageWidget->getMenu()->itemAt(menuIndex)->setFromInternalPath(internalPath);
                        }
                        else
                        {
                            Wt::WApplication::instance()->setInternalPath("/messages",  false);
                            this->mainPageWidget->getMenu()->itemAt(0)->setFromInternalPath(internalPath);
                        }

                    }
                    UserActionManagement::registerUserAction(Enums::EAction::display,internalPathWithoutBlank[0],0);
                    showPage(i->index());
                    displayed = true;
                    break;
                }
            }
            if ((!displayed) && (boost::starts_with(internalPathWithoutBlank[0], "submenu_")))
            {
                displayed = true;
            }
            break;
        }
        default:
        {
            Wt::log("error") << "To many elements in the internal path";
            break;
        }
        }
        
        
        
        if (!displayed)
        {
            UserActionManagement::registerUserAction(Enums::EAction::display,"/messages (default)",0);
            Wt::WApplication::instance()->setInternalPath("/messages",  false);
            if (this->mainPageWidget->getMenu()->currentIndex() != Enums::EPageType::MESSAGES)
            {
                this->mainPageWidget->getMenu()->itemAt(Enums::EPageType::MESSAGES)->setFromInternalPath(internalPath);
            }
            showPage(Enums::EPageType::MESSAGES);
        }

    }
    else
    {
//        Wt::WApplication::instance()->setInternalPath("/",  false);
    }
}


void EchoesHome::showPage(int pageType)
{
    if (this->mainPageWidget->isHidden())
    {
        this->mainPageWidget->show();
    }
    
    this->mainPageWidget->doActionMenu(pageType);
}

void EchoesHome::onAuthEvent()
{
    if (this->session->login().loggedIn())
    {
        UserActionManagement::registerUserAction(Enums::EAction::login,"success",1);
        this->title->show();
        this->mainPageWidget->createUI();
        this->mainPageWidget->show();
        this->mainPageWidget->getSideBarContainer()->show();
        handleInternalPath(Wt::WApplication::instance()->internalPath());
    }
    else
    {
        UserActionManagement::registerUserAction(Enums::EAction::logout,"",0);
        this->mainPageWidget->reset();
        this->mainPageWidget->hide();
        this->mainPageWidget->getSideBarContainer()->hide();
        this->title->hide();
        Wt::WApplication::instance()->setInternalPath("/",  false);
    }
}

Wt::WWidget * EchoesHome::displayPasswordChangeWidget()
{
    if (this->session->login().loggedIn())
    {
        return this->authWidget->createUpdatePasswordView(this->session->login().user(),true);
    }
    else
    {
        return new Wt::WText();
    }
}

void EchoesHome::refresh()
{
    this->mainPageWidget->refresh();
}

void EchoesHome::deleteContent()
{
    this->mainPageWidget->clear();
    this->mainPageWidget->reset();
}

