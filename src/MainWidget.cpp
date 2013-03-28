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
    
    contentContainer = new Wt::WContainerWidget(this);
    contentContainer->setId("content");
    
    breadCrumbsContainer = new Wt::WContainerWidget(this);
    breadCrumbsContainer->setId("breadcrumb");
    
    breadCrumbsAnchor0 = new Wt::WAnchor("");
    breadCrumbsAnchor0->setAttributeValue("class","tip-bottom");
    breadCrumbsAnchor0->setText("Accueil");
    
    breadCrumbsAnchor1 = new Wt::WAnchor("");
    breadCrumbsAnchor1->setAttributeValue("class","tip-bottom");
    
    breadCrumbsAnchor2 = new Wt::WAnchor("");
    breadCrumbsAnchor2->setAttributeValue("class","tip-bottom");
    
    menu = new Wt::WMenu(sideBarContainer);
    menu->setInternalPathEnabled("/");
    
    alertSubmenu = new Wt::WMenu(sideBarContainer);
    Enums::EPageType *enumPTAl = new Enums::EPageType(Enums::EPageType::SUBMENU_ALERT);
    alertSubmenu->setInternalPathEnabled("/" + boost::lexical_cast<std::string>(enumPTAl->value()) +  "/");
    sideBarContainer->widget(0)->setStyleClass("style_widget0");
    
    
    alertSubmenu->parent()->setStyleClass("style_parent");
    alertSubmenu->setStyleClass("style_objet");
    
    accountSubmenu = new Wt::WMenu(sideBarContainer);
    Enums::EPageType *enumPTAc = new Enums::EPageType(Enums::EPageType::SUBMENU_ACCOUNT);
    accountSubmenu->setInternalPathEnabled("/" + boost::lexical_cast<std::string>(enumPTAc->value()) +  "/");
    
//        std::ifstream ifs("resources/themes/bootstrap/js/unicorn.js");
//        std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

    //    Wt::WText * textTest = new Wt::WText("<p>ICI</p><script type='text/javascript' src='" + Wt::WApplication::resourcesUrl() + "themes/bootstrap/js/unicorn.js'></script>", Wt::XHTMLUnsafeText, this);
//        Wt::WText * textTest = new Wt::WText("<p>ICI</p></div></div><script type='text/javascript'>" + content + "</script>", Wt::XHTMLUnsafeText, this);
//        textTest->setInline(false);
    //    textTest->createDomElement()
    
    
//    menu->itemAt(0)->insertWidget(0,textTest);
    
    createContentDiv();
    createContainerFluid();
    
    createUI();
    

    
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
    const Wt::WLink *test = new Wt::WLink("");
    breadCrumbsAnchor0->setText("Accueil");
    breadCrumbsAnchor0->setLink(*test);
    
    breadCrumbsContainer->addWidget(breadCrumbsAnchor0);
    
       
    for (Enums::EPageType::const_iterator i = Enums::EPageType::begin(); i != Enums::EPageType::end(); ++i)
    {
//        Enums::EPageType *test;
//        std::string strTest = test->value();
        if (!boost::starts_with(i->value(), "submenu"))
        {
            createMenuItem(*i,menu);
            createPage(*i);
        }
        else
        {
            createSubMenu(*i);
        }
    }
    
    if (menu->currentIndex() != -1)
    {
        menu->itemAt(menu->currentIndex())->select();
    }
    else 
    {
        menu->itemAt(0)->select();
    }
    
    // add submenu classes to the menu
    for (unsigned int i = 0; i < menu->items().size() ; i++)
    {
        if (menu->items()[i]->menu())
        {
            menu->items()[i]->setStyleClass("submenu");
        }
    }

    
//    new ScatterPlot(this);
}

Wt::WContainerWidget * MainWidget::createContentHeader()
{
    Wt::WContainerWidget *res = new Wt::WContainerWidget();
    Wt::WHBoxLayout *layout = new Wt::WHBoxLayout();
//    titleText->setWidth("60%");
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
    switch (enumPT.index())
    {
        case Enums::EPageType::SUBMENU_ALERT:
        {
            for (Enums::EAlertSubmenu::const_iterator i = Enums::EAlertSubmenu::begin(); i != Enums::EAlertSubmenu::end(); ++i)
            {
                createMenuItem(*i,alertSubmenu);
                createAlertPage(*i);
            }
            menu->addMenu(enumPT.value(),alertSubmenu);
            break;
        }
        case Enums::EPageType::SUBMENU_ACCOUNT:
        {
            for (Enums::EAccountSubmenu::const_iterator i = Enums::EAccountSubmenu::begin(); i != Enums::EAccountSubmenu::end(); ++i)
            {
                createMenuItem(*i,accountSubmenu);
                createAccountPage(*i);
            }
            menu->addMenu(enumPT.value(),accountSubmenu);
            break;
        }
        default:
            break;
    }
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
        default:
            break;
    }

}

void MainWidget::updateTitle(int index)
{
    for (Enums::EPageType::const_iterator i = Enums::EPageType::begin(); i != Enums::EPageType::end(); ++i)
    {
        if (i->index() == index)
        {
            this->titleText->setText("<h1>" + tr(boost::lexical_cast<std::string>("Alert.admin.")+i->value()+boost::lexical_cast<std::string>("-tab")) + "</h1>");
            break;
        }
    }
}

void MainWidget::updateBreadcrumbs()
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
                const Wt::WLink *test = new Wt::WLink(internalPathWithoutBlank[i-1] + "/" + internalPathSplitResult[i]);
                breadCrumbsAnchor2->setText(tr(boost::lexical_cast<std::string>("Alert.admin.")+internalPathSplitResult[i]+boost::lexical_cast<std::string>("-tab")));
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
void MainWidget::updateContainerFluid(int type)
{
    for (int i = 0 ; i < this->contentFluid->count() ; i++)
    {
        contentFluid->removeWidget(contentFluid->widget(i));
    }
    
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
//        case Enums::EPageType::OPTION:
//        {
//            this->contentFluid->addWidget(omw);
//            break;
//        }
//        case Enums::EPageType::ALERT:
//        {
//            this->contentFluid->addWidget(aew);
//            break;
//        }
        default:
            break;
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

void MainWidget::doActionMenu(int index)
{
    menu->itemAt(index)->setFromInternalPath(Wt::WApplication::instance()->internalPath());
    updateTitle(index);
    updateBreadcrumbs();
    updateContainerFluid(index);
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