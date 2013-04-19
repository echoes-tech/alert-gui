/* 
 * File:   MonitoringWidget.cpp
 * Author: tsa
 * 
 * Created on 14 août 2012, 11:50
 */

#include "MainWidget.h"

MainWidget::MainWidget(Session *session)
: Wt::WContainerWidget()
{
    created_ = false;
    this->session = session;
//    Wt::WApplication *app = Wt::WApplication::instance();
//    app->messageResourceBundle().use("test",false);     
    
    titleText = new Wt::WText();
    
    sideBarContainer = new Wt::WContainerWidget();
    sideBarContainer->setId("sidebar");
    sideBarContainer->hide();
    
    Wt::WAnchor *phoneMenuAnchor = new Wt::WAnchor("#");
    phoneMenuAnchor->setText("Menu");
    phoneMenuAnchor->setAttributeValue("class","visible-phone");
    sideBarContainer->insertWidget(0,phoneMenuAnchor);
    
    contentContainer = new Wt::WContainerWidget(this);
    contentContainer->setId("content");
    
    breadCrumbsContainer = new Wt::WContainerWidget(this);
    breadCrumbsContainer->setId("breadcrumb");
    
    breadCrumbsAnchor0 = new Wt::WAnchor("");
    breadCrumbsAnchor0->setAttributeValue("class","tip-bottom");
    
    breadCrumbsAnchor1 = new Wt::WAnchor("");
    breadCrumbsAnchor1->setAttributeValue("class","tip-bottom");
    
    breadCrumbsAnchor2 = new Wt::WAnchor("");
    breadCrumbsAnchor2->setAttributeValue("class","tip-bottom");
    
    menu = new Wt::WMenu(sideBarContainer);
    menu->setInternalPathEnabled("/");
    
    alertSubmenu = new Wt::WMenu(sideBarContainer);
    Enums::EPageType *enumPTAl = new Enums::EPageType(Enums::EPageType::SUBMENU_ALERT);
    alertSubmenu->setInternalPathEnabled("/" + boost::lexical_cast<std::string>(enumPTAl->value()) +  "/");
   
//    sideBarContainer->widget(0)->setStyleClass("style_widget0");
    
    
//    alertSubmenu->parent()->setStyleClass("style_parent");
//    alertSubmenu->setStyleClass("style_objet");
    
    accountSubmenu = new Wt::WMenu(sideBarContainer);
    Enums::EPageType *enumPTAc = new Enums::EPageType(Enums::EPageType::SUBMENU_ACCOUNT);
    accountSubmenu->setInternalPathEnabled("/" + boost::lexical_cast<std::string>(enumPTAc->value()) +  "/");
    
    
    
//    createUI();
    

    
}

void MainWidget::render(Wt::WFlags<Wt::RenderFlag> flags)
{
    if (!created_)
    {
//        update();
        created_ = true;
    }

    Wt::WContainerWidget::render(flags);
}

void MainWidget::createUI()
{
    createContentDiv();
    createContainerFluid();
    
//    const Wt::WLink *test = new Wt::WLink("");
    breadCrumbsAnchor0->setTextFormat(Wt::XHTMLUnsafeText);
    breadCrumbsAnchor0->setText("<i class='icon-home'></i>" + tr("Alert.admin.home"));
//    breadCrumbsAnchor0->setLink(*test);
    
    breadCrumbsContainer->addWidget(breadCrumbsAnchor0);
    
       
    for (Enums::EPageType::const_iterator i = Enums::EPageType::begin(); i != Enums::EPageType::end(); ++i)
    {
//        Enums::EPageType *test;
//        std::string strTest = test->value();
        if (!boost::starts_with(i->value(), "submenu"))
        {
            createMenuItem(*i,menu,getIconName(*i));
            createPage(*i);
        }
        else
        {
            createSubMenu(*i);
//            Wt::WMenuItem *item;
//            item->setAttributeValue("class", "submenu");
//            menu->items().back()->setAttributeValue("class", "submenu");
//            Wt::WText *iconHTML = new Wt::WText("</span><i class='icon icon-" + getIconName(*i) + "'></i><span>",Wt::XHTMLUnsafeText);
//            Wt::WAnchor *anchorInsideMenu = (Wt::WAnchor*)menu->items().back()->widget(0);
//            anchorInsideMenu->insertWidget(0,iconHTML);
        }
    }
    
    
//    if (menu->currentIndex() != -1)
//    {
//        menu->itemAt(menu->currentIndex())->select();
//    }
//    else 
//    {
//        menu->itemAt(0)->select();
//    }

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

void MainWidget::doNothing()
{
    
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
            break;
        }
        case Enums::EPageType::WELCOME:
        {
            wcw = new Wt::WText(tr("welcome-text"));
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
        case Enums::EAlertSubmenu::ALERT:
        {
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
            break;
        }
        case Enums::EAlertSubmenu::ALERT_LIST:
        {
            alw = new AlertListWidget(this->session);
            break;
        }
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
            omw = new OptionManagementWidget(omm,this->session);
            break;
        }
//        case Enums::EAccountSubmenu::USER:
//        {
//            
//            break;
//        }
        case Enums::EAccountSubmenu::MEDIA:
        {
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

            break;
        }
        default:
            break;
    }

}

// ToDo : temlate ?
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
                    break;
                }
                case Enums::EPageType::WELCOME:
                {
                    this->contentFluid->addWidget(wcw);
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
                case Enums::EAlertSubmenu::ALERT:
                {
                    this->contentFluid->addWidget(aew);
                    break;
                }
                case Enums::EAlertSubmenu::ALERT_LIST:
                {
                    this->contentFluid->addWidget(alw);
                    break;
                }
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
                case Enums::EAccountSubmenu::MEDIA:
                {
                    this->contentFluid->addWidget(uew);
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
    menu->itemAt(index)->setFromInternalPath(Wt::WApplication::instance()->internalPath());
    // ToDo open the menu if necessary
    updateTitle(index,menuRoot);
    updateBreadcrumbs(menuRoot);
    updateContainerFluid(index,menuRoot);
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
        case Enums::EPageType::SUBMENU_ALERT:
        {
            res = "eye-open";
            break;
        }
        case Enums::EPageType::SUBMENU_ACCOUNT:
        {
            res = "user";
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

void MainWidget::close()
{
    delete this;
}

void MainWidget::refresh()
{
    this->amw->refresh();
}
