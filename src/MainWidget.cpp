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
    
//    Wt::WApplication *app = Wt::WApplication::instance();
//    app->messageResourceBundle().use("test",false);  
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
    
    alertSubmenu = new Wt::WMenu(sideBarContainer);
    Enums::EPageType *enumPTAl = new Enums::EPageType(Enums::EPageType::SUBMENU_ALERT);
    alertSubmenu->setInternalPathEnabled("/" + boost::lexical_cast<std::string>(enumPTAl->value()) +  "/");
   
    accountSubmenu = new Wt::WMenu(sideBarContainer);
    Enums::EPageType *enumPTAc = new Enums::EPageType(Enums::EPageType::SUBMENU_ACCOUNT);
    accountSubmenu->setInternalPathEnabled("/" + boost::lexical_cast<std::string>(enumPTAc->value()) +  "/");
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

    //    const Wt::WLink *test = new Wt::WLink("");
        breadCrumbsAnchor0->setTextFormat(Wt::XHTMLUnsafeText);
        breadCrumbsAnchor0->setText("<i class='icon-home'></i>" + tr("Alert.admin.home"));
    //    breadCrumbsAnchor0->setLink(*test);

        breadCrumbsContainer->addWidget(breadCrumbsAnchor0);


        for (Enums::EPageType::const_iterator i = Enums::EPageType::begin(); i != Enums::EPageType::end(); ++i)
        {
            if (!boost::starts_with(i->value(), "submenu"))
            {
                createMenuItem(*i,menu,getIconName(*i));
                createPage(*i);
            }
            else
            {
                createSubMenu(*i);
            }
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

void MainWidget::createSubMenu(Enums::EPageType enumPT)
{
    Wt::WMenuItem *itemMenu;
    Wt::WText *labelHTML;
    switch (enumPT.index())
    {
        case Enums::EPageType::SUBMENU_ALERT:
        {
            for (Enums::EAlertSubmenu::const_iterator i = Enums::EAlertSubmenu::begin(); i != Enums::EAlertSubmenu::end(); ++i)
            {
                createMenuItem(*i,alertSubmenu,"");
                createAlertPage(*i);
            }
            
            
            itemMenu = menu->addMenu(tr("Alert.admin." + boost::lexical_cast<std::string>(enumPT.value()) + "-tab"),alertSubmenu);
            labelHTML = new Wt::WText("</span><span class='label'>" + boost::lexical_cast<std::string>(alertSubmenu->items().size()) + "</span><span>" ,Wt::XHTMLUnsafeText);
            break;
        }
        case Enums::EPageType::SUBMENU_ACCOUNT:
        {
            for (Enums::EAccountSubmenu::const_iterator i = Enums::EAccountSubmenu::begin(); i != Enums::EAccountSubmenu::end(); ++i)
            {
                createMenuItem(*i,accountSubmenu,"");
                createAccountPage(*i);
            }
            itemMenu = menu->addMenu(tr("Alert.admin." + boost::lexical_cast<std::string>(enumPT.value()) + "-tab"),accountSubmenu);
            labelHTML = new Wt::WText("</span><span class='label'>" + boost::lexical_cast<std::string>(accountSubmenu->items().size()) + "</span><span>" ,Wt::XHTMLUnsafeText);
            break;
        }
        default:
            break;
    }
    itemMenu->setPathComponent(boost::lexical_cast<std::string>(enumPT.value()));
    Wt::WText *iconHTML = new Wt::WText("</span><i class='icon icon-" + getIconName(enumPT) + "'></i><span>",Wt::XHTMLUnsafeText);
    Wt::WAnchor *anchorInsideMenu = (Wt::WAnchor*)itemMenu->widget(0);
    anchorInsideMenu->clicked().preventPropagation(false);
    anchorInsideMenu->insertWidget(0,iconHTML);
    anchorInsideMenu->addWidget(labelHTML);
    itemMenu->addStyleClass("submenu",true);
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
//            wcw = new Wt::WText(tr("welcome-text"));
            wcw = new SummaryBoard(this->session);
            break;
        }
        case Enums::EPageType::RECIPIENTS:
        {
            rpw = new RecipientsWidget(this->session, this->_apiUrl);
            break;            
        }
        case Enums::EPageType::ALERTS:
        {
            atw = new AlertsWidget(this->session, this->_apiUrl);
            break;            
        }        
        case Enums::EPageType::PLUGIN:
        {
            pew = new PluginEditionWidget(this->session, _apiUrl);
            break;
        }
        default:
            break;
    }

}

void MainWidget::createAlertPage(Enums::EAlertSubmenu enumSAL)
{
    switch (enumSAL.index())
    {
//        case Enums::EAlertSubmenu::ALERT:
//        {
//            aew = new AlertEditionWidget(_apiUrl);
//            try
//            {
//                Wt::Dbo::Transaction transaction(*(this->session));
////                const_cast<Echoes::Dbo::User *>(this->session->user().get());
////                aem = new AlertEditionModel(const_cast<Echoes::Dbo::User *>(this->session->user().get()));
////                aem->setSession(session);
//                transaction.commit();
//            }
//            catch (Wt::Dbo::Exception e)
//            {
//                Wt::log("error") << e.what();
//            }
////            aew->setModel(aem);
////            aew->setSession(session);
//            break;
//        }
        default:
            break;
    }

}

void MainWidget::createAccountPage(Enums::EAccountSubmenu enumSAC)
{
    switch (enumSAC.index())
    {
        case Enums::EAccountSubmenu::OPTION:
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
        case Enums::EAccountSubmenu::ROLE:
        {
            rcw = new RoleCustomizationWidget(session, _apiUrl);
            break;
        }
        default:
            break;
    }

}

// ToDo : template ?
void MainWidget::updateTitle(int index, Enums::EMenuRoot menuRoot)
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
        case Enums::alerts:
        {
            for (Enums::EAlertSubmenu::const_iterator i = Enums::EAlertSubmenu::begin(); i != Enums::EAlertSubmenu::end(); ++i)
            {
                if (i->index() == index)
                {
                    this->titleText->setText("<h1>" + tr(boost::lexical_cast<std::string>("Alert.admin.")+i->value()+boost::lexical_cast<std::string>("-tab")) + "</h1>");
                    break;
                }
            }
            break;
        }
        case Enums::accounts:
        {
            for (Enums::EAccountSubmenu::const_iterator i = Enums::EAccountSubmenu::begin(); i != Enums::EAccountSubmenu::end(); ++i)
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
                const Wt::WLink *test = new Wt::WLink(internalPathWithoutBlank[i]);
                breadCrumbsAnchor1->setText(tr(boost::lexical_cast<std::string>("Alert.admin.")+internalPathWithoutBlank[i]+boost::lexical_cast<std::string>("-tab")));
                breadCrumbsAnchor1->setLink(*test);
                this->breadCrumbsContainer->addWidget(breadCrumbsAnchor1);
                breadCrumbsAnchor1->setAttributeValue("class", getBreadcrumbsClass(internalPathWithoutBlank.size(),1).c_str());
                break;
            }
            case 2:
            {
                const Wt::WLink *test = new Wt::WLink(internalPathWithoutBlank[i-1] + "/" + internalPathWithoutBlank[i]);
                breadCrumbsAnchor2->setText(tr(boost::lexical_cast<std::string>("Alert.admin.")+internalPathWithoutBlank[i]+boost::lexical_cast<std::string>("-tab")));
                breadCrumbsAnchor2->setLink(*test);
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


// ToDo ajouter deux update poru les sous menus
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
                case Enums::EPageType::ASSET:
                {
                    this->contentFluid->addWidget(amw);
                    amw->recoverListAsset();
                    break;
                }
                case Enums::EPageType::WELCOME:
                {
                    this->contentFluid->addWidget(wcw);
                    break;
                }
                case Enums::EPageType::RECIPIENTS:
                {
                    this->contentFluid->addWidget(rpw);
                    break;
                }
                case Enums::EPageType::ALERTS:
                {
                    this->contentFluid->addWidget(atw);
                    atw->recoverListAlert();
                    break;
                }
                case Enums::EPageType::PLUGIN:
                {
                    this->contentFluid->addWidget(pew);
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case Enums::alerts:
        {
            switch (type)
            {
//                case Enums::EAlertSubmenu::ALERT:
//                {
////                    this->contentFluid->addWidget(aew);
//                    break;
//                }
                default:
                    break;
            }
            break;
        }
        case Enums::accounts:
        {
            switch (type)
            {
//                case Enums::EAccountSubmenu::USER:
//                {
//    //                this->contentFluid->addWidget(aew);
//                    break;
//                }
                case Enums::EAccountSubmenu::OPTION:
                {
                    this->contentFluid->addWidget(omw);
                    break;
                }
                case Enums::EAccountSubmenu::ROLE:
                {
                    this->contentFluid->addWidget(rcw);
                    break;
                }  
            }
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
//        case Enums::EPageType::SUBMENU_ALERT:
//        {
//            res = "eye-open";
//            break;
//        }
        case Enums::EPageType::RECIPIENTS:
        {
            res = "user"; //group
            break;
        }
        case Enums::EPageType::ALERTS:
        {
            res = "bell"; //group
            break;
        }
        case Enums::EPageType::SUBMENU_ACCOUNT:
        {
            res = "user";
            break;
        }
        case Enums::EPageType::PLUGIN:
        {
            res = "pencil";
            break;
        }
    }
    return res;
}

Wt::WMenu * MainWidget::getMenu()
{
    return this->menu;
}

Wt::WMenu * MainWidget::getAlertSubmenu()
{
    return this->alertSubmenu;
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
