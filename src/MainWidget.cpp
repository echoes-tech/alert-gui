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
    Wt::WApplication *app = Wt::WApplication::instance();
    app->messageResourceBundle().use("test",false);     
    
    sideBarContainer = new Wt::WContainerWidget(this);
    sideBarContainer->setId("sidebar");
    
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
        createMenuItem(*i);
        createPage(*i);
    }
    
    if (menu->currentIndex() != -1)
    {
        menu->itemSelected().connect(boost::bind(&MainWidget::testMenu, this,-1));
    }
    else 
    {
        menu->itemAt(0)->setFromInternalPath("/welcome");
        menu->itemSelected().connect(boost::bind(&MainWidget::testMenu, this,-1));
    }

    
//    new ScatterPlot(this);
}

Wt::WContainerWidget * MainWidget::createContentHeader()
{
    Wt::WContainerWidget *res = new Wt::WContainerWidget();
    Wt::WHBoxLayout *layout = new Wt::WHBoxLayout();
    Wt::WText *text = new Wt::WText("TEST");
    text->setWidth("60%");
    res->setWidth("70%");
    res->setLayout(layout);
    layout->addWidget(text);
    res->setId("content-header");
    return res;
}

void MainWidget::createMenuItem(Enums::EPageType enumPT)
{
    Wt::WMenuItem *newMenuItem = new Wt::WMenuItem(tr(boost::lexical_cast<std::string>("Alert.admin.")+enumPT.value()+boost::lexical_cast<std::string>("-tab")));
    newMenuItem->setAttributeValue("name",boost::lexical_cast<std::string>(enumPT.index()));
    newMenuItem->setPathComponent(enumPT.value());
    menu->addItem(newMenuItem);
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
        case Enums::EPageType::OPTION:
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


void MainWidget::updateBreadcrumbs()
{
    this->breadCrumbsContainer->removeWidget(breadCrumbsAnchor1);
    this->breadCrumbsContainer->removeWidget(breadCrumbsAnchor2);
    
    std::string internalPath = Wt::WApplication::instance()->internalPath();
    std::vector<std::string> internalPathSplitResult;
    boost::split(internalPathSplitResult, internalPath, boost::is_any_of("/"), boost::token_compress_on);
    
    
    
    for (unsigned int i = 1; i < internalPathSplitResult.size(); i++)
    {
        if (i >= 4)
        {
            break;
        }
        
        switch (i)
        {
            case 1:
            {
                const Wt::WLink *test = new Wt::WLink(internalPathSplitResult[i]);
                breadCrumbsAnchor0->setText(tr(boost::lexical_cast<std::string>("Alert.admin.")+internalPathSplitResult[i]+boost::lexical_cast<std::string>("-tab")));
                breadCrumbsAnchor0->setLink(*test);
                break;
            }
            case 2:
            {
                if (internalPathSplitResult[i].compare(""))
                {
                    const Wt::WLink *test = new Wt::WLink(internalPathSplitResult[i-1] + "/" + internalPathSplitResult[i]);
                    breadCrumbsAnchor0->setText(tr(boost::lexical_cast<std::string>("Alert.admin.")+internalPathSplitResult[i]+boost::lexical_cast<std::string>("-tab")));
                    breadCrumbsAnchor1->setLink(*test);
                    this->breadCrumbsContainer->addWidget(breadCrumbsAnchor1);
                }
                break;
            }
            case 3:
            {
                if (internalPathSplitResult[i].compare(""))
                {
                    const Wt::WLink *test = new Wt::WLink(internalPathSplitResult[i-1] + "/" + internalPathSplitResult[i]);
                    breadCrumbsAnchor0->setText(tr(boost::lexical_cast<std::string>("Alert.admin.")+internalPathSplitResult[i]+boost::lexical_cast<std::string>("-tab")));
                    breadCrumbsAnchor2->setLink(*test);
                    this->breadCrumbsContainer->addWidget(breadCrumbsAnchor2);
                }
                break;
            }
            default:
            {
                break;
            }
        }

    }

}

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
        case Enums::EPageType::OPTION:
        {
            this->contentFluid->addWidget(omw);
            break;
        }
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

void MainWidget::testMenu(int index)
{
    updateBreadcrumbs();
    updateContainerFluid(index);
}

Wt::WMenu * MainWidget::getMenu()
{
    return this->menu;
}

void MainWidget::close()
{
    delete this;
}