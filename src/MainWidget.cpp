/* 
 * Gui MonitoringWidget.cpp
 * 
 * @author ECHOES Technologies (TSA)
 * 
 * @date 14/08/2012
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS WITHOUT
 * COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012-2013 BY ECHOES TECHNOLGIES SAS
 * 
 */

#include "MainWidget.h"
#include "SummaryBoard.h"

MainWidget::MainWidget(Echoes::Dbo::Session *session, const std::string &apiUrl)
: Wt::WContainerWidget()
{
    setApiUrl(apiUrl);
    
    
    contentContainer = new Wt::WContainerWidget(this);

    sideBarContainer = new Wt::WContainerWidget();
    footerContainer = new Wt::WContainerWidget();
    
    breadCrumbsContainer = new Wt::WContainerWidget(this);
    
    reset(session);
    
}

void MainWidget::reset(Echoes::Dbo::Session *session)
{
    delete breadCrumbsContainer;
    // voir si utile, refait dans initMenus()
    sideBarContainer->clear();
    footerContainer->clear();
    contentContainer->clear();
    titleText = new Wt::WText();
    breadCrumbsContainer = new Wt::WContainerWidget(this);
    breadCrumbsAnchor0 = new Wt::WAnchor("");
    breadCrumbsAnchor1 = new Wt::WAnchor("");
    breadCrumbsAnchor2 = new Wt::WAnchor("");
    breadCrumbsAnchor0->setRefInternalPath("/welcome");
    created_ = false;
    this->session = session;
    
    try
    {
        Wt::Dbo::Transaction transaction(*(this->session));
        const_cast<Echoes::Dbo::User *>(this->session->user().get());
        transaction.commit();
    }
    catch (Wt::Dbo::Exception e)
    {
        Wt::log("error") << e.what();
    }
    
    // Todo : check, sans doute inutile
    initMenus();
    initFooter();
    contentContainer->setId("content");
    breadCrumbsContainer->setId("breadcrumb");
    
    
}


// Peut-être à supprimer maintenant qu'on ne surcharge plus la méthode
void MainWidget::render(Wt::WFlags<Wt::RenderFlag> flags)
{
//    if (!created_)
//    {
////        update();
//        created_ = true;
//    }

    Wt::WContainerWidget::render(flags);
}

void MainWidget::initMenus(void)
{
    sideBarContainer->clear();
    sideBarContainer->setId("sidebar");
    //    sideBarContainer->hide();
        
    Wt::WAnchor *phoneMenuAnchor = new Wt::WAnchor("#");
    phoneMenuAnchor->setText("Menu");
    phoneMenuAnchor->setAttributeValue("class","visible-phone");
    
    sideBarContainer->insertWidget(0,phoneMenuAnchor);
    menu = new Wt::WMenu(sideBarContainer);
    menu->setInternalPathEnabled("/");
    
}
    
void    MainWidget::initFooter(void)
{
    footerContainer->clear();
    footerContainer->setId("footer");
    Wt::WText   *footerText = new Wt::WText(tr("Footer.text.date") + " "
                            + Wt::WDateTime::currentDateTime().toString("yyyy") + " "
                            + tr("Footer.text") 
                            + "<br />"
                            + "Version " + EchoesHome::version_g);
    footerContainer->insertWidget(0, footerText);
}

void MainWidget::createUI()
{
    if (!created_)
    {
        reset(session);
        initMenus();
        createContentDiv();
        createContainerFluid();

        breadCrumbsAnchor0->setTextFormat(Wt::XHTMLUnsafeText);
        breadCrumbsAnchor0->setText("<i class='icon-home'></i>" + tr("Alert.admin.home"));

        breadCrumbsContainer->addWidget(breadCrumbsAnchor0);


        for (Enums::EPageType::const_iterator i = Enums::EPageType::begin(); i != Enums::EPageType::end(); ++i)
        {
            createMenuItem(*i,menu,getIconName(*i));
            createPage(*i);
        }
        created_ = true;
    }
}

Wt::WContainerWidget * MainWidget::createContentHeader()
{
    Wt::WContainerWidget *res = new Wt::WContainerWidget();
    Wt::WHBoxLayout *layout = new Wt::WHBoxLayout();
    res->setLayout(layout);
    layout->addWidget(titleText);
    res->setId("content-header");
    return res;
}


void MainWidget::createPage(Enums::EPageType enumPT)
{
    switch (enumPT.index())
    {
        case Enums::EPageType::ASSET:
        {
            amw = new AssetManagementWidget(this->session, this->_apiUrl);
            break;
        }
        case Enums::EPageType::WELCOME:
        {
            wcw = new SummaryBoard(this->session);
            break;
        }
        case Enums::EPageType::RECIPIENTS:
        {
            rpw = new RecipientsWidget(this->session, this->_apiUrl);
            break;            
        }
        case Enums::EPageType::INFORMATIONS:
        {
            inw = new InformationsWidget(this->session, this->_apiUrl);
            break;
        }
        case Enums::EPageType::ASSOCIATION:
        {
            act = new Association(this->session, this->_apiUrl);
            break;
        }
        case Enums::EPageType::ALERTS:
        {
            alw = new AlertsWidget(this->session, this->_apiUrl);
            break;            
        }        
        case Enums::EPageType::PLUGIN:
        {
            pew = new PluginEditionWidget(this->session, _apiUrl);
            break;
        }
        case Enums::EPageType::ROLE:
        {
            rcw = new RoleCustomizationWidget(session, _apiUrl);
            break;
        }
        case Enums::EPageType::OPTIONS:
        {
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
            omw = new OptionManagementWidget(omm, this->session, _apiUrl);
            break;
        }
        default:
            break;
    }

}


void MainWidget::createAccountPage(Enums::EAccountSubmenu enumSAC)
{

}

// ToDo : template ?
void MainWidget::updateTitle(unsigned int index, Enums::EMenuRoot menuRoot)
{
    switch (menuRoot)
    {
        case Enums::main:
        {
            for (Enums::EPageType::const_iterator i = Enums::EPageType::begin(); i != Enums::EPageType::end(); ++i)
            {
                if (i->index() == index)
                {
                    this->titleText->setText("<h1>" + tr(boost::lexical_cast<std::string>("Alert.admin.")+i->value()+boost::lexical_cast<std::string>("-tab")) + "</h1>");
                    break;
                }
            }
            break;
        }
        
    }
    
}

void MainWidget::updateBreadcrumbs(Enums::EMenuRoot menuRoot)
{
    this->breadCrumbsContainer->removeWidget(breadCrumbsAnchor2);
    
    std::string internalPath = Wt::WApplication::instance()->internalPath();
    std::vector<std::string> internalPathSplitResult;
    std::vector<std::string> internalPathWithoutBlank;
    boost::split(internalPathSplitResult, internalPath, boost::is_any_of("/"), boost::token_compress_on);
    for (std::vector<std::string>::const_iterator i = internalPathSplitResult.begin() ; i != internalPathSplitResult.end() ; i++)
    {
        if (i->compare("") != 0)
        {
            internalPathWithoutBlank.push_back(*i);
        }
    }
    
    
    for (unsigned int i = 0; i < internalPathWithoutBlank.size(); i++)
    {
        if (i >= 3)
        {
            break;
        }
        
        switch (i+1)
        {
            case 1:
            {
                breadCrumbsAnchor1->setText(tr(boost::lexical_cast<std::string>("Alert.admin.")+internalPathWithoutBlank[i]+boost::lexical_cast<std::string>("-tab")));
                breadCrumbsAnchor1->setRefInternalPath("/" + internalPathWithoutBlank[i]);
                this->breadCrumbsContainer->addWidget(breadCrumbsAnchor1);
                breadCrumbsAnchor1->setAttributeValue("class", getBreadcrumbsClass(internalPathWithoutBlank.size(),1).c_str());
                 break;
            }
            case 2:
            {
                breadCrumbsAnchor2->setText(tr(boost::lexical_cast<std::string>("Alert.admin.")+internalPathWithoutBlank[i]+boost::lexical_cast<std::string>("-tab")));
                breadCrumbsAnchor2->setRefInternalPath("/" + internalPathWithoutBlank[i-1] + "/" + internalPathWithoutBlank[i]);
                this->breadCrumbsContainer->addWidget(breadCrumbsAnchor2);
                breadCrumbsAnchor2->setAttributeValue("class", getBreadcrumbsClass(internalPathWithoutBlank.size(),2).c_str());
                break;
            }
            default:
            {
                break;
            }
        }

    }

}

std::string MainWidget::getBreadcrumbsClass(int pathSize, int level)
{
    std::string res;
    if (pathSize == level)
    {
        res = "current";
    }
    else
    {
        res = "";
    }
    return res;
}

void MainWidget::updateContainerFluid(int type, Enums::EMenuRoot menuRoot)
{
    for (int i = 0 ; i < this->contentFluid->count() ; i++)
    {
        contentFluid->removeWidget(contentFluid->widget(i));
    }
    
    switch (menuRoot)
    {
        case Enums::main:
        {
            switch (type)
            {
                case Enums::EPageType::WELCOME:
                {
                    this->contentFluid->addWidget(wcw);
                    break;
                }
                case Enums::EPageType::ASSET:
                {
                    amw->getResourceList();
                    this->contentFluid->addWidget(amw);
                    break;
                }
                case Enums::EPageType::RECIPIENTS:
                {
                    rpw->update();
                    this->contentFluid->addWidget(rpw);
                    break;
                }
                case Enums::EPageType::INFORMATIONS:
                {
                    inw->getResourceList();
                    this->contentFluid->addWidget(inw);
                    break;
                }
                case Enums::EPageType::ASSOCIATION:
                {
                    act->getResourceList();
                    this->contentFluid->addWidget(act);
                    break;
                }
                case Enums::EPageType::ALERTS:
                {
                    alw->getResourceList();
                    this->contentFluid->addWidget(alw);
                    break;
                }
                case Enums::EPageType::PLUGIN:
                {
                    this->contentFluid->addWidget(pew);
                    break;
                }
                case Enums::EPageType::ROLE:
                {
                    this->contentFluid->addWidget(rcw);
                    break;
                }
                case Enums::EPageType::OPTIONS:
                {
                    this->contentFluid->addWidget(omw);
                    break;
                }
                default:
                    break;
            }
            break;
        }
        
    }
    
    

}

void MainWidget::createContainerFluid()
{
    contentFluid = new Wt::WContainerWidget();
    contentFluid->setStyleClass("container-fluid");
    
    
    contentContainer->addWidget(contentFluid);
}

void MainWidget::createContentDiv()
{
    contentContainer->addWidget(createContentHeader());
    contentContainer->addWidget(breadCrumbsContainer);
}

void MainWidget::doActionMenu(int index, Enums::EMenuRoot menuRoot)
{
    if (index < menu->count())
    {
        menu->itemAt(index)->setFromInternalPath(Wt::WApplication::instance()->internalPath());
        // ToDo open the menu if necessary
        updateTitle(index,menuRoot);
        updateBreadcrumbs(menuRoot);
        updateContainerFluid(index,menuRoot);
    }
    else
    {
        Wt::WApplication::instance()->setInternalPath("/welcome",  false);
        menu->itemAt(0)->setFromInternalPath("/welcome");
        updateTitle(0,Enums::main);
        updateBreadcrumbs(Enums::main);
        updateContainerFluid(0,Enums::main);
    }
    
    
}

std::string MainWidget::getIconName(Enums::EPageType enumPT)
{
    std::string res = "home";
    switch (enumPT.index())
    {
        case Enums::EPageType::WELCOME:
        {
            res = "home";
            break;
        }
        case Enums::EPageType::ASSET:
        {
            res = "hdd";
            break;
        }
        case Enums::EPageType::RECIPIENTS:
        {
            res = "globe"; //group
            break;
        }
        case Enums::EPageType::INFORMATIONS:
        {
            res = "eye-open"; 
            break;
        }
        case Enums::EPageType::ASSOCIATION:
        {
            res = "refresh"; 
            break;
        }
        case Enums::EPageType::ALERTS:
        {
            res = "bell"; 
            break;
        }
        case Enums::EPageType::PLUGIN:
        {
            res = "pencil";
            break;
        }
        case Enums::EPageType::ROLE:
        {
            res = "film";
            break;
        }
        case Enums::EPageType::OPTIONS:
        {
            res = "check";
            break;
        }
        default:
            res = "home";
            break;
    }
    return res;
}

Wt::WMenu * MainWidget::getMenu()
{
    return this->menu;
}

Wt::WMenu * MainWidget::getAccountSubmenu()
{
    return this->accountSubmenu;
}

Wt::WContainerWidget * MainWidget::getSideBarContainer()
{
    return this->sideBarContainer;
}

Wt::WContainerWidget * MainWidget::getFooterContainer()
{
    return this->footerContainer;
}

void MainWidget::close()
{
    delete this;
}

void MainWidget::refresh()
{
//    this->aew->refresh();
//    this->amw->refresh();
}

void MainWidget::setApiUrl(std::string apiUrl)
{
    _apiUrl = apiUrl;
    return;
}

std::string MainWidget::getApiUrl() const
{
    return _apiUrl;
}
